//
// Created by chuangbo.cheng on 2021/6/7.
// 道路病害
//

#ifndef ANDROID_ROADDISEASE_H
#define ANDROID_ROADDISEASE_H

#include <string>
#include <BaseTXData.h>
#include "YGIData.h"

class RoadDisease : public BaseTXData {
public:

    ~RoadDisease() {

    }
    unsigned long long timestamp = 0;
    // 0 人工打点  1 算法识别（仅上传单张图片等信息） 2 服务端配置自动触发
    int type = 0;
    // 病害的用途 1 pad显示用  2 需要上传  3 显示&上传
    int diseaseUse = 0;
    std::string info;

    // 当前病害的经纬度
    double x = 0;
    double y = 0;

    // 对应的YGI数据
    std::shared_ptr<YGIData> ygiData = nullptr;
};
#endif //ANDROID_ROADDISEASE_H
