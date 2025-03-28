//
// Created by chuangbo.cheng on 2020/8/3.
// GPS 实体类
//

#ifndef SMART_EYE_GPS_H
#define SMART_EYE_GPS_H

#include "DataTypeDefine.h"

class SmartGPS{
public:
    Timestamp getTimestamp() const {
        return timestamp;
    }

    void setTimestamp(Timestamp time) {
        this->timestamp = time;
    }

    double getLng() const {
        return lng;
    }

    void setLng(double longitude) {
        this->lng = longitude;
    }

    double getLat() const {
        return lat;
    }

    void setLat(double latitude) {
        this->lat = latitude;
    }

    double getAltitude() const {
        return altitude;
    }

    void setAltitude(double altitudeValue) {
        this->altitude = altitudeValue;
    }

    double getHdop() const {
        return hdop;
    }

    void setHdop(double hDop) {
        this->hdop = hDop;
    }

    int getGpsNum() const {
        return gpsNUM;
    }

    void setGpsNum(int gpsNum) {
        gpsNUM = gpsNum;
    }

    int getGpsStatus() const {
        return gpsStatus;
    }

    void setGpsStatus(int status) {
        this->gpsStatus = status;
        if (status == 2 || status == 4 || status == 5){
            rtkStatus = 2;
        }else {
            rtkStatus = 1;
        }
    }

    int getRtkStatus() const {
        return rtkStatus;
    }

    void setRtkStatus(int status) {
        this->rtkStatus = status;
    }

    double getSpeed() const {
        return speed;
    }

    void setSpeed(double speed) {
        this->speed = speed;
    }

    double getBearing() {
        return bearing;
    }

    void setBearing(double _bearing) {
        this->bearing = _bearing;
    }

    double getLatErrorDeviation() {
        return latErrorDeviation;
    }

    void setLatErrorDeviation(double _latErrorDeviation) {
        this->latErrorDeviation = _latErrorDeviation;
    }

    double getLngErrorDeviation() {
        return lngErrorDeviation;
    }

    void setLngErrorDeviation(double _lngErrorDeviation) {
        this->lngErrorDeviation = _lngErrorDeviation;
    }

    double getAltErrorDeviation() {
        return altErrorDeviation;
    }

    void setAltErrorDeviation(double _altErrorDeviation) {
        this->altErrorDeviation = _altErrorDeviation;
    }

    double getCxx() {
        return Cxx;
    }

    void setCxx(double _Cxx) {
        this->Cxx = _Cxx;
    }

    double getCxy() {
        return Cxy;
    }

    void setCxy(double _Cxy) {
        this->Cxy = _Cxy;
    }

    double getCxz() {
        return Cxz;
    }

    void setCxz(double _Cxz) {
        this->Cxz = _Cxz;
    }

    double getCyy() {
        return Cyy;
    }

    void setCyy(double _Cyy) {
        this->Cyy = _Cyy;
    }

    double getCyz() {
        return Cyz;
    }

    void setCyz(double _Cyz) {
        this->Cyz = _Cyz;
    }

    double getCzz() {
        return Czz;
    }

    void setCzz(double _Czz) {
        this->Czz = _Czz;
    }
    double getVxx() {
        return Vxx;
    }

    void setVxx(double _Vxx) {
        this->Vxx = _Vxx;
    }

    double getVxy() {
        return Vxy;
    }

    void setVxy(double _Vxy) {
        this->Vxy = _Vxy;
    }

    double getVxz() {
        return Vxz;
    }

    void setVxz(double _Vxz) {
        this->Vxz = _Vxz;
    }

    double getVyy() {
        return Vyy;
    }

    void setVyy(double _Vyy) {
        this->Vyy = _Vyy;
    }

    double getVyz() {
        return Vyz;
    }

    void setVyz(double _Vyz) {
        this->Vyz = _Vyz;
    }

    double getVzz() {
        return Vzz;
    }

    void setVzz(double _Vzz) {
        this->Vzz = _Vzz;
    }

