//
// Created by lpf on 2020/9/2.
//

#ifndef ANDROID_IDECODER_H
#define ANDROID_IDECODER_H

#include <string>

class IDecoder {


public:

    virtual ~IDecoder() {

    };

    /**
     * jpg -> bgr
     * @param filePath 图片路径
     * @param outWidth 输出图片宽度
     * @param outHeight 输出图片高度
     * @return 目标图像的buffer
     */
    virtual unsigned char *jpegToBgr(const char *filePath, int &outWidth, int &outHeight) = 0;

    /**
    * jpg -> rgb
    * @param filePath 图片路径
    * @param outWidth 输出图片宽度
    * @param outHeight 输出图片高度
    * @return 目标图像的buffer
    */
    virtual unsigned char *jpegToRgb(const char *filePath, int &outWidth, int &outHeight) = 0;


    /**
    * jpg 解码 -> Yuv
    * @param filePath jpg图片路径
    * @param yuvBuffer 目标文件buffer
    * @param yuvSize yuv图像的大小
    * @param yuvType yuv图像的类型
    * @return -1:失败
    */
    virtual int jpegToYuv(const char *filePath, unsigned char **yuvBuffer, int *yuvSize,
                          std::string *yuvType) = 0;
   /**
    * jpg 解码 -> Yuv
    * @param jpgBuffer
    * @param jpgSize
    * @param yuvBuffer
    * @param yuvType
    * @return
    */
    virtual int jpegToYuv(unsigned char *jpgBuffer, int jpgSize, unsigned char **yuvBuffer, std::string *yuvType) = 0;
};

#endif //ANDROID_IDECODER_H
