//
// Created by lpf on 2025/3/26.
//

#include "Communication.h"
#include "CxxCallJavaHelper.h"
#include "thread.h"
#include <sys/stat.h> // Linux/MacOS平台需要这两个头文件
#include <sys/types.h>
// 辅助函数：创建目录
bool createDirectory(const std::string& path) {
    size_t pos = 0;
    do {
        pos = path.find_first_of('/', pos + 1);
        if (mkdir(path.substr(0, pos).c_str(), 0777) != 0 && errno != EEXIST) {
            return false;
        }
    } while (pos != std::string::npos);
    return true;
}

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
        // 新增文件接收逻辑
        const uint8_t* byteData = reinterpret_cast<const uint8_t*>(data);
        int offset = 0;

        // 1. 解析数据类型
        int dataType;
        if (offset + 4 > length) return;
        memcpy(&dataType, byteData + offset, 4);
        offset += 4;
        Log::info("Communication", "lihongwei-111111111111111111:%d", dataType);

        // 验证是否为文件传输类型（假设DataType::Send_Disease对应特定值）
        if (dataType != 70) return; // 替换为实际的Send_Disease值
        Log::info("Communication", "lihongwei-22222222222222");

        // 2. 解析JSON长度
        int jsonLen;
        if (offset + 4 > length) return;
        memcpy(&jsonLen, byteData + offset, 4);
        offset += 4;
        Log::info("Communication", "lihongwei-333333333333");

        // 3. 提取并手动解析JSON
        if (offset + jsonLen + 4 > length) return;
        std::string jsonStr(reinterpret_cast<const char*>(byteData + offset), jsonLen);
        offset += jsonLen;

        // 手动解析文件夹名
        std::string folderName;
        size_t folderStart = jsonStr.find("\"folder\":\"");
        if (folderStart != std::string::npos) {
            folderStart += 10; // 跳过"folder":"
            size_t folderEnd = jsonStr.find('\"', folderStart);
            if (folderEnd != std::string::npos) {
                folderName = jsonStr.substr(folderStart, folderEnd - folderStart);
            }
        }
        Log::info("Communication", "lihongwei-4444444444444444");

        // 手动解析文件名列表
        std::vector<std::string> fileNames;
        size_t filesStart = jsonStr.find("\"files\":[");
        if (filesStart != std::string::npos) {
            filesStart += 9; // 跳过"files":[
            size_t filesEnd = jsonStr.find(']', filesStart);
            if (filesEnd != std::string::npos) {
                std::string filesSection = jsonStr.substr(filesStart, filesEnd - filesStart);
                size_t pos = 0;
                while ((pos = filesSection.find('\"', pos)) != std::string::npos) {
                    size_t endPos = filesSection.find('\"', pos + 1);
                    if (endPos == std::string::npos) break;
                    fileNames.push_back(filesSection.substr(pos + 1, endPos - pos - 1));
                    pos = endPos + 1;
                }
            }
        }
        Log::info("Communication", "lihongwei-5555555555555555555");

        // 4. 解析文件数量
        int fileCount;
        memcpy(&fileCount, byteData + offset, 4);
        offset += 4;

        // 创建本地存储目录
        const std::string baseSavePath = "/storage/ext4_sdcard/Android/data/img/marked/";
        std::string fullPath = baseSavePath + folderName;
        if (!createDirectory(fullPath)) {
            Log::info("FileSave", "Failed to create directory: %s", fullPath.c_str());
            return;
        }
        Log::info("Communication", "lihongwei-66666666666666666");

        // 5. 遍历保存每个文件
        for (int i = 0; i < fileCount; ++i) {
            if (offset + 4 > length) break;

            // 解析文件大小
            int fileSize;
            memcpy(&fileSize, byteData + offset, 4);
            offset += 4;

            // 检查数据边界
            if (offset + fileSize > length) break;

            // 写入文件
            std::string filename = fullPath + "/" + fileNames[i];
            FILE* fp = fopen(filename.c_str(), "wb");
            if (fp) {
                fwrite(byteData + offset, 1, fileSize, fp);
                fclose(fp);
                Log::info("FileSave", "Saved: %s (%d bytes)", filename.c_str(), fileSize);
            } else {
                Log::info("FileSave", "Failed to write: %s", filename.c_str());
            }
            offset += fileSize;
        }

        Log::info("FileSave", "接收到 %d 个文件，在文件夹: %s中", fileCount, folderName.c_str());
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


