#pragma once

#include "DataExchange.h"
#include <android/log.h>
#include "YGIData.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Smart", __VA_ARGS__))
extern DataExchangeFrame* ReaderQueryFrameByOffset(void* buf, int32_t offset);
extern ImageExchangeMeta* ReaderQueryMetaByOffset(void* buf, int32_t offset);
extern uint8_t* ReaderQueryImageYuvByOffset(void* buf, int32_t offset);
extern uint8_t* ReaderQueryImageJpegByOffset(void* buf, int32_t offset);
extern uint8_t* ReaderQueryImagePreviewByOffset(void* buf, int32_t offset);
extern IMUFloatData* ReaderQueryImuByOffset(void* buf, int32_t offset);
extern NmeaSentenceBuf* ReaderQueryNmeaSentenceByOffset(void* buf, int32_t offset);
extern UbxSentenceBuf* ReaderQueryUbxSentenceByOffset(void* buf, int32_t offset);
UbxSentenceBuf* ReaderQueryUbxSentenceByOffset(void* buf, int32_t offset);
TSInfoBuf* ReaderQueryXVSByOffset(void* buf, int32_t offset);
TSInfoBuf* ReaderQueryPPSByOffset(void* buf, int32_t offset);
InfoSentenceBuf* ReaderQueryInfoSentenceByOffset(void* buf, int32_t offset);
RawSentenceBuf* ReaderQueryRawSentenceByOffset(void* buf, int32_t offset);
GnssRawBuf* ReaderQueryGnssRawByOffset(void* buf, int32_t offset);
GnssRawBuf* ReaderQueryRtcmRawByOffset(void* buf, int32_t offset);
void HanderReaderFramebuf(int32_t frameIndex, void* buf, std::shared_ptr<YGIData> ygiData);
int32_t ReaderLogDataFromDataExchange(void* buf, int32_t start, int32_t end);
void ReaderPollingFrameReadyData(void* buf);
int32_t FirstReaderFrameReady(void *buf);
int32_t LastReaderFrameReady(void *buf);
void ReaderPollingFrameData(void* buf);
void ReaderPollingNmeaData(void* buf);
void ReaderPollingGnssRaw(void* buf);
void parseGNGGA(const std::string &gnggaStr, SmartGPS *smartGps);
void parseGNRMC(const std::string &gnrmcStr, SmartGPS *smartGps);
void parseGPGST(const std::string &gpgstStr, SmartGPS *smartGps);
void parseWZCOV(const std::string &wzcovStr, SmartGPS *smartGps);
