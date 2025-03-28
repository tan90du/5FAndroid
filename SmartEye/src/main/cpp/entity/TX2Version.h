//
// Created by chuangbo.cheng on 2021/6/7.
//

#ifndef ANDROID_TX2VERSION_H
#define ANDROID_TX2VERSION_H

#include <string>
#include <list>
#include <BaseTXData.h>

class TX2VersionInfo {
public:
    std::string name;
    std::string version;
};


class TX2Version :public BaseTXData{

public:
    ~TX2Version() {

    }
    std::list<std::shared_ptr<TX2VersionInfo>> list;
};
#endif //ANDROID_TX2VERSION_H
