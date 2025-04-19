//
// Created by chuangbo.cheng on 2020/8/3.
//

#ifndef ANDROID_YGIDATA_H
#define ANDROID_YGIDATA_H

#include "SmartFrame.h"
#include "SmartGPS.h"
#include "SmartIMU.h"
#include "VIOResult.h"
#include "GpsUblox.h"
#include <vector>
#include "EnumVIOStatus.h"
#include "EnumYGISaveType.h"
#include "Log.h"
class YGIData {
public:
    YGIData(){
        this->mGPSVector = new std::vector<SmartGPS*>;
        this->mSmartFrame = new SmartFrame();
        this->mIMUVector = new std::vector<SmartIMU*>();
    }
    ~YGIData(){
        // 释放资源
        if (mSmartFrame != nullptr){
            delete[] mSmartFrame->getFrameData();
            delete mSmartFrame;
            mSmartFrame = nullptr;
//            Log::info("neicunshifang", "释放frame数据了");
        }
        // 以下可以考虑模板函数实现
        if (mGPSVector != nullptr){
            for (auto &item : *mGPSVector){
                delete item;
                item = nullptr;
            }
            delete mGPSVector;
            mGPSVector = nullptr;
        }
        if (mIMUVector != nullptr){
            for (auto &item : *mIMUVector){
                delete item;
                item = nullptr;
            }
            delete mIMUVector;
            mIMUVector = nullptr;
        }
        if(vioResult != nullptr){
            delete vioResult;
            vioResult = nullptr;
        }
        if (gpsUblox != nullptr){
            delete gpsUblox;
            gpsUblox = nullptr;
        }
    }

    SmartFrame *getSmartFrame() const {
        return mSmartFrame;
    }

    void setSmartFrame(SmartFrame *smartFrame) {
        this->mSmartFrame = smartFrame;
    }

    std::vector<SmartGPS *> *getGpsVector() const {
        return mGPSVector;
    }

    void setGpsVector(std::vector<SmartGPS *> *gpsVector) {
        mGPSVector = gpsVector;
    }

    std::vector<SmartIMU *> *getImuVector() const {
        return mIMUVector;
    }

    void setMimuVector(std::vector<SmartIMU *> *imuVector) {
        mIMUVector = imuVector;
    }

    EnumVIOStatus getStatus() const {
        return status;
    }

    void setStatus(EnumVIOStatus status) {
        this->status = status;
    }

    VIOResult *getVioResult() const {
        return vioResult;
    }

    void setVioResult(VIOResult *vioResult) {
        this->vioResult = vioResult;
    }

    /**
     * 获取当前数据的 GPS 开始时间和结束时间。
     *
     * @param outStart 开始时间
     * @param outEnd 结束时间
     *
     * @return 0 正常； -1 异常。
     */
    int getGpsTimeRange(Timestamp& outStart, Timestamp& outEnd) {
        if (this->mGPSVector->empty()) {
            return -1;
        }

        auto begin = mGPSVector->begin();
        outStart = (*begin)->getTimestamp();

        auto end = mGPSVector->back();
        outEnd = end->getTimestamp();

        return 0;
    }

    void setGpsUblox(GpsUblox *gpsUblox) {
        this->gpsUblox = gpsUblox;
    }

    GpsUblox *getGpsUblox() const {
        return gpsUblox;
    }

    EnumYGISaveType getSaveType() const {
        return saveType;
    }

    void setSaveType(EnumYGISaveType saveType) {
        this->saveType = saveType;
    }
    int getNeedSaveStatus() const {
        return needSaveStatus;
    }

    void setNeedSaveStatus(int needSaveStatus) {
        this->needSaveStatus = needSaveStatus;
    }

public:
    bool isAddSaveCache = false;
private:
    /**
     * YUV对象 存放图片数据等信息
     */
    SmartFrame *mSmartFrame;
    /**
     * 两帧yuv之间的gps集合 size可能为空
     */
    std::vector<SmartGPS*> *mGPSVector;
    /**
     * 两帧yuv之间的IMU 包括加速度、角速度
     */
    std::vector<SmartIMU*> *mIMUVector;
    /**
     * vio计算出的当前桢的位姿
     */
    VIOResult *vioResult = nullptr;
    /**
     * 状态
     */
    EnumVIOStatus status = EnumVIOStatus::Default;

    EnumYGISaveType saveType = EnumYGISaveType::Not_Save_Any;
    /**
     * 标识是否已经落盘过了
     */
    bool isAlreadyExistsInDB = false;
    GpsUblox *gpsUblox = nullptr;
    // 是否需要持久化 0 默认值(未知) 1 需要 2 不需要
    int needSaveStatus = 0;

};
#endif //ANDROID_YGIDATA_H
