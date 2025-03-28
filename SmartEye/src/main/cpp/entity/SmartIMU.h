//
// Created by chuangbo.cheng on 2020/8/3.
//

#ifndef ANDROID_SMARTIMU_H
#define ANDROID_SMARTIMU_H

#include "DataTypeDefine.h"

/**
 * 暂未添加积分后的字段
 */
class SmartIMU{
public:
    Timestamp getTimestamp() const {
        return timestamp;
    }

    void setTimestamp(Timestamp time) {
        this->timestamp = time;
    }

    double getGyroX() const {
        return gyroX;
    }

    void setGyroX(double gyroX) {
        this->gyroX = gyroX;
    }

    double getGyroY() const {
        return gyroY;
    }

    void setGyroY(double gyroY) {
        this->gyroY = gyroY;
    }

    double getGyroZ() const {
        return gyroZ;
    }

    void setGyroZ(double gyroZ) {
        this->gyroZ = gyroZ;
    }

    double getAccelX() const {
        return accelX;
    }

    void setAccelX(double accelX) {
        this->accelX = accelX;
    }

    double getAccelY() const {
        return accelY;
    }

    void setAccelY(double accelY) {
        this->accelY = accelY;
    }

    double getAccelZ() const {
        return accelZ;
    }

    void setAccelZ(double accelZ) {
        this->accelZ = accelZ;
    }

    // 预留的字段
    std::string moreField = "";

private:
    /**
     * 时间戳 精确到毫秒
     */
    Timestamp timestamp = 0;
    /**
     * 陀螺仪x轴
     */
    double gyroX = 0;
    /**
    * 陀螺仪y轴
    */
    double gyroY = 0;
    /**
    * 陀螺仪z轴
    */
    double gyroZ = 0;
    /**
     * 加速度x轴
     */
    double accelX = 0;
    /**
    * 加速度y轴
    */
    double accelY = 0;
    /**
    * 加速度z轴
    */
    double accelZ = 0;
};
#endif //ANDROID_SMARTIMU_H
