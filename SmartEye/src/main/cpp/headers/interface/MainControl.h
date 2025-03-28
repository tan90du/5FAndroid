#pragma once

#include <stdint.h>

typedef int(*EventListenerCallback)(uint32_t Module, uint32_t ID, uint32_t type, uint16_t len, const uint8_t *payload);

enum MODULE_ID {
	KERNEL_ID = 0, //kernel 的ID，目前没有使用
	THERMAL_ID,    //温控服务ID,温控服务发送event时使用这个ID
	HDSERVER_ID,   //采集服务ID, 采集服务发送event时使用这个ID
	FOTA_ID,       //ota升级服务ID, ota升级服务发送event时使用这个ID
	VIO_ID,	       //APP端ID，APP端发送event时使用这个ID
	OTHER_ID,
	MODULE_ALL = 0xFFFF,
};

enum EVENT_TYPE {
	EVENT_INFO = 0,
	EVENT_WARNING,
	EVENT_ERROR,
	EVENT_ALL = 0xFFFF,
};

enum EVENT_ERROR_ID {
	THERMAL_CAMERA_TEMP_HIGH = (1<<0),      //Camera模组温度过高标志  0:温度正常 1:温度过高（温度过高时会关闭camera 恢复正常时再打开camera）
	THERMAL_CPU_TEMP_HIGH = (1<<1),         //CPU温度过高    0:温度正常  1:温度过高，这个只是高温报警，不做任何处理
	THERMAL_CPU_TEMP_SHUTDOWN = (1<<2),     //CPU温度超高要关机处理  0:温度正常  1:温度超高，2秒后关机
	HDSERVER_CAMERA_OPENFAIL = (1<<3),      //Camera 打开失败标志  0:打开正常   1:打开失败
	HDSERVER_CAMERA_CALLBACK_TIMEOUT = (1<<4),        //Camera 1秒内没有接收到数据  0:接收数据正常   1:1秒内没有接收到camera数据（理论上每50ms接收一帧camera数据）
	HDSERVER_PPS_TIMEOUT = (1<<5),                    //PPS 数据接收超时    0:接收正常  1:接收超时
	HDSERVER_IMU_TIMEOUT = (1<<6),                   //IMU数据接收超时    0:接收正常  1:接收超时
	HDSERVER_GPS_TIMEOUT = (1<<7),	                //GPS数据接收超时    0:接收正常  1:接收超时
	HDSERVER_XVS_TIMEOUT = (1<<8),	                //CAMERA XVS数据接收超时    0:接收正常  1:接收超时
	HDSERVER_UART_TIMEOUT = (1<<9),                //UART数据接收超时    0:接收正常  1:接收超时
	HDSERVER_ISP5200_UPDATE_FAIL = (1<<10),         //Camera isp5200 升级失败   0:无异常  1：升级失败
	HDSERVER_MCU_UPDATE_FAIL = (1<<11),            //mcu 升级失败   0:无异常  1：升级失败
	FOTA_ANDROID_UPDATE_FAIL = (1<<12),            //android 升级失败   0:无异常  1：升级失败
	FOTA_ISP5200_FILE_DOWNLOAD_FAIL = (1<<13),     //Camera isp5200 升级文件下载失败   0:无异常  1：下载失败
	FOTA_MCU_FILE_DOWNLOAD_FAIL = (1<<14),         //mcu升级文件下载失败   0:无异常  1：下载失败
	FOTA_ANDROID_FILE_DOWNLOAD_FAIL = (1<<15),     //android升级文件下载失败   0:无异常  1：下载失败
	HDSERVER_RTKACCOUNT_AUTHEN_FAIL = (1<<16),     //RTK账号认证失败          0:无异常   1:认证失败
};

enum EVENT_WARNING_ID {
	HDSERVER_CAMERA_CALLBACK_OVERTIME = (1<<0),      //camera两帧数据间隔超过80ms（理论是50ms）   0:无异常    1:两帧数据间隔超过80ms
	HDSERVER_IMU_DATA_UNDERSIZED = (1<<1),		//未使用
	FOTA_NETWORK_ABNORMAL = (1<<2),                 //ota升级网络异常    0:无异常   1:ota网络异常
	OTHER_VALTAGE_OVERHIGH = (1<<3),                 //车机供电电压过高（大于16v）   0:无异常   1:电压过高
	OTHER_VALTAGE_OVERLOW = (1<<4),                  //车机供电电压过低（小于8v）   0:无异常   1:电压过低
	VIO_RAOD_YAWING = (1<<5),                        //道路偏航警报              0:无异常    1:道路偏航
};

enum EVENT_INFO_ID {
	HDSERVER_PPS_TIMELOCK_STATUS = (1<<0),       //GPS时间同步标志     0:未时间同步    1:时间同步成功
	HDSERVER_ISP5200_UPDATE_ONGOING = (1<<1),     //Camera isp5200 升级标志  0:未升级   1:升级过程中
	HDSERVER_MCU_UPDATE_ONGOING = (1<<2),         //mcu 升级标志  0:未升级   1:升级过程中
	FOTA_ANDROID_UPDATE_ONGOING = (1<<3),         //android 升级标志  0:未升级   1:升级过程中
	VIO_START_RTKSERVER = (1<<4),                 //rtk服务使能命令   0:rtk未启用   1:启用rtk服务（app端发送该event）
	VIO_DATA_UPLOADING = (1<<5),                   //数据上传中      0:未上传      1:上传中

	HDSERVER_CAMERA_OPENSTATUS = (1<<16),         //camera是否打开标志    0:未打开   1:打开状态
	RTKSERVER_RUNNING_STATUS = (1<<17),           //rtk服务运行状态       0:rtk服务未运行    1:rtk服务运行中

};

enum COMMAND_ID {
	VIO_SET_RTK_ACCOUNT = 0,                     //设置rtk账号
	PUTLOG_TO_BUSINESS = 1,
};
