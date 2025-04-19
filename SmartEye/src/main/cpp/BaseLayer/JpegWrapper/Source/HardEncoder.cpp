//
// Created by lpf on 2020/9/2.
//

#include <android/log.h>
//#include <Time/TimeUtils.h>
#include "HardEncoder.h"
#include "jpegYuv420.h"
#include "Log.h"

/**
 * 硬件编码 yuv -> jpeg
 * @param yuvBuffer
 * @param width
 * @param height
 * @param quality
 * @param jpgBuffer
 * @param jpgSize
 * @return
 */
int HardEncoder::yuvToJpeg(unsigned char *yuvBuffer,
                           int width, int height, int quality,
                           uint8_t *jpgBuffer, int *jpgSize) {

    Log::info("HardEncoder", "开始硬编yuv到jpeg");

    #ifdef DHARD_ENCODE
        u8 begin = TimeUtils::currentTimeMillis();
        int size = JpegHwEngine_yuv420_compress(width, height, yuvBuffer, quality, jpgBuffer, jpgSize);
        u8 end = TimeUtils::currentTimeMillis();

        Log::info("HardEncoder", "结束硬编yuv到jpeg，得到的size:%d, 耗时：%lld毫秒", size, end-begin);

        if (size > 0) {
            return 0;
        } else {
            return -1;
        }
    #endif


}

