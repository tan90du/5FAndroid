#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>

#ifdef _WIN32
#include <windows.h>

#define _CRT_SECURE_NO_WARNINGS
#endif

#define PROCESS_ERROR(x)	do{						\
								if(!(x))			\
									goto Exit0 ;	\
							} while(false)


#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

struct LocalTimeRef {
	uint32_t second;
	uint32_t us;

	uint16_t year;
	uint16_t month;
	uint16_t day;
};



#ifndef UNUSED
#define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */
#endif

#define STORE_BASE_VOL_SIZE		(200 * 1024 * 1024 * 1024)
#define STORE_BASE_FREE_SIZE	(20 * 1024 * 1024 * 1024)

#define STORE_BASE_DIR_PATH		"/media/pi/DATA"
#define USB_TTY_ACM_PATH		"/dev/ttyS1"
#define RTK_ACCOUNT_FILE_PATH	"/sdcard/rtk_account.txt"
#define RTK_ACCOUNT_IN_CURRENT_DIR	"/storage/ext4_sdcard/rtk_account.txt"

#define SHARED_MEMORY_NAME		"DATA_SHARED"

#define HARDWARE_SHARED_MEMORY_NAME		"DATA_SHARED_HARDWARE"


#define IMAGE_FRAME_INTERVAL		(49000)

#define AUTORUN_MODE		(0)

#include <android/log.h>
#define LOG_TAG "HDCOLLECT"
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#pragma pack(1)
typedef struct __ISP5200_INFO_S {
    uint64_t    apiVersion;
    uint32_t    maxRspPayloadSizePerChunk;
    uint32_t    chipSku;
    uint32_t    deviceName;
    uint8_t     sensorID[16];
    uint32_t    sensorRev;
    uint64_t    sysFWID;
    uint8_t 	sysFWBuildDate[8];
    uint64_t    p5FWID;
    uint8_t     p5FWBuildDate[8];
    uint64_t    sensorBinRev;
    uint8_t 	sensorBuildDate[8];
    uint64_t    bootloaderBinRev;
    uint8_t     bootloaderBuildDate[8];
}
ISP5200_INFO_S, *PISP5200_INFO_S;
#pragma pack()

#endif
