//
// Created by chuangbo.cheng on 2020/9/3.
//

#ifndef ANDROID_ENUMVIOSTATUS_H
#define ANDROID_ENUMVIOSTATUS_H
enum class EnumVIOStatus {
    Default,       // 默认值
    NeedCalc,      // 需要计算(已废弃)
    Calculating,   // 计算中
    Calculated,    // 计算完成
    Valid,         // 需要保存且有效(已废弃)
    Valid_Not_KEY, // 需要保存但不是关键帧或者无信息点(已废弃)
    Invalid       //  不需要保存 不关注vio计算的结果(已废弃)
};
#endif //ANDROID_ENUMVIOSTATUS_H
