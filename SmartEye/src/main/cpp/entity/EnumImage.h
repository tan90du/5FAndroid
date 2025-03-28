//
// Created by chuangbo.cheng on 2020/8/31.
//

#ifndef ANDROID_IMAGEEMNU_H
#define ANDROID_IMAGEEMNU_H
// 可以随时新增格式
enum class EnumImage{
    Y = 0, // 只是灰度
    YUV420,
    YUV420p,
    NV12,
    NV21,
    RGB24,
    RGBA,
    JPEG,
    PNG,
    WEBP
};
#endif //ANDROID_IMAGEEMNU_H
