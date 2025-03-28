//
// Created by chuangbo.cheng on 2020/10/23.
//

#ifndef ANDROID_GPSUBLOX_H
#define ANDROID_GPSUBLOX_H
#include "DataTypeDefine.h"
#include <string>
class GpsUblox{
public:
    Timestamp timestamp = 0;
    int len = 0;
    // 压缩后的16进制数据
//    std::string compressedUblox;

    BYTE *ubloxData = nullptr;

    ~GpsUblox(){
     if (ubloxData != nullptr){
         delete[] ubloxData;
     }
    }
};
#endif //ANDROID_GPSUBLOX_H
