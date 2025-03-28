//
// Created by chuangbo.cheng on 2020/8/3.
//

#ifndef ANDROID_SMARTFRAME_H
#define ANDROID_SMARTFRAME_H

#include "DataTypeDefine.h"
#include "TSRResult.h"
#include <vector>
#include <string>
#include "EnumImage.h"

class SmartFrame{
public:
    SmartFrame(){
        this->tsrResultVector = nullptr;
    }
    ~SmartFrame(){
        // 释放资源
        if (tsrResultVector != nullptr){
            for (auto &item : *tsrResultVector){
                delete item;
                item = nullptr;
            }
            delete tsrResultVector;
            tsrResultVector = nullptr;
        }
        if (compressedData != nullptr){
            delete[] compressedData;
            compressedData = nullptr;
        }
        if (frameScaleData != nullptr){
            delete[] frameScaleData;
            frameScaleData = nullptr;
        }
    }
public:
    int getWidth() const {
        return width;
    }

    void setWidth(int width) {
        this->width = width;
    }

    const std::string &getFilePath() const {
        return filePath;
    }

    void setFilePath(const std::string &filePath) {
        this->filePath = filePath;
    }

    u8 getTimestamp() const {
        return timestamp;
    }

    void setTimestamp(u8 timestamp) {
        this->timestamp = timestamp;
    }

    int getHeight() const {
        return height;
    }

    void setHeight(int height) {
        this->height = height;
    }

    int getLength() const {
        return length;
    }

    void setLength(int length) {
        this->length = length;
    }

    EnumImage getType() const {
        return type;
    }

    void setType(EnumImage type) {
        this->type = type;
    }

    BYTE *getFrameData() const {
        return frameData;
    }

    void setFrameData(BYTE *frameData) {
        this->frameData = frameData;
    }

    std::vector<TSRResult *> *getTsrResultVector() const {
        return tsrResultVector;
    }

    void setTsrResultVector(std::vector<TSRResult *> *tsrResultVector) {
        this->tsrResultVector = tsrResultVector;
    }

    int getCompressedWidth() const {
        return compressedWidth;
    }

    void setCompressedWidth(int compressedWidth) {
        this->compressedWidth = compressedWidth;
    }

    int getCompressedHeight() const {
        return compressedHeight;
    }

    void setCompressedHeight(int compressedHeight) {
        this->compressedHeight = compressedHeight;
    }

    int getCompressedLength() const {
        return compressedLength;
    }

    void setCompressedLength(int compressedLength) {
        this->compressedLength = compressedLength;
    }

    EnumImage getCompressedType() const {
        return compressedType;
    }

    void setCompressedType(EnumImage compressedType) {
        this->compressedType = compressedType;
    }

    BYTE *getCompressedData() const {
        return compressedData;
    }

    void setCompressedData(BYTE *compressedData) {
        this->compressedData = compressedData;
    }
// 私有的
public:
    std::string filePath = "";
    /**
     * 时间 精确到毫秒
     */
    u8 timestamp = 0;
    /**
     * 宽
     */
    int width = 0;
    /**
     * 高
     */
    int height = 0;
    /**
     * 文件长度
     */
    int length = 0;
    /**
     * 缩略图
     */
    int scaleLength = 0;
    /**
     * 格式类型yuv420
     */
    EnumImage type = EnumImage::YUV420;
    /**
     * 压缩后的宽度
     */
    int compressedWidth = 0;
    /**
     * 压缩后的高度
     */
    int compressedHeight = 0;
    /**
     * 压缩后的长度
     */
    int compressedLength = 0;
    /**
     * 压缩的数据类型 Y为灰度图
     */
    EnumImage compressedType = EnumImage ::Y;
    /**
     * 数据指针
     */
    BYTE *frameData = nullptr;
    /**
     * 压缩后的灰度图
     */
    BYTE *compressedData = nullptr;
    /**
     * 预览缩略图
     */
    BYTE *frameScaleData = nullptr;
    /**
     * 识别的结果
     */
    std::vector<TSRResult*>  *tsrResultVector;
    /**
     * us 曝光时间
     */
    double integration = 0;
    /**
     * us 每行曝光时间
     */
    double lineOfIntegration = 0;

    // 预留的字段
    std::string moreField = "";

};

typedef std::vector<std::shared_ptr<SmartFrame>> FrameVector;


#endif //ANDROID_SMARTFRAME_H
