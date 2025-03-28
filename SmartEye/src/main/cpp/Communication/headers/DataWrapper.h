//
// Created by 郑伟 on 2021/5/14.
//

#ifndef ANDROID_DATAWRAPPER_H
#define ANDROID_DATAWRAPPER_H


#include <cstdint>
#include <memory>
#include <BaseTXData.h>
#include <YGIData.h>


/**
 * TCP通信的数据封装处理，协议格式如下
 * 数据类型：      4字节，int32
 *                 1为图片
 *                 2为升级包(zip)
 *                 3为模型文件（zip）
 *                 4为字符串
 * 数据：          变长，存储真正的数据
 */
class DataWrapper {
public:

    // 发送识别病害需要的数据
    static void wrapYGIData(std::shared_ptr<YGIData> ygiData, int32_t& output_length, unsigned char** out_data);

    // 解析出TX2发送过来的数据
    static std::shared_ptr<BaseTXData> parse(const char* data, int32_t length);
};


#endif //ANDROID_DATAWRAPPER_H
