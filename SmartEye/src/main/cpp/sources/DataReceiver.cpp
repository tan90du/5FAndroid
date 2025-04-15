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
#include "EncodeFrame2Jpeg.h"
#include "functional"

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

double calSlope(double s1, double s2, double h1, double h2, double time) {
    double avgSpeed = (s1 + s2) / 2;
    double distance = avgSpeed * time;
    double slope = std::fabs(h1 - h2) / distance * 100;
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

// 病毒母库信息发送标记
bool isSendDiseaseLibrary = false;
// 记录最后一次发送位置上报时间
std::chrono::steady_clock::time_point lastSendTime = std::chrono::steady_clock::now();

void sendCustomJson(const std::string &jsonData) {

    Communication *comm = Communication::getSingleton();
    if (!comm->connected()) {
        Log::info("Sender", "未连接到服务器，发送失败");
        return;
    }

    // 定义数据类型
    const int dataType = 71; // 自定义数据类型，需与接收端一致
    const int jsonLength = static_cast<int>(jsonData.size());

    // 构造数据包：4字节类型 + 4字节JSON长度 + JSON内容
    const int totalSize = 4 + 4 + jsonLength;
    unsigned char *buffer = new unsigned char[totalSize];
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
}

/**
 * 保存视频帧到sdcard
 * @param ygiData
 * @param frameIndex
 */
void saveFrameDataToFile(std::shared_ptr<YGIData> ygiData) {
    SmartFrame *smartFrame = ygiData->getSmartFrame();
    if (smartFrame && smartFrame->getFrameData() != nullptr && smartFrame->getDataSize() > 0) {
        // 假设任务开启时间为 20250401-0810，如果间隔10分钟更新时间，会创建新的文件夹 20250401-0820，用于视频帧的保存
        std::string time =
                CxxCallJavaHelper::call("getCurrentFormattedTime", "");
        // 保存采集的原始图像到original文件夹。 /storage/ext4_sdcard/Android/data/img/original/20250401-0810/
        std::string dirPath =
                CxxCallJavaHelper::call("getAppSDCardPath", "") + "img/original/" + time + "/";
        DIR *dir = opendir(dirPath.c_str());
        if (dir) {
            closedir(dir);
        } else {
            std::string cmd = "mkdir -p ";
            cmd += dirPath;
            system(cmd.c_str());
        }

        // 生成文件名，形如 timestamp-longitude-latitude-altitude-speed.yuv, 年月日-时分秒-微秒
        std::string filename = dirPath +
                               std::to_string(ygiData->getSmartFrame()->getTimestamp()) + "-" +
                               std::to_string(ygiData->getGpsVector()->at(0)->getLng()) + "-" +
                               std::to_string(ygiData->getGpsVector()->at(0)->getLat()) + "-" +
                               std::to_string(ygiData->getGpsVector()->at(0)->getAltitude()) + "-" +
                               std::to_string(ygiData->getGpsVector()->at(0)->getSpeed()) + ".yuv";
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

// 运行在编码线程
void DataReceiver::encodeFinish(int code, std::shared_ptr<YGIData> ygiData) {
    /*if (mYgiDataMap == nullptr || code != 0){
        return;
    }
    int static index = 0;

    u8 t1 = TimeUtils::currentTimeMicrosecond();
    // {"interval":2}
    // 获取开机时长(毫秒)
    struct timespec ts;
    clock_gettime(CLOCK_BOOTTIME, &ts);
    long long bootTime = (long long)ts.tv_sec * 1000 + (long long)(ts.tv_nsec/1000000);
    bootTime *= 1000;
    static u8 baseDiff = ygiData->getSmartFrame()->getTimestamp() - bootTime;
    u8 currentDiff = ygiData->getSmartFrame()->getTimestamp() - bootTime;
    u8 interval = baseDiff >= currentDiff ? baseDiff - currentDiff : currentDiff - baseDiff;
    ygiData->getSmartFrame()->moreField.append("{\"interval\":")
            .append(std::to_string(interval)).append("}");
    // 质检
    doYGIQuality(ygiData);
    ST::SmartRecursiveLock lock(mutex, "YGIDataMemoryCache.encodeFinish");*/

    Communication::getSingleton()->sendYGIData(ygiData);

   /* ygiData->setStatus(EnumVIOStatus::Default);
    auto *smartFrame = ygiData->getSmartFrame();

    // 检查下当前的容量是否大于maxMemory 如果依然超出缓存 则强制移除最早的一个(这种情况一般为vio异常或者长时间堵车导致)
    while (currentMemorySize + smartFrame->getLength() > mDataCacheParams.maxMemory){
        // 移除最早的一帧
        std::map<u8, std::shared_ptr<YGIData>>::iterator iterator = mYgiDataMap->begin();
        currentMemorySize -= iterator->second->getSmartFrame()->getLength();
//        Log::info("YGIDataMemoryCache", "缓存已满,删除了第一帧 %llu", iterator->first);
        mYgiDataMap->erase(iterator);
    }
    // 添加到map的末尾
    mYgiDataMap->insert(std::make_pair(smartFrame->getTimestamp(), ygiData));
    // 将当前的size加上
    currentMemorySize += smartFrame->getLength();
    u8 t2 = TimeUtils::currentTimeMicrosecond();
    if (index % 100 == 0 || (t2 - t1) > 2000){
        Log::info("YGIDataMemoryCache", "size = %d, 耗时 %lld, moreField = %s",
                  mYgiDataMap->size(), t2 - t1, ygiData->getSmartFrame()->moreField.c_str());
    }
    index++;
    if (index > 100000){
        index = 0;
    }*/
}


/**
 * 帧回调处理
 * @param frameIndex
 */
void frameDataCallback(uint32_t frameIndex) {

    //发送病害母库列表
//    if (!isSendDiseaseLibrary) {
//        String diseaseList = CxxCallJavaHelper::call("getDiseaseLibrary", "");
//        sendCustomJson(diseaseList.c_str());
//        isSendDiseaseLibrary = true;
//    }

    std::shared_ptr<YGIData> ygiData(new YGIData());
    HanderReaderFramebuf(frameIndex, shareBuf, ygiData);

    if (ygiData == nullptr || ygiData->getSmartFrame() == nullptr) {
        return;
    }

    if (!ygiData->getGpsVector()->empty()) {
        //WGS84地图坐标转换成高德和百度地图坐标
        auto gcj02 = wgs84_to_gcj02(ygiData->getGpsVector()->at(0)->getLng(),
                                    ygiData->getGpsVector()->at(0)->getLat());
//        Log::info("DataReceiver", "高德地图坐标x:%.8f,坐标y:%.8f", gcj02.first, gcj02.second);

//        auto bd09 = gcj02_to_bd09(gcj02.first, gcj02.second);
//        Log::info("DataReceiver", "百度地图坐标x:%.8f,坐标y:%.8f", bd09.first, bd09.second);

        /*Log::info("DataReceiver",
                  "time = %lld, x = %.8f, y = %.8f, status = %d, Altitude = %.8f, speed = %.8f,",
                  ygiData->getSmartFrame()->getTimestamp(),
                  ygiData->getGpsVector()->at(0)->getLng(),
                  ygiData->getGpsVector()->at(0)->getLat(),
                  ygiData->getGpsVector()->at(0)->getGpsStatus(),
                  ygiData->getGpsVector()->at(0)->getAltitude(),
                  ygiData->getGpsVector()->at(0)->getSpeed());
        Log::info("DataReceiver", "ygiData 获取到一帧");*/

        // 构造包含经纬度的JSON字符串
        char jsonStr[256];
//        double lng = ygiData->getGpsVector()->at(0)->getLng();
//        double lat = ygiData->getGpsVector()->at(0)->getLat();
        double lng = gcj02.first;
        double lat = gcj02.second;

        snprintf(jsonStr, sizeof(jsonStr), R"(%.8f,%.8f)", lng, lat);
        std::string res = CxxCallJavaHelper::call("sendDeviceLocation", jsonStr);

        // 查询任务状态
        std::string taskStatus = CxxCallJavaHelper::call("getTaskStatus", "");

        // 经纬度合法且任务开启时才进行逻辑处理
        if (lng <= 180 && lat <= 90 && "open" == taskStatus) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                    now - lastSendTime);

            if (elapsed.count() >= 200) { // 定时发送
                // 发送合法图片到算力终端
                Log::info("DataReceiver", "服务端发送数据给算力平台");
                Communication::getSingleton()->sendYGIData(ygiData);
                lastSendTime = now;
            }

            // 保存帧
//            EncodeFrame2Jpeg* yuv2jpg = new EncodeFrame2Jpeg();
//            // 编码为jpeg
//            yuv2jpg->encodeFrame2Jpeg(ygiData, 80);

            // 计算道路坡度

            // 计算道路曲率

        }
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
 * 帧格式转换和保存
 * @param ygiData
 */
//void FrameConverseAndSave(std::shared_ptr<YGIData> ygiData){
//    auto yuv2jpg = std::make_unique<EncodeFrame2Jpeg>();
//    yuv2jpg->encodeFrame2Jpeg(ygiData, 80); // 保存的图片质量，100为原始大小
//    saveFrameDataToFile(ygiData);
//}

/**
 * 示例，启动数据接口
 */
void DataReceiver::start() {

    // 如果是养护设备
    Communication::getSingleton()->init([] {
        Log::info("Application", "TX2连接成功");
    });

    std::thread thread(handlerReaderFrame);
//    std::thread thread_yuv2jpg(FrameConverseAndSave);
    thread.detach();
//    thread_yuv2jpg.detach();
}
