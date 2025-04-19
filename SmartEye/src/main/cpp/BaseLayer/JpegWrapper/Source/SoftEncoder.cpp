//
// Created by lpf on 2020/9/2.
//

#include <android/log.h>
//#include <Time/TimeUtils.h>
#include "SoftEncoder.h"
//#include "Export_Inc/Log/Log.h"

SoftEncoder::SoftEncoder() {

};
SoftEncoder::~SoftEncoder() {

};


bool checkJPEG(const unsigned char *pStr, const uint32_t len) {
    bool rtv = true;
    const unsigned char &ch1 = pStr[0];
    const unsigned char &ch2 = pStr[1];
    const unsigned char &ch3 = pStr[len - 2];
    const unsigned char &ch4 = pStr[len - 1];

    if (0
        || ch1 != 255    //(0xff)
        || ch2 != 216    //(0xd8)
//		||	ch3 != 255	//(0xff)
//		||	ch4 != 217	//(0xd9)
            ) {
        return false;
    }

    return rtv;
}

int SoftEncoder::yuvToJpeg(unsigned char *yuvBuffer, int yuvSize, int type, int width, int height,
                           int padding,
                           int quality, unsigned char **jpgBuffer, int &jpgSize) {

    switch (type) {
        case 0: {
            return yu12ToJpeg(yuvBuffer, yuvSize, width, height, padding, quality, jpgBuffer,
                              jpgSize);
        }
        case 1: {
            if (false == yv12ToYu12(yuvBuffer, yuvSize, width, height, padding)) {
                return -1;
            }
            return yu12ToJpeg(yuvBuffer, yuvSize, width, height, padding, quality, jpgBuffer,
                              jpgSize);
        }
        case 2: {
            if (false == nv12ToYu12(yuvBuffer, yuvSize, width, height, padding)) {
                return -1;
            }
            return yu12ToJpeg(yuvBuffer, yuvSize, width, height, padding, quality, jpgBuffer,
                              jpgSize);
        }
        case 3: {
            if (0 != nv21ToYu12(yuvBuffer, width, height)) {
                return -1;
            }
            int result = yu12ToJpeg(yuvBuffer, yuvSize, width, height, padding, quality,
                                    jpgBuffer,
                                    jpgSize);
            return result;
        }
        case 4: {
            if (0 != yuyvToYuv422P(yuvBuffer, width, height)) {
                return -1;
            }
            int result = yu12ToJpeg(yuvBuffer, yuvSize, width, height, padding, quality,
                                    jpgBuffer,
                                    jpgSize, TJSAMP_422);
            return result;
        }
        case 7: { //jpeg类型不需要压缩，但是需要做完整性校验
            if (checkJPEG(yuvBuffer, yuvSize)) {
                *jpgBuffer = yuvBuffer;
                jpgSize = yuvSize;
            } else {
//                CommonUtils::write_log(LOGGER_WARNING, "yuv2jpeg", "jpeg格式不正确，format = %d", type);
            }
            return 0;
        }
        default: {
            //todo 其他格式YUV
//            CommonUtils::write_log(LOGGER_WARNING, "yuv2jpeg", "其他yuv格式暂不支持，format = %d", type);
            return -1;
        }
    }
}


/*
 * YUV420P：YV12 convert to YU12，because libjpeg only surport YU12
 */
bool SoftEncoder::yv12ToYu12(BYTE *yuvBuffer, int yuvSize, int width, int height, int padding) {
    int uvLength = (yuvSize - width * height) / 2;
    BYTE *tempCache = new BYTE[uvLength];

    memcpy(tempCache, yuvBuffer + width * height, uvLength);
    memcpy(yuvBuffer + width * height, yuvBuffer + width * height + uvLength, uvLength);
    memcpy(yuvBuffer + width * height + uvLength, tempCache, uvLength);
    delete[] tempCache;
    tempCache = nullptr;
    return true;
}

/*
 * NV12 convert to YU12，because libjpeg only surport YU12
 */