    std::string & getAdditionInfo () {

        if (additionInfo.empty()) {
            additionInfo
                    .append(std::to_string(latErrorDeviation)).append(",")
                    .append(std::to_string(lngErrorDeviation)).append(",")
                    .append(std::to_string(altErrorDeviation)).append(",")
                    .append(std::to_string(Cxx)).append(",")
                    .append(std::to_string(Cxy)).append(",")
                    .append(std::to_string(Cxz)).append(",")
                    .append(std::to_string(Cyy)).append(",")
                    .append(std::to_string(Cyz)).append(",")
                    .append(std::to_string(Czz)).append(",")
                    .append(std::to_string(Vxx)).append(",")
                    .append(std::to_string(Vxy)).append(",")
                    .append(std::to_string(Vxz)).append(",")
                    .append(std::to_string(Vyy)).append(",")
                    .append(std::to_string(Vyz)).append(",")
                    .append(std::to_string(Vzz));
        }


        return additionInfo;

    }

    void setAdditionInfo(std::string additionInfo){
        this->additionInfo = additionInfo;
    }

    std::shared_ptr<SmartGPS> deepCopy() const {
        std::shared_ptr<SmartGPS> temp(new SmartGPS());
        temp->setAltitude(altitude);
        temp->setRtkStatus(rtkStatus);
        temp->setGpsStatus(gpsStatus);
        temp->setGpsNum(gpsNUM);
        temp->setHdop(hdop);
        temp->setTimestamp(timestamp);
        temp->setLat(this->lat);
        temp->setLng(this->lng);
        temp->setSpeed(this->speed);
        temp->setBearing(this->bearing);
        temp->vioLat = this->vioLat;
        temp->vioLng = this->vioLng;
        temp->vioHeading = this->vioHeading;
        temp->setLatErrorDeviation(this->latErrorDeviation);
        temp->setLngErrorDeviation(this->lngErrorDeviation);
        temp->setLatErrorDeviation(this->altErrorDeviation);

        temp->setCxx(this->Cxx);
        temp->setCxy(this->Cxy);
        temp->setCxz(this->Cxz);

        temp->setCyy(this->Cyy);
        temp->setCyz(this->Cyz);
        temp->setCzz(this->Czz);

        temp->setVxx(this->Vxx);
        temp->setVxy(this->Vxy);
        temp->setVxz(this->Vxz);

        temp->setVyy(this->Vyy);
        temp->setVyz(this->Vyz);
        temp->setVzz(this->Vzz);
        temp->bootTime = this->bootTime;
        temp->setAdditionInfo(this->additionInfo);

        return temp;
    }

private:
    /**
     * 时间戳
     */
    Timestamp timestamp = 0;

    /**
     * 坐标 经度
     */
    double lng = 0;
    /**
    * 坐标 纬度
    */
    double lat = 0;
    /**
     * 高度
     */
    double altitude = 0;
    /**
     * 水平精度因子
     */
    double hdop = 0;
    /**
     * 卫星数量
     */
    int gpsNUM = 0;
    /**
     * GPS状态 0初始化 1 单点定位 2 码差分 3 无效PPS 4 固定解
     * 5 浮点解  6 正在估算 7 人工输入固定值 8 模拟模式 9 WAAS差分
     * 如果出现2,4或者5都表示rtk起作用了
     */
    int gpsStatus = 0;
    /**
     * RTK的状态 0 未知 1 未使用  2 已使用
     */
    int rtkStatus = 0;
    /**
     * 精度
     */
//    double accuracy = 0;
    /**
     * 速度
     */
    double speed = 0;
    /**
     * 0-359.99 航向
     */
    double bearing = -1;

    /**
     * GPGST 3个字段
     */
    double latErrorDeviation = 0;

    double lngErrorDeviation = 0;

    double altErrorDeviation = 0;

    /**
     * WZCOV 12个字段
     */

    double Cxx = 0;

    double Cxy = 0;

    double Cxz = 0;

    double Cyy = 0;

    double Cyz = 0;

    double Czz = 0;

    double Vxx = 0;

    double Vxy = 0;

    double Vxz = 0;

    double Vyy = 0;

    double Vyz = 0;

    double Vzz = 0;

    std::string additionInfo = "";

public:
    // vio计算出来的 vio内部不需要赋值
    double vioLat = 0.0;
    // vio计算出来的 vio内部不需要赋值
    double vioLng = 0.0;
    // vio计算出来的 vio内部不需要赋值
    double vioHeading = 0.0;
    // 开机时长
    u8 bootTime = 0;

    // 预留的字段
    std::string moreField = "";
};

typedef std::vector<std::shared_ptr<SmartGPS> > GpsPtrVector;

#endif //SMART_EYE_GPS_H
