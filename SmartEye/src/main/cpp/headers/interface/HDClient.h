#pragma once
#include <stdint.h>
#include <sys/cdefs.h>
#include <functional>
#include "HDErrortype.h"

namespace autonavi {
namespace HDCOLLECT {

typedef void(*UartDataCallbackFunc)(uint8_t Class, uint8_t ID, uint16_t len, const uint8_t* payload);
typedef void(*GPSDataCallbackFunc)(uint16_t len, const uint8_t* payload);
typedef void(*FrameDataCallbackFunc)(uint32_t frameIndex);
typedef void(*ServiceDiedCallbackFunc)(void);
typedef void(*EventCallbackFunc)(uint32_t Module, uint32_t ID, uint32_t type, uint16_t len, const uint8_t* payload);
typedef void(*LogDataCallbackFunc)(uint32_t frameIndex);
//typedef std::function<void(uint8_t, uint8_t, uint16_t, const uint8_t*)> UartDataCallbackFunc;
//typedef std::function<void(uint16_t, const uint8_t*)> GPSDataCallbackFunc;
//typedef std::function<void(int32_t)> FrameDataCallbackFunc; 
//typedef std::function<void(void)> ServiceDiedCallbackFunc;

struct HDCollect_Callbacks {
    UartDataCallbackFunc uartDataHandler;
    GPSDataCallbackFunc  gpsDataHandler;
    FrameDataCallbackFunc frameDataHandler;
    ServiceDiedCallbackFunc  serviceDiedHandler;
    EventCallbackFunc  eventHandler;
    LogDataCallbackFunc logDataHandler;
};

/**
 ** This used to connect mcu service.
 **/
int HDClient_ConnectService(ServiceDiedCallbackFunc callback);

/**
 ** This used to start HDCamera.
 **/
int HDClient_startHDCamera(FrameDataCallbackFunc callback);

/**
 ** This used to stop HDCamera
 **/
int HDClient_stopHDCamera();

/**
 ** This used to get share memory
 **/
int HDClient_getShareMemory(void** pointer, size_t* size);

/**
 ** Send command to MainControl
 **/
int MainControl_sendCommand(uint32_t Module, uint32_t ID, uint16_t len, const uint8_t* payload);

/**
 ** Send event to MainControl
 **/
int MainControl_sendEvent(uint32_t Module, uint32_t ID, uint32_t type, uint32_t set);

/**
 ** Register event listener
 **/
int MainControl_registerEventListener(uint32_t Module, uint32_t ID, uint32_t type, EventCallbackFunc callback);

/**
 ** Cancel event listener
 **/
int MainControl_cancelEventListener(uint32_t Module, uint32_t ID, uint32_t type);

/**
 ** Register Log listener
 **/
int HDClient_registerLogListener(LogDataCallbackFunc callback);

/**
 ** Cancel Log Listener
 **/
int HDClient_cancelLogListener();

/**
 ** Get event statistics from MainControl
 **/
int MainControl_getEventStatistics(uint32_t Module, uint32_t ID, uint32_t type);

/**
 ** This creates a threadpool for incoming binder transactions if it has not already been created.
 **/
void HDClient_startThreadPool();

/**
 ** This sets the maximum number of threads that can be started in the threadpool. By default, after
 ** startThreadPool is called, this is one. If it is called additional times, it will only prevent
 ** the kernel from starting new threads and will not delete already existing threads.
 **/
bool HDClient_setThreadPoolMaxThreadCount(uint32_t numThreads);

/**
 ** This sets the maximum number of threads that can be started in the threadpool. By default, after
 ** startThreadPool is called, this is one. If it is called additional times, it will only prevent
 ** the kernel from starting new threads and will not delete already existing threads.
 **/
void HDClient_joinThreadPool();


};
};
