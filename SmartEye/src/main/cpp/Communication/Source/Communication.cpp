//
// Created by lpf on 2025/3/26.
//

#include "Communication.h"
#include "CxxCallJavaHelper.h"
#include "thread.h"


Communication *Communication::getSingleton() {
    static Communication *communication;
    static std::once_flag flag;

    std::call_once(flag, [&]() {
        communication = new Communication();
    });

    return communication;
}

std::atomic<bool> tryConnect(true);

void Communication::init(OnAcceptCallBack callback){
    onAcceptCallBack = callback;
    // 设置与客户端建立连接的回调
    s_listener.setOnAcceptCallBack([this]()->void{
        onAcceptCallBack();
    });
    s_listener.setDataCallBack( [this](int length, const char* data){
        std::shared_ptr<BaseTXData> baseTXData = DataWrapper::parse(data, length);
        if(baseTXData){
            if (baseTXData->code == FuncCode::BigEye_TX2_0070 && onReceiverRoadDiseaseCallBack) {
                std::shared_ptr<RoadDisease> roadDisease = std::dynamic_pointer_cast<RoadDisease>(baseTXData);
                auto ygi = getYGIData(roadDisease->timestamp);
                roadDisease->ygiData = ygi;
                if (ygi != nullptr) {
                    // 如果病害中 没有gps
                    std::vector<SmartGPS*> *gpsVector = ygi->getGpsVector();
                    if (gpsVector->size() > 0) {
                        roadDisease->x = gpsVector->at(0)->getLng();
                        roadDisease->y = gpsVector->at(0)->getLat();
                    } else {
                        // 找相邻的坐标填充
                        fillLngLatWithDisease(roadDisease);
                    }
                }
                onReceiverRoadDiseaseCallBack(roadDisease);
            }

        }
        Log::info("Communication", "收到TCP数据，大小:%d", length);
    });

    // 建议每30秒检测下 上次获取版本号的时间 若超时则 重新创建server
    std::thread connectServer([this]{
        Log::info("Communication", "检测到ip");
        while(tryConnect){
            std::string ip = CxxCallJavaHelper::call("GetEth0Address", "");
            Log::info("Communication", "检测到ip = %s", ip.c_str());
            this->foundIp = (ip == "") ? false : true;
            // 192.168.65.55
            resetServer();
            tryConnect = !mStart;
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    });
    connectServer.detach();

}

void Communication::resetServer() {
    if (pServer != nullptr) {
        bool ret = pServer->Stop();
        Log::info("Communication", "Stop:%d" , ret);
        delete pServer;
        pServer = nullptr;
    }
//    pServer = std::make_shared<CTcpPackServer>(&s_listener);
    pServer = new CTcpPackServer(&s_listener);
    pServer->SetMaxPackSize(0x3FFFFF);
    pServer->SetPackHeaderFlag(0x169);
    pServer->SetKeepAliveTime(5*1000L);
    pServer->SetKeepAliveInterval(5*1000L);
    pServer->SetSocketBufferSize(1024*1024);
    mStart = pServer->Start("192.168.65.55", 9001) ? true : false;
    Log::info("Communication", "TCP连接:%d" , mStart ? 1:0);
}

/**
 * 注册病害接口的回调
 * @param callback
 */
void Communication::registerOnReceiverRoadDiseaseCallBack(OnReceiverRoadDiseaseCallBack callback) {
    onReceiverRoadDiseaseCallBack = callback;
}

/**
 * 发送jpg和vio到TX2
 * @param ygiData
 * @return 0 成功 -1 未建立连接  -2 其它异常
 */
int Communication::sendYGIData(std::shared_ptr<YGIData> ygiData) {

    if (!connected()) {
        Log::info("Communication", "sendYGIData失败了");
        return -1;
    }

    std::thread sendThread([this, ygiData](){
        unsigned char* output = NULL;
        int32_t output_length = 0;
        DataWrapper::wrapYGIData(ygiData, output_length, &output);
        send(output, output_length);
        free(output);
    });

    sendThread.detach();

    return 0;
}

std::shared_ptr<YGIData> Communication::getYGIData(unsigned long long timestamp) {
//    ST::SmartRecursiveLock lock(mutex);
    for (auto &item : ygiDataList) {
        if (item->getSmartFrame()->timestamp == timestamp) {
            return item;
        }
    }
    return nullptr;
}

void Communication::release(){

    if (pServer != nullptr) {
        pServer->Stop();
        pServer = nullptr;
    }
}

void Communication::mockAIDisease() {

    std::shared_ptr<RoadDisease> roadDisease = std::shared_ptr<RoadDisease>(new RoadDisease);
    roadDisease->type = 1;
    // ,
    roadDisease->x = 116.477059;
    roadDisease->y = 39.983996;
    roadDisease->timestamp = ygiDataList.begin()->get()->getSmartFrame()->timestamp;
    roadDisease->info = "[{\"label\":0,\"prob\":0.7041230797767639,\"xmax\":225.2505645751953,\"xmin\":184.6738586425781,\"ymax\":839.4502563476562,\"ymin\":820.0486450195312},{\"label\":1,\"prob\":0.2084242105484009,\"xmax\":407.0290527343750,\"xmin\":370.2635803222656,\"ymax\":834.9066772460938,\"ymin\":817.4910888671875}]";
    auto ygi = getYGIData(roadDisease->timestamp);
    roadDisease->ygiData = ygi;
    roadDisease->diseaseUse = 2;
    if (ygi != nullptr) {
        // 如果病害中 没有gps
        std::vector<SmartGPS*> *gpsVector = ygi->getGpsVector();
        if (gpsVector->size() > 0) {
            roadDisease->x = gpsVector->at(0)->getLng();
            roadDisease->y = gpsVector->at(0)->getLat();
        } else {
            // 找相邻的坐标填充
            fillLngLatWithDisease(roadDisease);
        }
    }
    onReceiverRoadDiseaseCallBack(roadDisease);
}

bool Communication::connected(){
    return mStart && pServer->IsConnected(s_listener.getConnectId());
}


void Communication::send(const unsigned char* data, int length){
    if(connected()){
        BOOL ret = pServer->Send(s_listener.getConnectId(), (BYTE*)data, length);
        if (!ret)
            Log::info("Communication", "发送数据：ret = %d,大小:%d，第一个字节:%d，最后一个字节：%d", ret, length, data[4], data[length-1]);
    }else  {
        Log::info("Communication", "连接异常, send失败");
    }
}

bool Communication::getFoundIp() {
    return foundIp;
}

void Communication::fillLngLatWithDisease(std::shared_ptr<RoadDisease> disease) {
    // 非本身的 差值最小且有gps的
    if (disease->ygiData == nullptr) {
        return;
    }
//    ST::SmartRecursiveLock lock(mutex);
    u8 minDiff = MAXLONG;
    u8 timestamp = 0;
    for (auto &item : ygiDataList) {
        if (item->getSmartFrame()->timestamp == disease->timestamp) {
            continue;
        }
        u8 diff = 0;
        if (item->getSmartFrame()->timestamp > disease->timestamp) {
            diff = item->getSmartFrame()->timestamp - disease->timestamp;
        } else {
            diff = disease->timestamp - item->getSmartFrame()->timestamp;
        }
        if (diff < minDiff) {
            // 获取gps
            auto ygi = disease->ygiData;
            if (disease->ygiData->getGpsVector()->size() > 0) {
                minDiff = diff;
                disease->x = disease->ygiData->getGpsVector()->at(0)->getLng();
                disease->y = disease->ygiData->getGpsVector()->at(0)->getLat();
                timestamp = item->getSmartFrame()->timestamp;
            }
        }
    }
    Log::info("Communication", "fillLngLatWithDisease %lld 使用了 %lld gps坐标", disease->timestamp, timestamp);
}


