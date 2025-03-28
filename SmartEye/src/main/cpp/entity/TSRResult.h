//
// Created by chuangbo.cheng on 2020/8/10.
//

#ifndef ANDROID_TSRRESULT_H
#define ANDROID_TSRRESULT_H
#include "DataTypeDefine.h"

class TSRResult {
public:
    TSRResult(){
        id = 0;
        code = 0;
        confidence = 0;
        leftUpX = 0;
        leftUpY = 0;
        rightDownX = 0;
        rightDownY = 0;
    }
    u8 getId() const {
        return id;
    }

    void setId(u8 id) {
        this->id = id;
    }

    int getCode() const {
        return code;
    }

    void setCode(int code) {
        this->code = code;
    }

    float getConfidence() const {
        return confidence;
    }

    void setConfidence(float confidence) {
        this->confidence = confidence;
    }

    int getLeftUpX() const {
        return leftUpX;
    }

    void setLeftUpX(int leftUpX) {
        this->leftUpX = leftUpX;
    }

    int getLeftUpY() const {
        return leftUpY;
    }

    void setLeftUpY(int leftUpY) {
        this->leftUpY = leftUpY;
    }

    int getRightDownX() const {
        return rightDownX;
    }

    void setRightDownX(int rightDownX) {
        this->rightDownX = rightDownX;
    }

    int getRightDownY() const {
        return rightDownY;
    }

    void setRightDownY(int rightDownY) {
        this->rightDownY = rightDownY;
    }
private:
    /**
     * 单例id 现实世界中同一个物体的id相同
     */
    u8 id;
    /**
     * 物体的类别编码
     */
    int code;
    /**
     * 置信度
     */
    float confidence;
    /**
     * 物体在图像中的像素左上角x坐标
     */
    int leftUpX;
    /**
     * 物体在图像中的像素左上角y坐标
     */
    int leftUpY;
    /**
     * 物体在图像中的像素右下角x坐标
     */
    int rightDownX;
    /**
     * 物体在图像中的像素右下角y坐标
     */
    int rightDownY;
};
#endif //ANDROID_TSRRESULT_H
