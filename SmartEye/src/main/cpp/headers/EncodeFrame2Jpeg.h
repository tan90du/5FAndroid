//
// Created by chuangbo.cheng on 2020/9/7.
//

#ifndef ANDROID_ENCODEFRAME2JPEG_H
#define ANDROID_ENCODEFRAME2JPEG_H

#include "YGIData.h"
#include "JpgUtil.h"
#include <functional>

typedef std::function<void(int, std::shared_ptr<YGIData>)> OnEncodedCallback;
class EncodeFrame2Jpeg {
public:
    EncodeFrame2Jpeg();

    int encodeFrame2Jpeg(std::shared_ptr<YGIData> ygiData, int jpegQuality);

    int doDecodeJpegToYuv(unsigned char *jpgBuffer, int jpgSize, unsigned char **yuvBuffer, std::string *yuvType);

    JpgUtil *getJpgUtil() const;

//    Handler* getHandler();

private:
    //
private:
    // 默认值0
//    int frameType = 0;
//    HandlerThread *mJpegEncodeThread = nullptr;
//    Handler* handler = nullptr;

    JpgUtil *jpgUtil = nullptr;
};


#endif //ANDROID_ENCODEFRAME2JPEG_H
