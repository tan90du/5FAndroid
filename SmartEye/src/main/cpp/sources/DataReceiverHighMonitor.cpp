//
// Created by chuangbo.cheng on 2020/9/22.
//

#include "DataReceiverHighMonitor.h"
#include <math.h>
#include <map>
#include <thread>
#include <unistd.h>
#include <sys/stat.h>
#include "Log.h"
//#include "Communication.h"

/* ======================================================= */
/* Test Methods                                            */
/* ======================================================= */

namespace DataReceiverMock {

#define DRM_HEADER { std::shared_ptr<SmartGPS> gps(new SmartGPS()); gps->setLng(
#define DRM_MIDDLE );   gps->setLat(
#define DRM_END );   outGpsVector.push_back(gps); DRM_Interpolation }
#define DRM_Interpolation if (last != nullptr) {\
        double deltaLat = (gps->getLat() - last->getLat()) / 10;\
        double deltaLng = (gps->getLng() - last->getLng()) / 10;\
        for (int i = 1; i < 10; i++) {\
            double tempLat = last->getLat() + deltaLat;\
            double tempLng = last->getLng() + deltaLng;\
            std::shared_ptr<SmartGPS> temp(new SmartGPS());\
            temp->setLat(tempLat);\
            temp->setLng(tempLng);\
            outGpsVector.push_back(temp);\
        }\
    }\
    last = gps;
}




#define Smart_YUV_Width 1920
#define Smart_YUV_Height 1080
DataReceiverHighMonitor::DataReceiverHighMonitor(){

}

void DataReceiverHighMonitor::start(){

    std::thread thread(std::bind(&DataReceiverHighMonitor::startMock, this));
    thread.detach();
}

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


void DataReceiverHighMonitor::startMock(){

    std::vector<std::string> ridVector;


    double lng = 116.47300782312;
    double lat = 39.9933167834;


    int length = Smart_YUV_Width * Smart_YUV_Height * 1.5;
    // 生成yuv
    bool needGps = true;
    u8 microsecond = 0;
    BYTE *frameData = new BYTE[length];
    memset(frameData, 255, length);

    BYTE *frameScaleData = new BYTE[345600];
    memset(frameScaleData, 255, 345600);
    while(1){
        std::shared_ptr<YGIData> ygiData(new YGIData());
        microsecond = 100;
        if (needGps){
            needGps = false;
            auto gps = new SmartGPS();
            gps->setLng(lng);
            gps->setLat(lat);
            gps->setTimestamp(microsecond - 10);
            // 获取系统运行的时长
            struct timespec ts;
            clock_gettime(CLOCK_BOOTTIME, &ts);
            long long bootTime = (long long)ts.tv_sec * 1000 + (long long)(ts.tv_nsec/1000000);
            gps->bootTime = bootTime * 1000;
            gps->setBearing(30);
//            gps->setSpeed(40.3);
            gps->setSpeed(20);
            ygiData->getGpsVector()->push_back(gps);
            ygiData->setGpsUblox(new GpsUblox);
            ygiData->getGpsUblox()->len = 0;
            ygiData->getGpsUblox()->timestamp = microsecond;
            // 每100毫秒 走2米
            lat += 0.00002;
        }else{
            needGps = true;
        }
        // 生成imu
        for (int i = 0; i < 20; ++i) {
            auto imu = new SmartIMU();
            imu->setTimestamp(microsecond - (20 - i));
            imu->setGyroX(1);
            imu->setGyroY(2);
            imu->setGyroZ(3);
            imu->setAccelX(4);
            imu->setAccelY(5);
            imu->setAccelZ(6);
            ygiData->getImuVector()->push_back(imu);
        }
        // 生成yuv

        SmartFrame *frame = ygiData->getSmartFrame();
        frame->setTimestamp(microsecond);
        frame->setHeight(Smart_YUV_Height);
        frame->setWidth(Smart_YUV_Width);
        frame->setLength(length);
        frame->setFrameData(frameData);
        frame->integration = 1655;
        frame->lineOfIntegration = 22;
        ygiData->setSmartFrame(frame);
        ygiData->getSmartFrame()->scaleLength = 345600;
        ygiData->getSmartFrame()->frameScaleData = frameScaleData;
        Log::info("DataReceiverHighMonitor", "接收到一帧yuv数据");

        // 处理yuv数据 .....

        // 发送yuv数据给算力平台
//        int len = getFileSize("/sdcard/0test/111.jpg");
//        FILE* fd = fopen("/sdcard/0test/111.jpg", "rb");
//        if (len >0 && fd != NULL) {
//            auto jpgBuffer = new BYTE[len];
//            fread(jpgBuffer, len, 1, fd);
//            ygiData->getSmartFrame()->setFrameData(jpgBuffer);
//            ygiData->getSmartFrame()->setType(EnumImage::JPEG);
//            ygiData->getSmartFrame()->setLength(len);
//            fclose(fd);
//            Log::info("YGIDataMemoryCache", "发送了一张模拟的病害图片 %lld", ygiData->getSmartFrame()->getTimestamp());
////            Communication::getSingleton()->sendYGIData(ygiData);
//        }


        // 每秒20帧
        usleep(1000 * 1000);

    }
}




