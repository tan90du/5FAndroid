//
// Created by chuangbo.cheng on 2020/9/7.
//

//#include <Time/TimeUtils.h>
#include "EncodeFrame2Jpeg.h"
//#include <unistd.h>
#include "Log.h"

EncodeFrame2Jpeg::EncodeFrame2Jpeg() {
//    mJpegEncodeThread = HandlerThread::newThread("EncodeFrame2Jpeg");
//    mJpegEncodeThread->start();
    jpgUtil = new JpgUtil(JpgCodecType::CodecTypeSoft);
}
// 运行在数据接收线程 在添加到缓存后开启
int EncodeFrame2Jpeg::encodeFrame2Jpeg(std::shared_ptr<YGIData> ygiData, int jpegQuality) {
    // 值传递
//    if (handler == nullptr) {
//        handler = mJpegEncodeThread->getHandler();
//    }
//    handler->post([=] {
        auto frame = ygiData->getSmartFrame();
//        if (frame->getType() == EnumImage::JPEG){
//            onEncodedCallback(0, ygiData);
//            return;
//        }
//        u8 t1 = TimeUtils::currentTimeMicrosecond();
        BYTE *jpgBuffer = nullptr;
        int jpgSize = 0;


        // 模拟一张相机图片数据
//        int yuvSize = 1920 * 1080 * 3 / 2;
//        unsigned char *yuvBuffer = new unsigned char[yuvSize];
//        FILE *fp_in = fopen(
//                "/sdcard/0test/111.yuv",
//                "rb+");
//        if (fp_in != NULL) {
//            fread(yuvBuffer, yuvSize, 1, fp_in);
//        } else {
////            Log::info("doJpegSoftEncodeTest", "找不到yuv文件");
//            return;
//        }
//        fclose(fp_in);


        getJpgUtil()->doSoftEncodeYuvToJpeg(frame->getFrameData(), frame->getLength(), 0, frame->getWidth(), frame->getHeight(), 4, jpegQuality, &jpgBuffer, jpgSize);
        if (jpgBuffer != nullptr) {
            frame->setFrameData(jpgBuffer);
            frame->setType(EnumImage::JPEG);
            frame->setLength(jpgSize);
        }
        if (frame->frameScaleData == nullptr) {
//            u8 t2 = TimeUtils::currentTimeMicrosecond();
//            // 大于30毫秒就输出日志
//            if (t2 - t1 > 30000) {
//                Log::info("EncodeFrame2Jpeg", "frame->getTimestamp() = %lld, encode 耗时 = %d", frame->getTimestamp(), t2 - t1);
//            }
//            onEncodedCallback(0, ygiData);
            return 0;
        }
        BYTE *jpgScaleBuffer = nullptr;
        int jpgScaleSize = 0;
        getJpgUtil()->doSoftEncodeYuvToJpeg(frame->frameScaleData, frame->scaleLength, 0, 640, 360,
                                            4, jpegQuality, &jpgScaleBuffer, jpgScaleSize);
        if (jpgScaleBuffer != nullptr) {
            frame->frameScaleData = jpgScaleBuffer;
            frame->scaleLength = jpgScaleSize;
        }
//        u8 t2 = TimeUtils::currentTimeMicrosecond();
//        // 大于30毫秒就输出日志
//        if (t2 - t1 > 30000) {
//            Log::info("EncodeFrame2Jpeg", "frame->getTimestamp() = %lld, encode耗时 = %d", frame->getTimestamp(), t2 - t1);
//        }
//        onEncodedCallback(0, ygiData);
//    });
    return 0;
}
//Handler* EncodeFrame2Jpeg::getHandler(){
//    return handler;
//}
int EncodeFrame2Jpeg::doDecodeJpegToYuv(unsigned char *jpgBuffer, int jpgSize, unsigned char **yuvBuffer, std::string *yuvType){
    return getJpgUtil()->doDecodeJpegToYuv(jpgBuffer, jpgSize, yuvBuffer, yuvType);
}

JpgUtil *EncodeFrame2Jpeg::getJpgUtil() const {
    return jpgUtil;
}
