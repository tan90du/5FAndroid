//
// Created by lpf on 2020/9/3.
//

#ifndef ANDROID_JPGUTIL_H
#define ANDROID_JPGUTIL_H


#include "CommonMacro.h"
#include "IEncoder.h"
#include "IDecoder.h"

/**
 * 图片编解码类型
 */
enum class JpgCodecType {

    /**
     * 软编解码
     */
    CodecTypeSoft,

    /**
     * 硬编解码
     */
    CodecTypeHard

};

/**
 * Jpeg编解码
 */
class JpgUtil {

public:

    JpgUtil(JpgCodecType type);

    ~JpgUtil() {
        delete iEncoder;
        delete iDecoder;
    }


    /**
     * 设置策略（硬编解、软编解）
     */
    void setStrategy(JpgCodecType jpgCodecType);

public :

    // ************************  jpg 编码 **********************

    /**
     * 软编码 yuv -> jpeg
     */
    void doSoftEncodeYuvToJpeg(unsigned char *yuvBuffer,
                               int yuvSize,
                               int type,
                               int width,
                               int height,
                               int padding,
                               int quality,
                               unsigned char **jpgBuffer,
                               int &jpgSize);

    /**
     * 硬编码 yuv -> jpeg
     */
    void doHardEncodeYuvToJpeg(unsigned char *yuvBuffer,
                               int width,
                               int height,
                               int quality,
                               uint8_t *jpgBuffer,
                               int *jpgSize);

public:

    // ************************  jpg 解码 **********************


    /**
    * jpg -> bgr
    * @param filePath 图片路径
    * @param outWidth 输出图片宽度
    * @param outHeight 输出图片高度
    * @return 目标图像的buffer
    */
    void doDecodeJpegToBgr(const char *filePath, int &outWidth, int &outHeight);


    /**
    * jpg -> rgb
    * @param filePath 图片路径
    * @param outWidth 输出图片宽度
    * @param outHeight 输出图片高度
    * @return 目标图像的buffer
    */
    void doDecodeJpegToRgb(const char *filePath, int &outWidth, int &outHeight);

    /**
    * jpg 解码 -> Yuv
    * @param filePath jpg图片路径
    * @param yuvBuffer 目标文件buffer
    * @param yuvSize yuv图像的大小
    * @param yuvType yuv图像的类型
    * @return -1:失败
    */
    int doDecodeJpegToYuv(const char *filePath, unsigned char **yuvBuffer, int *yuvSize,
                          std::string *yuvType);

    /**
     * jpg 解码 -> Yuv
     * @param jpgBuffer
     * @param jpgSize
     * @param yuvBuffer
     * @param yuvType
     * @return
     */
    int doDecodeJpegToYuv(unsigned char *jpgBuffer, int jpgSize, unsigned char **yuvBuffer, std::string *yuvType);


private:

    // 编码基类
    IEncoder *iEncoder = nullptr;

    // 解码基类
    IDecoder *iDecoder = nullptr;

};


#endif //ANDROID_JPGUTIL_H
