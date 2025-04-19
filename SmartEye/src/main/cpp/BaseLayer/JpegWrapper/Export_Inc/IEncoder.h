//
// Created by lpf on 2020/9/1.
//

#ifndef ANDROID_IENCODER_H
#define ANDROID_IENCODER_H

#include "string"
#include "DataTypeDefine.h"

/**
 * 图片编码的基类
 */
class IEncoder {

public:


    virtual ~IEncoder() {

    };

    /**
     * 软编码: Yuv -> Jpeg
     * @param yuvBuffer 指向原始yuv图像buffer的指针
     * @param yuvSize yuv图像的大小
     * @param type yuv的类型
     * @param width 原始图片的宽度
     * @param height 原始图片的高度
     * @param padding 源图像中使用的行填充。例如，如果YUV图像的每个平面中的每一行都被填充为4字节的最近倍数，padding为4
     * @param quality jpeg图像的质量，1是最差，100是最好
     * @param jpgBuffer 输出的jpg数据
     * @param jpgSize 输出的jpg的大小
     * @return 0：成功 -1：失败 (失败可查看#tjGetErrorStr2() and #tjGetErrorCode())
     */
    virtual int yuvToJpeg(
            unsigned char *yuvBuffer,
            int yuvSize,
            int type,
            int width,
            int height,
            int padding,
            int quality,
            unsigned char **jpgBuffer,
            int &jpgSize) {};

    /**
     * 硬编码：yuv ->jpeg
     */
    virtual int yuvToJpeg(
            unsigned char *yuvBuffer,
            int width,
            int height,
            int quality,
            uint8_t *jpgBuffer,
            int *jpgSize) {};

};

#endif //ANDROID_IENCODER_H
