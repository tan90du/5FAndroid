//
// Created by lpf on 2020/9/2.
//

#ifndef ANDROID_SOFTENCODER_H
#define ANDROID_SOFTENCODER_H


#include "turbojpeg.h"
#include "IEncoder.h"

class SoftEncoder : public IEncoder {

public:
    SoftEncoder();

    ~SoftEncoder();

    /**
     * yuv -> jpg
     */
    virtual int yuvToJpeg(
            unsigned char  *yuv_buffer,
            int yuv_size,
            int type,
            int width,
            int height,
            int padding,
            int quality,
            unsigned char  **jpg_buffer,
            int &jpg_size) override ;

private:

    /*
    * YUV420P：YV12 -> YU12，because libjpeg only surport YU12
    */
    bool yv12ToYu12(BYTE* yuv_buffer, int yuv_size, int width, int height, int padding);

    /*
     * NV12 -> YU12
     */
    bool nv12ToYu12(BYTE* yuv_buffer, int yuv_size, int width, int height, int padding);

    /**
     * NV21 -> YU12
     */
    int nv21ToYu12(BYTE *in, int width, int height);

    /**
     * YUYV -> YUV422P
     */
    int yuyvToYuv422P(BYTE *in, int width, int height);

    /**
     * YU12 -> Jpeg
     * @param yuv_buffer：yuv数据区
     * @param yuv_size：yuv数据大小
     * @param width：yuv宽度
     * @param height：yuv高度
     * @param quality：jpg压缩质量
     * @out jpg_buffer:输出的jpg数据
     * @out jpg_size :输出的jpg大小
     * @return :0为成功，其他则失败
     */
    int yu12ToJpeg(BYTE* yuv_buffer, int yuv_size, int width, int height, int padding, int quality,
                   BYTE** jpg_buffer, int& jpg_size, TJSAMP = TJSAMP_420);


};


#endif //ANDROID_SOFTENCODER_H
