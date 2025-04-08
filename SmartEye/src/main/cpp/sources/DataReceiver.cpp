//
// Created by chuangbo.cheng on 2020/8/27.
//

#include "DataExchangeReader.h"
#include "DataReceiver.h"
#include "HDClient.h"
#include "MainControl.h"
#include "YGIData.h"
#include <stdio.h>
#include <thread>
#include <unistd.h>
#include "Log.h"
#include "Communication.h"
#include "CxxCallJavaHelper.h"
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <cmath>
#include <utility>
using namespace std;

const double PI = 3.14159265358979324;

static double transform_lat(double x, double y);
static double transform_lon(double x, double y);

pair<double, double> wgs84_to_gcj02(double lon, double lat) {
    double a = 6378245.0;
    double ee = 0.00669342162296594323;

    double dlat = transform_lat(lon - 105.0, lat - 35.0);
    double dlon = transform_lon(lon - 105.0, lat - 35.0);

    double rad_lat = lat * PI / 180.0;
    double magic = sin(rad_lat);
    magic = 1 - ee * magic * magic;
    double sqrt_magic = sqrt(magic);

    dlat = (dlat * 180.0) / ((a * (1 - ee)) / (magic * sqrt_magic) * PI);
    dlon = (dlon * 180.0) / (a / sqrt_magic * cos(rad_lat) * PI);

    return make_pair(lon + dlon, lat + dlat);
}

pair<double, double> gcj02_to_bd09(double lon, double lat) {
    double x_pi = 3000.0 / 180.0 * PI;
    double z = sqrt(lon * lon + lat * lat) + 0.00002 * sin(lat * x_pi);
    double theta = atan2(lat, lon) + 0.000003 * cos(lon * x_pi);

    double bd_lon = z * cos(theta) + 0.0065;
    double bd_lat = z * sin(theta) + 0.006;

    return make_pair(bd_lon, bd_lat);
}

double transform_lat(double x, double y) {
    double ret = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y
                 + 0.1 * x * y + 0.2 * sqrt(fabs(x));
    ret += (20.0 * sin(6.0 * x * PI) + 20.0 * sin(2.0 * x * PI)) * 2.0 / 3.0;
    ret += (160.0 * sin(y * PI / 12.0) + 320 * sin(y * PI / 30.0)) * 2.0 / 3.0;
    return ret;
}

double transform_lon(double x, double y) {
    double ret = 300.0 + x + 2.0 * y + 0.1 * x * x
                 + 0.1 * x * y + 0.1 * sqrt(fabs(x));
    ret += (20.0 * sin(6.0 * x * PI) + 20.0 * sin(x * PI)) * 2.0 / 3.0;
    ret += (150.0 * sin(x * PI / 12.0) + 300.0 * sin(x * PI / 30.0)) * 2.0 / 3.0;
    return ret;
}

double calSlope(double s1,double s2,double h1,double h2,double time){
    double avgSpeed = (s1+s2)/2;
    double distance = avgSpeed*time;
    double slope = std::fabs(h1-h2)/distance*100;
    return slope;
}

void *shareBuf;
size_t shareBufLen;

///**
//* 获取文件大小,字节数
//*/
//int getFileSize(std::string path) {
//    struct stat buf;//文件字节数
//    int res = stat(path.c_str(), &buf);
//    if (res != 0) {
//        return 0;
//    } else {
//        return buf.st_size;
//    }
//    return 0;
//}
void serverDieCallback(void) {
    LOGI("HD server died");
    Log::info("DeviceSystem", "serverDieCallback!!!!!");
}

bool flag = 1;
long pre = 0;
double pre_speed = 0;
double pre_h = 0;
double now_speed = 0;
double now_h = 0;

bool flag_getDiseaseLibrary=1;

void sendCustomJson(const std::string& jsonData) {

    Communication* comm = Communication::getSingleton();
    if (!comm->connected()) {
        Log::info("Sender", "未连接到服务器，发送失败");
        return;
    }

    // 定义数据类型
    const int dataType = 71; // 自定义数据类型，需与接收端一致
    const int jsonLength = static_cast<int>(jsonData.size());

    // 构造数据包：4字节类型 + 4字节JSON长度 + JSON内容
    const int totalSize = 4 + 4 + jsonLength;
    unsigned char* buffer = new unsigned char[totalSize];
    int offset = 0;

    // 填充数据类型
    memcpy(buffer + offset, &dataType, 4);
    offset += 4;

    // 填充JSON长度
    memcpy(buffer + offset, &jsonLength, 4);
    offset += 4;

    // 填充JSON内容
    memcpy(buffer + offset, jsonData.data(), jsonLength);

    // 发送数据
    comm->send(buffer, totalSize);

    delete[] buffer;
    Log::info("Sender", "JSON数据已发送");
    flag_getDiseaseLibrary=0;
}
/**
 * 保存视频帧到sdcard
 * @param ygiData
 * @param frameIndex
 */
