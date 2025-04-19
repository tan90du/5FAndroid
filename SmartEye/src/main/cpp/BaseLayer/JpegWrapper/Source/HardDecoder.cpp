//
// Created by lpf on 2020/9/2.
//

#include "HardDecoder.h"


unsigned char *HardDecoder::jpegToBgr(const char *filepath, int &outwidth, int &outheight) {
    return nullptr;
}

unsigned char *HardDecoder::jpegToRgb(const char *filepath, int &outwidth, int &outheight) {
    return nullptr;
}

int HardDecoder::jpegToYuv(const char *filePath, unsigned char** yuv_buffer, int* yuvSize, std::string* yuvType ) {
    return -1;
}

int HardDecoder::jpegToYuv(unsigned char *jpgBuffer, int jpgSize, unsigned char **yuvBuffer, std::string *yuvType) {
    return -1;
}