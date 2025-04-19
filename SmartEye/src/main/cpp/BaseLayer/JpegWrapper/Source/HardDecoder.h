//
// Created by lpf on 2020/9/2.
//

#ifndef ANDROID_HARDDECODER_H
#define ANDROID_HARDDECODER_H


#include "../Export_Inc/IDecoder.h"

class HardDecoder : public IDecoder {

public:

    HardDecoder() {};

    ~HardDecoder() {};

    unsigned char *jpegToBgr(const char *filepath, int &outWidth, int &outHeight) override;

    unsigned char *jpegToRgb(const char *filepath, int &outWidth, int &outHeight) override;

    /**
    * yuv -> jpeg
    */
    virtual int jpegToYuv(const char *filePath, unsigned char **yuvBuffer, int *yuvSize,
                          std::string *yuvType) override;

    virtual int jpegToYuv(unsigned char *jpgBuffer, int jpgSize, unsigned char **yuvBuffer, std::string *yuvType) override;
};


#endif //ANDROID_HARDDECODER_H
