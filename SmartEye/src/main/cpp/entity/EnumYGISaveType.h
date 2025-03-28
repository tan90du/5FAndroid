//
// Created by chuangbo.cheng on 2020/10/12.
//

#ifndef ANDROID_ENUMYGISAVETYPE_H
#define ANDROID_ENUMYGISAVETYPE_H
enum EnumYGISaveType {
    Default,       // 待计算
    Not_Save_Any,  // 什么都不保存
    Save_All,      // 保存所有
    Save_IMU_GPS   // 仅保存imu和gps
 };
#endif //ANDROID_ENUMYGISAVETYPE_H
