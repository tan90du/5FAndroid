//
// Created by lpf on 2025/3/26.
//

#ifndef BEDEMO2_COMMUNICATION_H
#define BEDEMO2_COMMUNICATION_H

#include <iostream>
#include <functional>
#include <atomic>
#include "TcpServerImp.h"
#include <string.h>
#include <list>
#include "YGIData.h"
#include "RoadDisease.h"
#include "CommonMacro.h"
#include "Log.h"
#include "DataWrapper.h"


enum FuncCode {
    BigEye_TX2_0010 = 10,  // 请求TX2版本信息
    BigEye_TX2_0020 = 20,  // 发送YGI数据
    BigEye_TX2_0030 = 30,  // 发送升级文件
    BigEye_TX2_0040 = 40,  // 发送请求日志
    BigEye_TX2_0042 = 42,  // 发送vio数据
    BigEye_TX2_0050 = 50,  // TX2向BigEye发送请求
    BigEye_TX2_0060 = 60,  // 发送设备与运行状态标识
    BigEye_TX2_0070 = 70,  // 发送识别结果（无病害不需发送）
    BigEye_TX2_0080 = 80   // 发送日志文件
};


typedef std::function<void()> OnAcceptCallBack;
typedef std::function<void(std::shared_ptr<RoadDisease>)> OnReceiverRoadDiseaseCallBack;


class Communication {
private:
    Communication() = default;

public:
    static Communication *getSingleton();

    void init(OnAcceptCallBack callback)  ;

    bool connected()  ;

    /**
     * 注册各种来自客户端的请求 接收到一个病害
     * @param callback
     */
    void registerOnReceiverRoadDiseaseCallBack(OnReceiverRoadDiseaseCallBack callback) ;

    /**
     * 发送jpg和vio到TX2
     * @param ygiData
     * @return 0 成功 -1 未建立连接  -2 其它异常
     */
    int sendYGIData(std::shared_ptr<YGIData> ygiData) ;


    std::shared_ptr<YGIData> getYGIData(unsigned long long timestamp) ;

    void release()  ;

    /**
     * 模拟一帧病害数据
     */
    void mockAIDisease() ;

    /**
     * 是否找到本地网卡
     * @return
     */
    bool getFoundIp() ;

private:
private:
    void send(const unsigned char* data, int length);

    void resetServer();

    OnAcceptCallBack onAcceptCallBack;
    OnReceiverRoadDiseaseCallBack onReceiverRoadDiseaseCallBack;
    TcpServerImp s_listener;
    CTcpPackServer* pServer = nullptr;
    bool mStart = false;
    std::recursive_mutex mutex;
    std::list<std::shared_ptr<YGIData>> ygiDataList;
    // 是否找到了本地网卡ip
    bool foundIp = false;
    /**
     * 找相邻的坐标
     * @param disease
     */
    void fillLngLatWithDisease(std::shared_ptr<RoadDisease> disease);

};


#endif //BEDEMO2_COMMUNICATION_H
