#ifndef _WGS2GCJ_H_
#define _WGS2GCJ_H_

#if (defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#  ifdef MARS_USE_STATIC
#    define MARS_DLL_EXPORT
#  else
#    ifdef MARS_BUILD_DLL
#      define MARS_DLL_EXPORT __declspec(dllexport)
#    else
#      define MARS_DLL_EXPORT __declspec(dllimport)
#    endif
#  endif
#else
#  define MARS_DLL_EXPORT
#endif

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

    /**
    * @brief            WGS84 to GCJ02
    * @param lng_wgs    WGS84经度(单位:度)
    * @param lat_wgs    WGS84纬度(单位:度)
    * @param lng_gcj    GCJ02经度(单位:度)
    * @param lat_gcj    GCJ02经度(单位:度)
    * @return           0x00000000:正常结束 0xFFFF95FF:参数错误
    */
    MARS_DLL_EXPORT unsigned int wgs2gcj(double lng_wgs, double lat_wgs, double *lng_gcj, double *lat_gcj);

    /**
    * @brief            WGS84 to GCJ02
    * @param lng_wgs    WGS84经度(单位:秒)
    * @param lat_wgs    WGS84纬度(单位:秒)
    * @param lng_gcj    GCJ02经度(单位:秒)
    * @param lat_gcj    GCJ02经度(单位:秒)
    * @return           0x00000000:正常结束 0xFFFF95FF:参数错误
    */
    MARS_DLL_EXPORT unsigned int wgs2gcj_second(double lng_wgs, double lat_wgs, double *lng_gcj, double *lat_gcj);

#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif /* _WGS2GCJ_H_ */