void saveFrameDataToFile(std::shared_ptr<YGIData> ygiData, uint32_t frameIndex) {
    SmartFrame *smartFrame = ygiData->getSmartFrame();
    if (smartFrame && smartFrame->getFrameData() != nullptr && smartFrame->getDataSize() > 0) {
        //创建文件夹
        std::string dirPath = CxxCallJavaHelper::call("getAppSDCardPath", "") + "img/";
        DIR *dir = opendir(dirPath.c_str());
        if (dir) {
            closedir(dir);
        } else {
            std::string cmd = "mkdir -p ";
            cmd += dirPath;
            system(cmd.c_str());
        }
        // 生成文件名，例如 frame_100.yuv
        std::string filename = dirPath +  std::to_string(frameIndex) + ".yuv";
        // 打开文件进行二进制写入
        std::ofstream outFile(filename, std::ios::out | std::ios::binary);
        if (outFile.is_open()) {
            outFile.write(reinterpret_cast<const char *>(smartFrame->getFrameData()),
                          smartFrame->getDataSize());
            outFile.close();
            Log::info("DataReceiver", "视频帧保存成功: %s", filename.c_str());
        } else {
            Log::info("DataReceiver", "视频帧保存失败: %s", filename.c_str());
        }
    } else {
        Log::info("DataReceiver", "无效视频帧");
    }
}


/**
 * 帧回调处理
 * @param frameIndex
 */
void frameDataCallback(uint32_t frameIndex) {

    std::shared_ptr<YGIData> ygiData(new YGIData());
    HanderReaderFramebuf(frameIndex, shareBuf, ygiData);

    if (ygiData == nullptr || ygiData->getSmartFrame() == nullptr) {
        return;
    }

    if (!ygiData->getGpsVector()->empty()) {
        Log::info("DataReceiver", "time = %lld, x = %.8f, y = %.8f, status = %d, Altitude = %.8f, speed = %.8f,",
                  ygiData->getSmartFrame()->getTimestamp(),
                  ygiData->getGpsVector()->at(0)->getLng(),
                  ygiData->getGpsVector()->at(0)->getLat(),
                  ygiData->getGpsVector()->at(0)->getGpsStatus(),
                  ygiData->getGpsVector()->at(0)->getAltitude(),
                  ygiData->getGpsVector()->at(0)->getSpeed());
        Log::info("DataReceiver", "ygiData 获取到一帧");

//        std::string str = CxxCallJavaHelper::call("UpLoadImg", "");

        // 构造包含经纬度的JSON字符串
        char jsonStr[256];
        snprintf(jsonStr, sizeof(jsonStr),
                 R"(%.8f,%.8f)",
                 ygiData->getGpsVector()->at(0)->getLng(),
                 ygiData->getGpsVector()->at(0)->getLat());
        String res = CxxCallJavaHelper::call("getDeviceLocation", jsonStr);

        //WGS84地图坐标转换成高德和百度地图坐标
        auto gcj02 = wgs84_to_gcj02(ygiData->getGpsVector()->at(0)->getLng(), ygiData->getGpsVector()->at(0)->getLat());
        Log::info("DataReceiver", "高德地图坐标x:%.8f,坐标y:%.8f",gcj02.first,gcj02.second);

        auto bd09 = gcj02_to_bd09(gcj02.first, gcj02.second);
        Log::info("DataReceiver", "百度地图坐标x:%.8f,坐标y:%.8f",bd09.first,bd09.second);

//        // 计算坡度
//        double test = calSlope(6,10,6,10,2);
//        Log::info("DataReceiver", "6 10 6 10 2 测试坡度为:%.8f%%",test);
//        long time = ygiData->getSmartFrame()->getTimestamp()/100000;
//        int interval = 2;
//        if(flag){
//            pre = time;
//            pre_speed=ygiData->getGpsVector()->at(0)->getSpeed();
//            pre_h=ygiData->getGpsVector()->at(0)->getAltitude();
//            flag=0;
//        }
//        if(flag==0){
//            if(time-pre>=interval){
//                now_speed=ygiData->getGpsVector()->at(0)->getSpeed();
//                now_h=ygiData->getGpsVector()->at(0)->getAltitude();
//                double slope = calSlope(pre_speed,now_speed,pre_h,now_h,interval);
//                Log::info("DataReceiver", "时间间隔为:%d",interval);
//                Log::info("DataReceiver", "起点时间为:%lld",pre);
//                Log::info("DataReceiver", "终点时间为:%lld",time);
//                Log::info("DataReceiver", "起点速度为:%.8f",pre_speed);
//                Log::info("DataReceiver", "终点速度为:%.8f",now_speed);
//                Log::info("DataReceiver", "起点高度为:%.8f",pre_h);
//                Log::info("DataReceiver", "终点高度为:%.8f",now_h);
//                Log::info("DataReceiver", "坡度为:%.8f%%",slope);
//                pre_speed=now_speed;
//                pre_h=now_h;
//                pre=time;
//            }
//        }

        //发送病害母库列表
        if(flag_getDiseaseLibrary==1){
//            String res2 = CxxCallJavaHelper::call("getDiseaseLibrary", "");
//            Log::info("DataReceiver", "getDiseaseLibrarylihongwei:%s",res2.c_str());
            // 构造JSON字符串
            std::string jsonStr = R"({
              "code": 200,
              "message": "成功",
              "data": [
                {
                  "id": 1,
                  "uniqueId": "病害唯一id",
                  "pendingId": 1001,
                  "equipmentId": 2001,
                  "inspectionRecordId": 3001,
                  "parentId": 0,
                  "type": "类型1:端AI病害, 2:全量图片",
                  "imageUrl": "图片地址",
                  "diseaseType": "病害类型",
                  "road": "道路",
                  "location": "位置 经纬度",
                  "direction": "病害方向",
                  "quantity": "量化",
                  "reportTime": "上报时间2025-04-01 11:03:47",
                  "position": "病害标注信息",
                  "highlightImageUrl": "已标注的照片",
                  "diseaseStatus": "病害状态",
                  "deptId": 123
                }
              ]
            })";

//            // 发送JSON数据
//            Communication::getSingleton()->send(
//                    reinterpret_cast<const unsigned char*>(jsonStr.c_str()),
//                    static_cast<int>(jsonStr.size())
//            );
            sendCustomJson(jsonStr.c_str());
        }


        Log::info("DataReceiver", "服务端保存视频帧到sdcard");
        saveFrameDataToFile(ygiData, frameIndex);

        Log::info("DataReceiver", "服务端发送数据给算力平台");
        Communication::getSingleton()->sendYGIData(ygiData);

