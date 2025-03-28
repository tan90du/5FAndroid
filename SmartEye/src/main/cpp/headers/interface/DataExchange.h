#ifndef _DATA_EXCHANGE_H_
#define _DATA_EXCHANGE_H_

#include <stdint.h>

#ifndef ROUND_UP_TO
#define ROUND_UP_TO(a,b)	((b) * ((a) / (b) + (((a) % (b) > 0) ? 1 : 0)))
#endif


#define DATA_EXCHANGE_BUFFER_RESERVED		(1 * 1024 * 1024)
#define FRAME_SEARCH_FORWARD_COUNT		(15)

#define IMAGE_FORMAT_YUV		' VUY'
#define IMAGE_FORMAT_JPEG		'GEPJ'
#define IMAGE_FORMAT_PREVIEW	'WERP'

#define IMAGE_INDEX_YUV			(0)
#define IMAGE_INDEX_JPEG		(1)
#define IMAGE_INDEX_PREVIEW		(2)

#define SUB_BUF_INDEX_FRAME		(0)
#define SUB_BUF_INDEX_META		(1)
#define SUB_BUF_INDEX_YUV		(2)
#define SUB_BUF_INDEX_JPEG		(3)
#define SUB_BUF_INDEX_PREVIEW	(4)
#define SUB_BUF_INDEX_IMU		(5)
#define SUB_BUF_INDEX_GNSS_NMEA	(6)
#define SUB_BUF_INDEX_GNSS_UBX	(7)
#define SUB_BUF_INDEX_INFO		(8)
#define SUB_BUF_INDEX_RAW		(9)
#define SUB_BUF_INDEX_TS_XVS	(10)
#define SUB_BUF_INDEX_TS_PPS	(11)
#define SUB_BUF_INDEX_GNSS_RAW	(12)
#define SUB_BUF_INDEX_RTCM		(13)
#define SUB_BUF_COUNT			(14)

#define UBX_SENTENCE_BUF_SIZE  (4096)
#define NMEA_SENTENCE_BUF_SIZE (256)
#define INFO_SENTENCE_BUF_SIZE	(1024)
#define GNSS_RAW_BUF_SIZE		(512)
#define RAW_SENTENCE_BUF_SIZE	(64)


#pragma pack(1)

struct ImageExchangeConfig {
	uint32_t format;
	int32_t width;
	int32_t height;
	int32_t channel;
	int32_t stride[3]; // Max 3 channels, for YUV could be Y/U/V, for RGB only use [0]
	int32_t size;
};


struct ImageAncillaryData {
    uint8_t       header[4];
    uint8_t       payloadMajorVersion;
    uint8_t       payloadMinorVersion;
    uint16_t payloadNumBytes;
    uint32_t gw5State;
    float    gw5Temperature;
    uint32_t inputFrameErrorCount;
    uint32_t inputFrameCount;
    uint32_t inputFrameCrc;
    uint64_t inputFrameTimestampMs;
    uint32_t outputFrameCount;
    uint32_t outputFrameCrc;
    uint64_t outputFrameTimestampMs;
    float    totalGain[4];
    float    sensorGain[4];
    float    ispGain[4];
    float    unclippedEv[4];
    float    unclippedEvAvg;
    uint16_t integrationLines[4];
    uint16_t vts;
    uint8_t  padAe[2];
    float    frameRate;
    uint8_t  numberOfSensors;
    uint8_t  padSensor[3];
    uint16_t longMediumRatio;
    uint16_t mediumShortRatio;
    uint16_t shortVeryShortRatio;
    uint8_t  padCombiner[2];
    int16_t  colorMatrix[9];
    uint8_t  padCcm[2];
    uint16_t ltmGain;
    uint8_t  padLtm[2];
    uint16_t blackOffsetR;
    uint16_t blackOffsetGR;
    uint16_t blackOffsetGB;
    uint16_t blackOffsetB;
    uint8_t  payloadChecksum;
};


struct ImageExchangeMeta {
	int64_t ts;

	float integrationInUs;
	float lineExposureDelayInUs; 

	ImageAncillaryData ancData;
};

struct DataExchangeConfig {
	int32_t sizeInSecond;

