//
// Created by chuangbo.cheng on 2021/6/7.
//

#ifndef ANDROID_BASETXDATA_H
#define ANDROID_BASETXDATA_H
class BaseTXData {
public:
    bool parseSuccess = false;
    // 接口编号
    int code;
    //
public:
    virtual ~BaseTXData() {}
};
#endif //ANDROID_BASETXDATA_H
