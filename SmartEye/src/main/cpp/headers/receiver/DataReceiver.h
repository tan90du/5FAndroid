//
// Created by chuangbo.cheng on 2020/8/27.
//

#ifndef ANDROID_DATARECEIVER_H
#define ANDROID_DATARECEIVER_H

#include "YGIData.h"

class DataReceiver {
public:
    DataReceiver() {};

    ~DataReceiver() = default;

    static void start();

    /**
 * 编码完成的回调
 * @param code
 * @param ygiData
 */
    void encodeFinish(int code, std::shared_ptr<YGIData> ygiData);


private:
};


#endif //ANDROID_DATARECEIVER_H
