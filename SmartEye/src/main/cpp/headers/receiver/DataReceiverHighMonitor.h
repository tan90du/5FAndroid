//
// Created by chuangbo.cheng on 2020/9/22.
//

#ifndef ANDROID_DATARECEIVERHIGHMONITOR_H
#define ANDROID_DATARECEIVERHIGHMONITOR_H

#include <YGIData.h>
// 高仿模拟gps imu yuv
// 可运行在任何Android设备上

// 首先调用服务端接口 获取坐标
// 根据1秒10帧 计算出速度
//

class DataReceiverHighMonitor {
public:
    DataReceiverHighMonitor();
    ~DataReceiverHighMonitor()= default;

    void start() ;
private:
    void startMock();

private:
};


#endif //ANDROID_DATARECEIVERHIGHMONITOR_H
