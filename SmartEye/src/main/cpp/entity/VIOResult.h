//
// Created by chuangbo.cheng on 2020/8/26.
//

#ifndef ANDROID_VIORESULT_H
#define ANDROID_VIORESULT_H

#include <string>
class VIOResult {
public:
    void setInfo(std::string info){
        this->info = info;
    }
    // 避免重新拷贝一份
    std::string& getInfo(){
        //
        return info;
    }
    std::string info = "";
    /**
     * 0 初始化已完成 1 初始化中 2 内部异常
     */
    int code = 0;

    /**
     * 时间戳
     */
    unsigned long long timestamp = 0;

    /**
     * 是否为关键帧
     */
    bool isKeyFrame = false;

    /**
     * utc时间  "utc"
     */
    unsigned long long utcTime = 0;

    /**
     * 局部时间 "header" 相对时间，算法定义的时间，因为使用绝对时间戳比较长,计算耗时
     */
    double headerTime = 0.0;

    /**
     * 经度 "lon_lat_alt.x()"
     */
    double lonLatAltX = 0.0;

    /**
     * 纬度 "lon_lat_alt.y()"
     */
    double lonLatAltY = 0.0;

    /**
     * 高度 "lon_lat_alt.z()"
     */
    double lonLatAltZ = 0.0;

    /**
     * 旋转角度 w  "q.w()"
     */
    double rotationAngleW = 0.0;

    /**
     * 旋转角度 x "q.x()"
     */
    double rotationAngleX = 0.0;

    /**
     * 旋转角度 y "q.y()"
     */
    double rotationAngleY = 0.0;

    /**
     * 旋转角度 z "q.z()"
     */
    double rotationAngleZ = 0.0;

    /**
     * 旋转角度绕x轴角度 "ypr.x()"
     */
    double rotationAngleXAxis = 0.0;

    /**
     * 旋转角度绕y轴角度 "ypr.y()"
     */
    double rotationAngleYAxis = 0.0;

    /**
     * 旋转角度绕z轴角度 "ypr.z()"
     */
    double rotationAngleZAxis = 0.0;

    /**
     * x线速度 "v.x()"
     */
    double linearVelocityX = 0.0;

    /**
     * y线速度 "v.y()"
     */
    double linearVelocityY = 0.0;

    /**
     * z线速度 "v.z()"
     */
    double linearVelocityZ = 0.0;


    /**
     * 绕x周角速度 "w.x()"
     */
    double angularVelocityX = 0.0;

    /**
     * 绕y周角速度 "w.y()"
     */
    double angularVelocityY = 0.0;

    /**
     * 绕z周角速度 "w.z()"
     */
    double angularVelocityZ = 0.0;

    /**
     * 行曝光延时 "rs_dt"
     */
    unsigned long long exposureDelayTime = 0;

    /**
     * 位姿好坏的标志位，默认值是什么？ "acc_lev"
     * 0：坏  1：好
     */
    int accLevel = 0;

    /**
     * 当前位置x方差 "cov_cur_p(0, 0)"
     */
    double currentPosVarianceOfX = 0.0;

    /**
     * 当前位置y方差 "cov_cur_p(1, 1)"
     */
    double currentPosVarianceOfY = 0.0;

    /**
     * 当前位置z方差 "cov_cur_p(2, 2)"
     */
    double currentPosVarianceOfZ = 0.0;

    /**
     * 当前绕x轴角度方差 "cov_cur_q(0, 0)"
     */
    double rotationAngleXAxisVariance = 0.0;

    /**
     * 当前绕y轴角度方差 "cov_cur_q(1, 1)"
     */
    double rotationAngleYAxisVariance = 0.0;

    /**
     * 当前绕z轴角度方差 "cov_cur_q(2, 2)"
     */
    double rotationAngleZAxisVariance = 0.0;

    /**
     * 相对位置x方差 "cov_adj_p(0, 0)"
     */
    double relativePosVarianceOfX = 0.0;

    /**
     * 相对位置y方差 "cov_adj_p(1, 1)"
     */
    double relativePosVarianceOfY = 0.0;

    /**
     * 相对位置z方差 "cov_adj_p(2, 2)"
     */
    double relativePosVarianceOfZ = 0.0;

    /**
     * 相对绕x轴角度方差 "cov_adj_q(0, 0)"
     */
    double relativeXAxisVariance = 0.0;

    /**
     * 相对绕y轴角度方差 "cov_adj_q(1, 1)"
     */
    double relativeYAxisVariance = 0.0;

    /**
     * 相对绕z轴角度方差 "cov_adj_q(2, 2)"
     */
    double relativeZAxisVariance = 0.0;


    /**
     * 位姿信息
     *  1 long long、19 long，20 int，其他类型是double
     *  1、 UTC时间（long long）： << utc << " "
        2、 局部时间： << header << " "
        3、 经： << lon_lat_alt.x() << " "
        4、 纬：     << lon_lat_alt.y() << " "
        5、 高：    << lon_lat_alt.z() << " "
        6、 旋转角度四元素w：    << q.w() << " "
        7、 旋转角度四元素x：    << q.x() << " "
        8、 旋转角度四元素y：    << q.y() << " "
        9、 旋转角度四元素z：    << q.z() << " "
        10、旋转角度绕x轴角度：    << ypr.x() << " "
        11、 旋转角度绕y轴角度：    << ypr.y() << " "
        12、旋转角度绕z轴角度：    << ypr.z() << " "
        13、x线速度：   << v.x() << " "
        14、 y线速度：    << v.y() << " "
        15、 z线速度：    << v.z() << " "
        16、 绕x周角速度：    << w.x() << " "
        17、 绕y周角速度：   << w.y() << " "
        18、 绕z周角速度：    << w.z() << " "
        19、 行曝光延时(long)：    << rs_dt << " "
        20、位姿好坏的标示位(int)：    << acc_lev << " "
        21、 当前位置x方差：    << cov_cur_p(0, 0) << " "
        22、当前位置y方差：    << cov_cur_p(1, 1) << " "
        23、当前位置z方差：    << cov_cur_p(2, 2) << " "
        24、当前绕x轴角度方差：    << cov_cur_q(0, 0) << " "
        25、当前绕y轴角度方差：   << cov_cur_q(1, 1) << " "
        26、当前绕z轴角度方差：   << cov_cur_q(2, 2) << " "
        27、相对位置x方差：   << cov_adj_p(0, 0) << " "
        28、相对位置y方差：   << cov_adj_p(1, 1) << " "
        29、相对位置z方差：   << cov_adj_p(2, 2) << " "
        30、相对绕x轴角度方差：   << cov_adj_q(0, 0) << " "
        31、 相对绕y轴角度方差：   << cov_adj_q(1, 1) << " "
        32、 相对绕z轴角度方差：   << cov_adj_q(2, 2) << " "
     */
};

#endif //ANDROID_VIORESULT_H
