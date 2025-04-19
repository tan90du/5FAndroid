//
// Created by lpf on 2020/9/2.
//

#ifndef ANDROID_SOFTDECODER_H
#define ANDROID_SOFTDECODER_H


#include "../Export_Inc/IDecoder.h"

class SoftDecoder : public IDecoder {

public:

    unsigned char *jpegToBgr(const char *filePath, int &outWidth, int &outHeight) override;

    unsigned char *jpegToRgb(const char *filePath, int &outWidth, int &outHeight) override;

    int jpegToYuv(const char *filePath, unsigned char **yuvBuffer, int *yuvSize,
                  std::string *yuvType) override;

    int jpegToYuv(unsigned char *jpgBuffer, int jpgSize, unsigned char **yuvBuffer, std::string *yuvType) override;
};


#endif //ANDROID_SOFTDECODER_H
