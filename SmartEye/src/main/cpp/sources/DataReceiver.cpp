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


/**
 * 保存视频帧到sdcard
 * @param ygiData
 * @param frameIndex
 */
void saveFrameDataToFile(std::shared_ptr<YGIData> ygiData, uint32_t frameIndex) {
    SmartFrame *smartFrame = ygiData->getSmartFrame();
    if (smartFrame && smartFrame->getFrameData() != nullptr && smartFrame->getDataSize() > 0) {
        // 生成文件名，例如 frame_100.yuv
        std::string filename =
                "/data/data/com.autonavi.smarteye/" + std::to_string(frameIndex) + ".yuv";
//        std::string filename = "/sdcard/DCIM/Camera/" + std::to_string(frameIndex) + ".yuv";

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
        Log::info("DataReceiver", "time = %lld, x = %.8f, y = %.8f, status = %d",
                  ygiData->getSmartFrame()->getTimestamp(),
                  ygiData->getGpsVector()->at(0)->getLng(),
                  ygiData->getGpsVector()->at(0)->getLat(),
                  ygiData->getGpsVector()->at(0)->getGpsStatus());
        Log::info("DataReceiver", "ygiData 获取到一帧");

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
