//
// Created by lpf on 2020/9/2.
//

#include "SoftDecoder.h"

#include <malloc.h>
#include <turbojpeg.h>
#include <sys/stat.h>
#include "Log.h"
#include <android/log.h>

/**
 *
 * @param filePath jpeg图片路径
 * @param outWidth 输出图片宽度
 * @param outHeight 输出图片高度
 * @param pixelFormat 输出的图片格式
 * @return 目标图像的buffer
 */
unsigned char *jpegDecodeWithPixelFormat(const char *filePath, int &outWidth, int &outHeight, int pixelFormat) {
    FILE *fd;
    struct stat fileinfo;
    stat(filePath, &fileinfo);
    unsigned long jpgSize;
    jpgSize = fileinfo.st_size;

    fd = fopen(filePath, "rb");
    if (nullptr == fd) {
//        CommonUtils::write_log(LOGGER_WARNING, "ImageRecognition", "file not open\n");
        return nullptr;
    }
    auto *jpg_buffer = (unsigned char *) malloc(sizeof(unsigned char) * fileinfo.st_size);
    fread(jpg_buffer, 1, fileinfo.st_size, fd);
    fclose(fd);

    tjhandle handle = nullptr;
    int img_width, img_height, img_subsamp, img_colorspace;
    int flags = 0;
    unsigned char *rgb_buffer = nullptr;
    unsigned char *value = nullptr;
    do {
        /*创建一个turbojpeg句柄*/
        handle = tjInitDecompress();
        if (nullptr == handle) {
            break;
        }
        /*获取jpg图片相关信息但并不解压缩*/
        int ret = tjDecompressHeader3(handle, jpg_buffer, jpgSize, &img_width, &img_height,
                                      &img_subsamp, &img_colorspace);

        if (0 != ret) {
            break;
        }
        outWidth = img_width;
        outHeight = img_height;
        /*解压缩时，tjDecompress2（）会自动根据设置的大小进行缩放，但是设置的大小要在它的支持范围，如1/2 1/4等*/
        flags |= 0;
        int size = outWidth * outHeight * 3;
        rgb_buffer = (unsigned char *) malloc(sizeof(unsigned char) * size);
        ret = tjDecompress2(handle, jpg_buffer, jpgSize, rgb_buffer, outWidth, 0, outHeight,
                            pixelFormat, flags);

        if (0 != ret) {
            break;
        }
        value = rgb_buffer;
    } while (false);

    if (handle) {
        tjDestroy(handle);
    }

    if (jpg_buffer) {
        free(jpg_buffer);
    }

    if (nullptr == value) {
        if (rgb_buffer) {
            free(rgb_buffer);
        }
    }
    return value;
}

/**
 * jpeg -> bgr
 */
unsigned char *SoftDecoder::jpegToBgr(const char *filePath, int &outWidth, int &outHeight) {

    return jpegDecodeWithPixelFormat(filePath, outWidth, outHeight, TJPF_BGR);
}

/**
 * jpeg -> rgb
 */
unsigned char *SoftDecoder::jpegToRgb(const char *filePath, int &outWidth, int &outHeight) {

    return jpegDecodeWithPixelFormat(filePath, outWidth, outHeight, TJPF_RGB);
}

/**
 * jpeg  -> Yuv
 */
