//
// Created by lpf on 2020/9/3.
//

#include "JpgUtil.h"
#include "SoftEncoder.h"
#include "SoftDecoder.h"
#include "HardEncoder.h"
#include "HardDecoder.h"
//#include "Export_Inc/Log/Log.h"


JpgUtil::JpgUtil(JpgCodecType jpgCodecType) {
    setStrategy(jpgCodecType);
}


/**
 * 设置策略（软硬编解码）
 */
void JpgUtil::setStrategy(JpgCodecType jpgCodecType) {

    if (this->iEncoder) {
        delete this->iEncoder;
    }

    if (this->iDecoder) {
        delete (this->iDecoder);
    }

    switch (jpgCodecType) {
        case JpgCodecType::CodecTypeSoft:
            this->iEncoder = new SoftEncoder();
            this->iDecoder = new SoftDecoder();
            break;
        case JpgCodecType::CodecTypeHard:
            this->iEncoder = new HardEncoder();
            this->iDecoder = new HardDecoder();
            break;
    }
}

// ****************************** jpg 编码  ******************************

/**
 * 软编码 yuv -> jpeg
 */
void JpgUtil::doSoftEncodeYuvToJpeg(unsigned char *yuvBuffer, int yuvSize, int type, int width,
                                    int height, int padding, int quality, unsigned char **jpgBuffer,
                                    int &jpgSize) {
#ifdef MOCK_YGI
    // 这样临时生成下 给测试同学使用 实际上不是真实的jpg
//    jpgSize = 300 * 1024;
//    *jpgBuffer = new unsigned char[jpgSize];
//
//    return;
#endif

    this->iEncoder->yuvToJpeg(
            yuvBuffer, yuvSize, type, width, height, padding, quality, jpgBuffer, jpgSize);
}

/**
 * 硬编码 yuv -> jpeg
 */
void JpgUtil::doHardEncodeYuvToJpeg(unsigned char *yuvBuffer, int width, int height,
                                    int quality, uint8_t *jpgBuffer, int *jpgSize) {

    this->iEncoder->yuvToJpeg(
            yuvBuffer, width, height, quality, jpgBuffer, jpgSize);
}



// ****************************** jpg 解码  ******************************

/**
 * jpg 解码 -> Bgr
 */
void JpgUtil::doDecodeJpegToBgr(const char *filePath, int &outWidth, int &outHeight) {
    this->iDecoder->jpegToBgr(filePath, outWidth, outHeight);
}

/**
 * jpg 解码 -> Rgb
 */
void JpgUtil::doDecodeJpegToRgb(const char *filePath, int &outWidth, int &outHeight) {
    this->iDecoder->jpegToRgb(filePath, outWidth, outHeight);
}

/**
 * jpg 解码 -> Yuv
 * @param filePath jpg图片路径
 * @param yuvBuffer 目标文件buffer
 * @param yuvSize yuv图像的大小
 * @param yuvType yuv图像的类型
 * @return -1:失败
 */
int JpgUtil::doDecodeJpegToYuv(const char *filePath, unsigned char **yuvBuffer, int *yuvSize,
                               std::string *yuvType) {
    return this->iDecoder->jpegToYuv(filePath, yuvBuffer, yuvSize, yuvType);
}
/**
 * jpg 解码 -> Yuv
 * @param filePath jpg图片路径
 * @param yuvBuffer 目标文件buffer
 * @param yuvSize yuv图像的大小
 * @param yuvType yuv图像的类型
 * @return -1:失败
 */
int JpgUtil::doDecodeJpegToYuv(unsigned char *jpgBuffer, int jpgSize, unsigned char **yuvBuffer, std::string *yuvType) {
    return this->iDecoder->jpegToYuv(jpgBuffer, jpgSize, yuvBuffer, yuvType);
}

