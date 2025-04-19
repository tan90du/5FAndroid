//
// Created by lpf on 2020/9/2.
//

#ifndef ANDROID_HARDENCODER_H
#define ANDROID_HARDENCODER_H


#include "IEncoder.h"
#include "DataTypeDefine.h"

class HardEncoder : public IEncoder {

public:

    HardEncoder() {};

    ~HardEncoder() {};

    int yuvToJpeg(
            unsigned char *yuvBuffer,
            int width,
            int height,
            int quality,
            uint8_t *jpgBuffer,
            int *jpgSize) override;
};


#endif //ANDROID_HARDENCODER_H