	int32_t frameRateImage;
	int32_t frameImageDifInUs;
	int32_t frameImageXVSDelayThres;
	int32_t frameRateGnss;
	int32_t frameRateImu;
	int32_t frameRateInfo;
	int32_t frameRateRaw;

	ImageExchangeConfig imageConfig[3]; // For Yuv/Jpeg/Preview

	int32_t imuSources;
	int32_t subSentenceNmea;
	int32_t subSentenceUbx;

// 	int32_t sizeOfNmeaSentence;
// 	int32_t sizeOfUbxSentence;
};


struct DataExchangeBufInfo {
	int32_t cur;

	int32_t total;
	int32_t itemSize;
	
	uint64_t data;
    int32_t offset;
	int32_t bufSize;
};

struct IMUFloatData
{
	uint8_t source;
	int64_t ts;

	float gyro[3];
	float accel[3];
	float temp;
};

struct NmeaSentenceBuf {
	int64_t ts;

	uint8_t buf[NMEA_SENTENCE_BUF_SIZE];
};

struct UbxSentenceBuf {
	int64_t ts;
	uint8_t classId;
	uint8_t msgId;
	int32_t size;
	uint8_t buf[UBX_SENTENCE_BUF_SIZE];
};

struct InfoSentenceBuf {
	uint64_t mpuTs;
	uint8_t id;
	uint32_t seq;
	int32_t size;
	uint8_t buf[INFO_SENTENCE_BUF_SIZE];
};

struct RawSentenceBuf {
	uint64_t mpuTs;
	int32_t size;
	uint8_t buf[RAW_SENTENCE_BUF_SIZE];
};

struct GnssRawBuf {
	uint64_t mpuTs;
	int32_t size;
	uint8_t buf[GNSS_RAW_BUF_SIZE];
};

struct TSInfoBuf {
	int64_t globalTs;
	uint64_t mpuTs;
};


#define FRAME_READY_FLAG_TOTAL		(FRAME_READY_FLAG_YUV | FRAME_READY_FLAG_JPEG | FRAME_READY_FLAG_PREVIEW | FRAME_READY_FLAG_IMU | FRAME_READY_FLAG_UBX | FRAME_READY_FLAG_NMEA)
#define FRAME_READY_FLAG_ESSENTIAL	(FRAME_READY_FLAG_YUV | FRAME_READY_FLAG_IMU | FRAME_READY_FLAG_UBX | FRAME_READY_FLAG_NMEA)
#define FRAME_READY_FLAG_STORAGE	(FRAME_READY_FLAG_YUV | FRAME_READY_FLAG_JPEG | FRAME_READY_FLAG_IMU | FRAME_READY_FLAG_UBX | FRAME_READY_FLAG_NMEA)

#define FRAME_READY_FLAG_YUV		(0x01)
#define FRAME_READY_FLAG_JPEG		(0x02)
#define FRAME_READY_FLAG_PREVIEW	(0x04)
#define FRAME_READY_FLAG_IMU		(0x08)
#define FRAME_READY_FLAG_UBX		(0x10)
#define FRAME_READY_FLAG_NMEA		(0x20)


struct DataExchangeFrame {
	int32_t metaOffset;
	int32_t yuvOffset;
	int32_t jpegOffset;
	int32_t previewOffset;

	int32_t imuOffset;
	int32_t imuSize;

	int32_t nmeaOffset;
	int32_t nmeaSize;

	int32_t ubxOffset;
	int32_t ubxSize;

	uint32_t flags;
};

struct DataExchangeHeader {
	DataExchangeBufInfo buf;
	DataExchangeConfig config;

	DataExchangeBufInfo frames;
	DataExchangeBufInfo metas;

	DataExchangeBufInfo yuvFrames;
	DataExchangeBufInfo jpegFrames;
	DataExchangeBufInfo previewFrames;

	DataExchangeBufInfo imuFrames;
	DataExchangeBufInfo nmeas;
	DataExchangeBufInfo ubxs;

	DataExchangeBufInfo infos;
	DataExchangeBufInfo raws;

	DataExchangeBufInfo xvs;
	DataExchangeBufInfo pps;

	DataExchangeBufInfo gnssRaws;
	DataExchangeBufInfo rtcmRaws;
};

#pragma pack()


#endif
