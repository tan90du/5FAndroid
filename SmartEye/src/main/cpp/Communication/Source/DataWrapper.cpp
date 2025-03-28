//
// Created by 郑伟 on 2021/5/14.
//

#include "DataWrapper.h"
#include "malloc.h"
#include "Communication.h"
#include <string.h>
#include <Log.h>

void DataWrapper::wrapYGIData(std::shared_ptr<YGIData> ygiData, int32_t& output_length, unsigned char** out_data) {

    // "timestamp":1622705351635000
    std::string info = "{\"timestamp\":";
    info.append(std::to_string(ygiData->getSmartFrame()->getTimestamp()));
    double lat = 0;
    double lng = 0;
    double speed = 0;
    if (ygiData->getGpsVector()->size() > 0) {
        lat = ygiData->getGpsVector()->at(0)->getLat();
        lng = ygiData->getGpsVector()->at(0)->getLng();
        speed = ygiData->getGpsVector()->at(0)->getSpeed();
    }
    char gpsInfo[32];
    sprintf(gpsInfo, "%.8f", lat);
    info.append(",\"lat\":").append(gpsInfo);
    sprintf(gpsInfo, "%.8f", lng);
    info.append(",\"lng\":").append(gpsInfo);
    sprintf(gpsInfo, "%.2f", speed);
    info.append(",\"speed\":").append(gpsInfo);
    info.append("}");
//    auto vioResult = ygiData->getVioResult()->info;
    int vioLength =  info.length();
    auto jpgData = ygiData->getSmartFrame()->getFrameData();
    int jgpLength  = ygiData->getSmartFrame()->getLength();
    // 4 字节位编号 + 4字节vioLength + 4字节jpgData + jgpLength
    output_length = 4 +  4 + vioLength + 4 + jgpLength;
    *out_data = (unsigned char*)malloc(output_length);

    int code = FuncCode::BigEye_TX2_0020;
    memcpy(*out_data, &code, 4);

    int len = vioLength;
    memcpy(*out_data + 4, &len, 4);

    // 设置vio数据
    memcpy(*out_data + 4 + 4, info.c_str(), vioLength);
    // 设置jpeg len
    int lenJpeg = jgpLength;
    memcpy(*out_data + 4 + 4 + vioLength, &lenJpeg, 4);

    // 设置jpeg数据
    memcpy(*out_data + 4 + 4 + vioLength + 4, jpgData, jgpLength);



//    int jgpLength  = ygiData->getSmartFrame()->getLength();
//    output_length = jgpLength;
//    *out_data = (unsigned char*)malloc(output_length + 4);
//    memcpy(*out_data, &jgpLength, 4);
//
//    auto jpgData = ygiData->getSmartFrame()->getFrameData();
//    memcpy(*out_data + 4, jpgData, jgpLength);
//
//    Log::info("DataWrapper", " output_length = %d ", jgpLength);
}

inline int bytesToInt(const char* bytes,int size ) {
    int a = bytes[0] & 0xFF;
    a |= ((bytes[1] << 8) & 0xFF00);
    a |= ((bytes[2] << 16) & 0xFF0000);
    a |= ((bytes[3] << 24) & 0xFF000000);
    return a;
}

inline std::shared_ptr<BaseTXData> parseRoadDisease(const char* data, int32_t length) {

    std::shared_ptr<RoadDisease> dataWrapper = std::make_shared<RoadDisease>();


    char jsonDataLen[4];
    memcpy(jsonDataLen, data + 4, 4);
    int dataLength = bytesToInt(jsonDataLen, 4);


    char *jsonData = (char*)malloc(dataLength + 1);
    memset(jsonData, 0, dataLength + 1);
    memcpy(jsonData, data + 8, dataLength);

    Log::info("DataWrapper", "接收到算力平台返回的数据 jsonData = %s ", jsonData);

//    // 解析json
//    Json::Reader reader;
//    Json::Value  value;
//    if (!reader.parse(jsonData, value, false)) {
//        free(jsonData);
//        return nullptr;
//    }
//    unsigned long long timestamp = value.get("timestamp", 0).asULongLong();
//    dataWrapper->timestamp = timestamp;
//
//    std::string info = value.get("info", "").asString();
//    dataWrapper->info = info;
//    dataWrapper->parseSuccess = true;
//    int diseaseUse = value.get("type", 3).asInt();
//    dataWrapper->diseaseUse = diseaseUse;
//    dataWrapper->type = 1;
//    free(jsonData);


    return dataWrapper;
}


std::shared_ptr<BaseTXData> DataWrapper::parse(const char* data, int32_t length){

    std::shared_ptr<BaseTXData> dataWrapper = nullptr;
    if (length <= 4) {
        return dataWrapper;
    }
    int data_type = bytesToInt(data, 4);
    Log::info("DataWrapper", "接收到 data_type = %d", data_type);
    switch (data_type){
        case FuncCode::BigEye_TX2_0070:
            dataWrapper = parseRoadDisease(data, length);
            break;
        default:
            Log::info("Communication_", "data_type = %d 未定义");
            break;
    }
    dataWrapper->code = data_type;
    return dataWrapper;
}