//        Log::info("DataReceiver", "服务端上传Zip帧压缩包到SaaS平台");

    } else {
        Log::info("DataReceiver", "GPS信坐标获取失败");
    }

    // 模拟数据接口调用
//    int len = getFileSize("/sdcard/0test/111.jpg");
//    FILE* fd = fopen("/sdcard/0test/111.jpg", "rb");
//    if (len >0 && fd != NULL) {
//        auto jpgBuffer = new BYTE[len];
//        fread(jpgBuffer, len, 1, fd);
//        ygiData->getSmartFrame()->setFrameData(jpgBuffer);
//        ygiData->getSmartFrame()->setType(EnumImage::JPEG);
//        ygiData->getSmartFrame()->setLength(len);
//        fclose(fd);
//        Log::info("YGIDataMemoryCache", "发送了一张模拟的病害图片 %lld", ygiData->getSmartFrame()->getTimestamp());
//        Communication::getSingleton()->sendYGIData(ygiData);
//    }
    // 发送yuv数据给算力平台

}

// 事件回调接口
void EventCallbackHandler(uint32_t Module, uint32_t ID, uint32_t type, uint16_t len,
                          const uint8_t *payload) {
//
//    Log::info("DeviceSystem", "Module = %d, ID = %d, type = %d, len = %d, payload = %s",
//              Module, ID, type, len, payload);
}

//Log 回调函数
void LogDataCallbackHandler(uint32_t frameIndex) {
    static int32_t curIndex = -1;
    curIndex = ReaderLogDataFromDataExchange(shareBuf, curIndex, frameIndex);
}

/**
 * 初始化
 */
void handlerReaderFrame() {

    Log::info("DataReceiver", "handlerReaderFrame");

    int ret = 0;
    autonavi::HDCOLLECT::HDClient_ConnectService(serverDieCallback);

    //注册event事件监听函数.(监听error类型的事件)
    autonavi::HDCOLLECT::MainControl_registerEventListener(0xffff, 0xffff, EVENT_ALL,
                                                           EventCallbackHandler);
    //注册采集log监听函数.（用于log收集）
    autonavi::HDCOLLECT::HDClient_registerLogListener(LogDataCallbackHandler);

    ret = autonavi::HDCOLLECT::HDClient_getShareMemory(&shareBuf, &shareBufLen);
    if (shareBuf == nullptr) {
        Log::info("DataReceiver", "shareBuf == nullptr");
        return;
    }
    autonavi::HDCOLLECT::HDClient_startHDCamera(frameDataCallback);
}

/**
 * 示例，启动数据接口
 */
void DataReceiver::start() {

    // 如果是养护设备
    Communication::getSingleton()->init([] {
        Log::info("Application", "TX2连接成功");
    });


    std::thread thread(handlerReaderFrame);
    thread.detach();
}