bool SoftEncoder::nv12ToYu12(BYTE *yuvBuffer, int yuvSize, int width, int height, int padding) {
    // 将yyyyyyuvuvuv 分离为 yyyuuuvvv
    // u 、v各占 1/6
    // y 独占 2/3
    // 创建存放UV的缓存
    int size = width * height * 3 / 2;
    BYTE *tempVBuffer = new BYTE[size / 6];
    // 第一个V的位置
    int uvIndex = 0;
    int vIndex = 0;
    int yEndPos = size * 2 / 3 + 1;
    for (int index = yEndPos; index < size; ++index) {
        // 偶数为 u
        if (index % 2 == 0) {
            // 当前的U 要替换  的位置
            int newUPos = yEndPos + uvIndex / 2;
            yuvBuffer[newUPos] = yuvBuffer[index];
        } else {
            *(tempVBuffer + vIndex) = yuvBuffer[index];
            vIndex++;
        }
        uvIndex++;
    }
    // 将v拼接到后面
    memcpy(yuvBuffer + size * 5 / 6, tempVBuffer, size / 6);
    delete[](tempVBuffer);
    return true;
}


/**
 * @param in  原始420 yuv数据
 * @param width
 * @param height
 * @return
 */
int SoftEncoder::nv21ToYu12(BYTE *in, int width, int height) {
    // 将yyyyyyvuvuvuvuvu 分离为 yyyuuuvvv
    // u 、v各占 1/6
    // y 独占 2/3
    // 创建存放UV的缓存
    int size = width * height * 3 / 2;
    BYTE *tempVBuffer = new BYTE[size / 6];
    // 第一个V的位置
    int uvIndex = 0;
    int vIndex = 0;
    int yEndPos = size * 2 / 3 + 1;
    for (int index = yEndPos; index < size; ++index) {
        // 偶数为 v
        if (index % 2 != 0) {
            // 当前的U 要替换  的位置
            int newUPos = yEndPos + uvIndex / 2;
            in[newUPos] = in[index];
        } else {
            *(tempVBuffer + vIndex) = in[index];
            vIndex++;
        }
        uvIndex++;
    }
    // 将v拼接到后面
    memcpy(in + size * 5 / 6, tempVBuffer, size / 6);
    delete[](tempVBuffer);
    return 0;
}

/**
 * YU12 -> Jpeg
 */
int SoftEncoder::yu12ToJpeg(BYTE *yuvBuffer, int yuvSize, int width, int height, int padding,
                            int quality,
                            BYTE **jpgBuffer, int &jpgSize, TJSAMP TJSAMP_TYPE) {

    int subsample = TJSAMP_TYPE;

    tjhandle handle = NULL;
    int flags = 0;
    int need_size = 0;
    int ret = 0;

    handle = tjInitCompress();

    flags |= 0;

    need_size = tjBufSizeYUV2(width, padding, height, subsample);
    if (need_size != yuvSize) {
        return -1;
    }
    unsigned long retSize = 0;
    ret = tjCompressFromYUV(handle, yuvBuffer, width, padding, height, subsample,
                            jpgBuffer, &retSize, quality, flags);
    jpgSize = retSize;
    if (ret < 0) {
//        Log::info("压缩jpeg失败，错误信息:%s", tjGetErrorStr());
    }
    tjDestroy(handle);

    return ret;
}

/**
 *
 * @param in
 * @param width
 * @param height
 * @return
 */
int SoftEncoder::yuyvToYuv422P(BYTE *in, int width, int height) {
    // 将yuyv 分离为 yyyuuuvvv
    // 创建存放UV的缓存
    int size = width * height * 2;
    int halfOfSize = size / 2;
    BYTE *tempUVBuffer = new BYTE[halfOfSize];
    // 第一个U的位置
    BYTE *UBuffer = tempUVBuffer;
    // 第一个V的位置
    BYTE *VBuffer = tempUVBuffer + halfOfSize / 2;
    bool isU = true;
    int uIndex = 0;
    int vIndex = 0;
    for (int index = 0; index < size; ++index) {
        // 偶数为 Y
        if (index % 2 == 0) {
            // 当前的Y 要替换 index/2 的位置
            in[index / 2] = in[index];
        } else {
            // U、V交替
            if (isU) {
                isU = false;
                *(UBuffer + uIndex) = in[index];
                uIndex++;
            } else {
                *(VBuffer + vIndex) = in[index];
                isU = true;
                vIndex++;
            }
        }
    }
    // 将uv拼接到后面
    memcpy(in + halfOfSize, tempUVBuffer, halfOfSize);
    delete[](tempUVBuffer);
    return 0;
}