int SoftDecoder::jpegToYuv(const char *filePath, unsigned char **yuvBuffer, int *yuvSize,
                           std::string *yuvType) {

    FILE *fd;
    struct stat fileInfo;
    stat(filePath, &fileInfo);
    unsigned long jpgSize;
    jpgSize = fileInfo.st_size;

    fd = fopen(filePath, "rb");
    if (nullptr == fd) {
        __android_log_print(ANDROID_LOG_INFO, "打开文件失败", "%s", filePath);
        return -1;
    }
    auto *jpg_buffer = (unsigned char *) malloc(sizeof(unsigned char) * fileInfo.st_size);
    fread(jpg_buffer, 1, fileInfo.st_size, fd);
    fclose(fd);

    tjhandle handle = nullptr;
    int width, height, subSample, colorSpace;
    int flags = 0;
    // 1或4均可，但不能是0
    int padding = 1;
    int ret = 0;

    do {
        // 创建一个turbojpeg句柄
        handle = tjInitDecompress();
        tjDecompressHeader3(handle, jpg_buffer, jpgSize, &width, &height, &subSample, &colorSpace);

        // 为啥要或0？
        // flags |= 0;

        // 注：经测试，指定的yuv采样格式只对YUV缓冲区大小有影响，实际上还是按JPEG本身的YUV格式来转换的
        *yuvSize = tjBufSizeYUV2(width, padding, height, subSample);
        *yuvBuffer = (unsigned char *) malloc(*yuvSize);
        if (*yuvBuffer == nullptr) {
            __android_log_print(ANDROID_LOG_INFO, "jpeg转yuv,获取yuvbuffer为空", "%s", __FILE__);
            break;
        }
        ret = tjDecompressToYUV2(handle, jpg_buffer, jpgSize, *yuvBuffer, width,
                                 padding, height, flags);
        if (ret < 0) {
            __android_log_print(ANDROID_LOG_INFO, "jpeg转yuv,解压缩出错", "%s", tjGetErrorStr());
            break;
        }

        switch (subSample) {
            case TJSAMP_444:
                *yuvType = ".yuv444";
                break;
            case TJSAMP_422:
                *yuvType = ".yuv422";
                break;
            case TJSAMP_420:
                *yuvType = ".yuv420";
                break;
            case TJSAMP_GRAY:
                *yuvType = ".yuv-gray";
                break;
            case TJSAMP_440:
                *yuvType = ".yuv440";
                break;
            case TJSAMP_411:
                *yuvType = ".yuv411";
                break;
            default:
                ret = -1;
        }

    } while (false);

    if (handle) {
        tjDestroy(handle);
    }

    if (jpg_buffer) {
        free(jpg_buffer);
    }

    return ret;
}

/**
 * jpeg  -> Yuv
 */
int SoftDecoder::jpegToYuv(unsigned char *jpg_buffer, int jpgSize, unsigned char **yuvBuffer, std::string *yuvType) {


    tjhandle handle = nullptr;
    int width, height, subSample, colorSpace;
    int flags = 0;
    // 1或4均可，但不能是0
    int padding = 1;
    int ret = 0;

    do {
        // 创建一个turbojpeg句柄
        handle = tjInitDecompress();
        tjDecompressHeader3(handle, jpg_buffer, jpgSize, &width, &height, &subSample, &colorSpace);

        // 为啥要或0？
        // flags |= 0;

        // 注：经测试，指定的yuv采样格式只对YUV缓冲区大小有影响，实际上还是按JPEG本身的YUV格式来转换的
        int yuvSize = tjBufSizeYUV2(width, padding, height, subSample);
        *yuvBuffer = (unsigned char *) malloc(yuvSize);
        if (*yuvBuffer == nullptr) {
            __android_log_print(ANDROID_LOG_INFO, "jpeg转yuv,获取yuvbuffer为空", "%s", __FILE__);
            break;
        }
        ret = tjDecompressToYUV2(handle, jpg_buffer, jpgSize, *yuvBuffer, width,
                                 padding, height, flags);
        if (ret < 0) {
            __android_log_print(ANDROID_LOG_INFO, "jpeg转yuv,解压缩出错", "%s", tjGetErrorStr());
            break;
        }

        switch (subSample) {
            case TJSAMP_444:
                *yuvType = ".yuv444";
                break;
            case TJSAMP_422:
                *yuvType = ".yuv422";
                break;
            case TJSAMP_420:
                *yuvType = ".yuv420";
                break;
            case TJSAMP_GRAY:
                *yuvType = ".yuv-gray";
                break;
            case TJSAMP_440:
                *yuvType = ".yuv440";
                break;
            case TJSAMP_411:
                *yuvType = ".yuv411";
                break;
            default:
                ret = -1;
        }

    } while (false);

    if (handle) {
        tjDestroy(handle);
    }
    return ret;
}
