
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <unistd.h>
#include <tgmath.h>
#include <iostream>
#include <Log.h>
#include "common.h"

#include "DataExchangeReader.h"

//void Handle_ImageYUVFrame(uint32_t frameIndex);

#define Hour_8_Microsecond 28800000000
//#define Hour_8_Microsecond 0

void split(const std::string &src, const std::string &sep,
                        std::vector<std::string> &ans) {
    std::string str = src;
    int nPos = 0;
    nPos = str.find(sep.c_str());
    while(-1 != nPos)
    {
        std::string temp = str.substr(0, nPos);
        ans.push_back(temp);
        str = str.substr(nPos+1);
        nPos = str.find(sep.c_str());
    }
    ans.push_back(str);

}

DataExchangeFrame* ReaderQueryFrameByOffset(void* buf, int32_t offset) {
	DataExchangeHeader* header = (DataExchangeHeader*)buf;
    if(header == NULL) {
        Log::info("ReaderQueryFrameByOffset", "header == NULL");
        return NULL;
    }
	DataExchangeFrame* result = NULL;
	if (header->frames.cur - offset > 20) {
//		Log::info("ReaderQueryFrameByOffset", "header->frames.cur - offset = %d", header->frames.cur - offset);
	}
//	PROCESS_ERROR(offset < header->frames.cur);
if(offset < header->frames.cur) {
    result = (DataExchangeFrame*)((char*)buf+header->frames.offset) + offset % header->frames.total;
} else {
    return NULL;
}

//Exit0:
	return result;
}


ImageExchangeMeta* ReaderQueryMetaByOffset(void* buf, int32_t offset) {
	DataExchangeHeader* header = (DataExchangeHeader*)buf;
	ImageExchangeMeta* result = NULL;

//	PROCESS_ERROR(offset < header->metas.cur);
    if(offset < header->metas.cur){
        result = (ImageExchangeMeta*)((char*)buf+header->metas.offset) + offset % header->metas.total;
    } else {
        return NULL;
    }

//Exit0:
	return result;
}

// 这里buf要添加上size todo lpfcheck
uint8_t* ReaderQueryImageYuvByOffset(void* buf, int32_t offset) {
	DataExchangeHeader* header = (DataExchangeHeader*)buf;
	uint8_t* result = NULL;

//	PROCESS_ERROR(offset < header->yuvFrames.cur);
if(offset < header->yuvFrames.cur){
    result = (uint8_t*)((char*)buf+header->yuvFrames.offset) + (offset % header->yuvFrames.total) * header->yuvFrames.itemSize;
} else {
    return NULL;
}
//Exit0:
	return result;
}

uint8_t* ReaderQueryImageJpegByOffset(void* buf, int32_t offset) {
	DataExchangeHeader* header = (DataExchangeHeader*)buf;
	uint8_t* result = NULL;

	PROCESS_ERROR(offset < header->jpegFrames.cur);
	result = (uint8_t*)((char*)buf+header->jpegFrames.offset) + (offset % header->jpegFrames.total) * header->jpegFrames.itemSize;
Exit0:
	return result;
}

uint8_t* ReaderQueryImagePreviewByOffset(void* buf, int32_t offset) {
	DataExchangeHeader* header = (DataExchangeHeader*)buf;
	uint8_t* result = NULL;

	PROCESS_ERROR(offset < header->previewFrames.cur);
	result = (uint8_t*)((char*)buf+header->previewFrames.offset) + (offset % header->previewFrames.total) * header->previewFrames.itemSize;

Exit0:
	return result;
}


IMUFloatData* ReaderQueryImuByOffset(void* buf, int32_t offset) {
	DataExchangeHeader* header = (DataExchangeHeader*)buf;
	IMUFloatData* result = NULL;

//	PROCESS_ERROR(offset < header->imuFrames.cur);
if(offset < header->imuFrames.cur){
    result = (IMUFloatData*)((char*)buf+header->imuFrames.offset) + offset % header->imuFrames.total;
} else {
    return NULL;
}

//Exit0:
	return result;
}

NmeaSentenceBuf* ReaderQueryNmeaSentenceByOffset(void* buf, int32_t offset) {
	DataExchangeHeader* header = (DataExchangeHeader*)buf;
	NmeaSentenceBuf* result = NULL;

//	PROCESS_ERROR(offset < header->nmeas.cur);
if(offset < header->nmeas.cur) {
    result = (NmeaSentenceBuf*)((char*)buf+header->nmeas.offset) + offset % header->nmeas.total;
}else{
    return NULL;
}

//Exit0:
	return result;
}

UbxSentenceBuf* ReaderQueryUbxSentenceByOffset(void* buf, int32_t offset) {
	DataExchangeHeader* header = (DataExchangeHeader*)buf;
	UbxSentenceBuf* result = NULL;

	PROCESS_ERROR(offset < header->ubxs.cur);
	result = (UbxSentenceBuf*)((char*)buf+header->ubxs.offset) + offset % header->ubxs.total;

Exit0:
	return result;
}


TSInfoBuf* ReaderQueryXVSByOffset(void* buf, int32_t offset) {
	DataExchangeHeader* header = (DataExchangeHeader*)buf;
	TSInfoBuf* result = NULL;

	PROCESS_ERROR(offset >= 0);
	PROCESS_ERROR(offset < header->xvs.cur);

	result = (TSInfoBuf*)((char*)buf+header->xvs.offset) + offset % header->xvs.total;

Exit0:
	return result;
}

TSInfoBuf* ReaderQueryPPSByOffset(void* buf, int32_t offset) {
	DataExchangeHeader* header = (DataExchangeHeader*)buf;
	TSInfoBuf* result = NULL;

	PROCESS_ERROR(offset >= 0);
	PROCESS_ERROR(offset < header->pps.cur);

	result = (TSInfoBuf*)((char*)buf+header->pps.offset) + offset % header->pps.total;

Exit0:
	return result;
}

InfoSentenceBuf* ReaderQueryInfoSentenceByOffset(void* buf, int32_t offset) {
	DataExchangeHeader* header = (DataExchangeHeader*)buf;
	InfoSentenceBuf* result = NULL;

	PROCESS_ERROR(offset >= 0);
	PROCESS_ERROR(offset < header->infos.cur);

	result = (InfoSentenceBuf*)((char*)buf+header->infos.offset) + offset % header->infos.total;

Exit0:
	return result;
}

RawSentenceBuf* ReaderQueryRawSentenceByOffset(void* buf, int32_t offset) {
	DataExchangeHeader* header = (DataExchangeHeader*)buf;
	RawSentenceBuf* result = NULL;

	PROCESS_ERROR(offset >= 0);
	PROCESS_ERROR(offset < header->raws.cur);

	result = (RawSentenceBuf*)((char*)buf+header->raws.offset) + offset % header->raws.total;

Exit0:
	return result;
}


GnssRawBuf* ReaderQueryGnssRawByOffset(void* buf, int32_t offset) {
	DataExchangeHeader* header = (DataExchangeHeader*)buf;
	GnssRawBuf* result = NULL;

	PROCESS_ERROR(offset >= 0);
	PROCESS_ERROR(offset < header->gnssRaws.cur);

	result = (GnssRawBuf*)((char*)buf+header->gnssRaws.offset) + offset % header->gnssRaws.total;

Exit0:
	return result;
}

GnssRawBuf* ReaderQueryRtcmRawByOffset(void* buf, int32_t offset) {
	DataExchangeHeader* header = (DataExchangeHeader*)buf;
	GnssRawBuf* result = NULL;

	PROCESS_ERROR(offset >= 0);
	PROCESS_ERROR(offset < header->rtcmRaws.cur);

	result = (GnssRawBuf*)((char*)buf+header->rtcmRaws.offset) + offset % header->rtcmRaws.total;

Exit0:
	return result;
}

void HanderReaderFramebuf(int32_t frameIndex, void* buf, std::shared_ptr<YGIData> ygiData) {

//    Log::info("HanderReaderFramebuf", "frameIndex = %d", frameIndex);

    if (ygiData.get() == nullptr){
        Log::info("HanderReaderFramebuf", "ygiData == nullptr");
        return;
    }
	int imuCount = 0;
	int nmeaCount = 0;
	int ubxCount = 0;
    DataExchangeHeader* header = (DataExchangeHeader*)buf;
	DataExchangeFrame* frameInfo = NULL;
	ImageExchangeMeta* meta = NULL;
    uint8_t* yuv = NULL;
    uint8_t* subsampled = NULL;
    /*Get frame info*/

    frameInfo = ReaderQueryFrameByOffset(buf, frameIndex);

    if(frameInfo == NULL) {
        Log::info("HanderReaderFramebuf", "ReaderQueryFrameByOffset, frameInfo == null");
    }
//    PROCESS_ERROR(frameInfo);
    if(frameInfo == NULL){
        return;
    }

//    Log::info("HanderReaderFramebuf", "ReaderQueryMetaByOffset");

    /*Get camera image meta info*/
	meta = ReaderQueryMetaByOffset(buf, frameInfo->metaOffset);
//	PROCESS_ERROR(meta);
    if(meta == NULL){
        return;
    }

	if (meta){
        ygiData->getSmartFrame()->setTimestamp(meta->ts + Hour_8_Microsecond);
    }

//    Log::info("HanderReaderFramebuf", "ReaderQueryImageYuvByOffset");

    /*Get camera yuv data*/
    yuv = ReaderQueryImageYuvByOffset(buf, frameInfo->yuvOffset);
//    PROCESS_ERROR(yuv);
if(yuv == NULL){
    return;
}

    if (yuv){
        int height = header->config.imageConfig[0].height;
        int width = header->config.imageConfig[0].width;
        ygiData->getSmartFrame()->setHeight(height);
        ygiData->getSmartFrame()->setWidth(width);
        int length = width * height * 3/2;
        ygiData->getSmartFrame()->setLength(length);
        // 还是需要拷贝一份 因为马上转成了jpeg 所以暂时不考虑复制一份
       // char *newData = new char[length];
       // memcpy(newData, yuv, length);
       if(ygiData->getSmartFrame() == NULL){
           Log::info("HanderReaderFramebuf", "ygiData->getSmartFrame() == NULL");
       } else {
           char  *newData = new char[length];
           memcpy(newData, yuv, length);
           ygiData->getSmartFrame()->setFrameData((unsigned char *)newData);
       }
    }

    /*Print camera meta info */
	if (meta) {

//		LOGI("ts\tintegration_us\tline_delay_us\r\n%lu\t%f\t%f\t\r",
//			meta->ts, meta->integrationInUs, meta->lineExposureDelayInUs);
		ygiData->getSmartFrame()->integration = meta->integrationInUs;
		ygiData->getSmartFrame()->lineOfIntegration = meta->lineExposureDelayInUs;

    }
    /*Get imu data*/
	if ((frameInfo->imuOffset >= 0) && (frameInfo->imuSize > 0)) {

//		LOGI("source\tts\tgyro_x\tgyro_y\tgyro_z\taccel_x\taccel_y\taccel_z\ttemperature\r");

		for (int i = 0; i < MIN(frameInfo->imuSize, 60); i++) {
			IMUFloatData* imuData = ReaderQueryImuByOffset(buf, frameInfo->imuOffset + i);
			SmartIMU *smartImu = nullptr;
			if (imuData) {
                smartImu = new SmartIMU();
//				LOGI(
//					"%d\t%lu\t%f\t%f\t%f\t%f\t%f\t%f\t%f\r",
//					imuData->source, imuData->ts,
//					imuData->gyro[0], imuData->gyro[1], imuData->gyro[2],
//					imuData->accel[0], imuData->accel[1], imuData->accel[2],
//					imuData->temp);
                smartImu->setTimestamp(imuData->ts + Hour_8_Microsecond);
                smartImu->setGyroX(imuData->gyro[0] * 0.0175);
                smartImu->setGyroY(imuData->gyro[1] * 0.0175);
                smartImu->setGyroZ(imuData->gyro[2] * 0.0175);
                smartImu->setAccelX(imuData->accel[0] * 0.00059820565);
                smartImu->setAccelY(imuData->accel[1] * 0.00059820565);
                smartImu->setAccelZ(imuData->accel[2] * 0.00059820565);
                ygiData->getImuVector()->push_back(smartImu);
			}
		}

	}

   /*Get GPS nmea data*/
//	LOGI("nmea: begin11111....");

	if ((frameInfo->nmeaOffset >= 0) && (frameInfo->nmeaSize > 0)) {
        if (ygiData->getGpsVector() == nullptr){
			ygiData->setGpsVector(new std::vector<SmartGPS*>());
		}
        SmartGPS *smartGps = nullptr;
//        LOGI("nmea: begin....");
        for (int i = 0; i < MIN(frameInfo->nmeaSize, 10); i++) {
            NmeaSentenceBuf* item = ReaderQueryNmeaSentenceByOffset(buf, frameInfo->nmeaOffset + i);
            if (item == nullptr){
                continue;
            }
//			LOGI("nmea: %s", (char*)item->buf);

            // $GNGGA,074721.60,4000.18618,N,11627.68679,E,4,12,0.77,36.3,M,-8.3,M,0.6,3340*4E
            if (strstr((char*)item->buf, "GGA") != NULL){
                if (smartGps == nullptr){
                    smartGps = new SmartGPS();
                }
                std::string gnggaStr = (char*)item->buf;
                // 室内环境用测试数据模拟
//                gnggaStr = "$GNGGA,074721.60,4000.18618,N,11627.68679,E,4,12,0.77,36.3,M,-8.3,M,0.6,3340*4E";
				smartGps->setTimestamp(item->ts + Hour_8_Microsecond);
//                LOGI("gnggaStr %s", gnggaStr.c_str());

				// 获取系统运行的时长
				struct timespec ts;
				clock_gettime(CLOCK_BOOTTIME, &ts);
				long long bootTime = (long long)ts.tv_sec * 1000 + (long long)(ts.tv_nsec/1000000);
				smartGps->bootTime = bootTime * 1000;

                parseGNGGA(gnggaStr, smartGps);
            }else if (strstr((char*)item->buf, "RMC") != NULL){
                // $GNRMC,073248.10,A,4000.27857,N,11628.33741,E,13.366,179.58,170820,,,A,V*3B
                if (smartGps == nullptr){
                    smartGps = new SmartGPS();
                }
                std::string gpgstStr = (char*)item->buf;
                parseGNRMC(gpgstStr, smartGps);
			} else if (strstr((char *) item->buf, "GST") != NULL) {
				// $GPGST,0246033.00,3.2,6.6,4.7,47.3,5.8,5.6,22.0*58
				if (smartGps == nullptr){
					smartGps = new SmartGPS();
				}
				std::string gpgstStr = (char*)item->buf;
				parseGPGST(gpgstStr, smartGps);

			} else if( strstr((char*) item->buf, "COV") != NULL) {

				if (smartGps == nullptr){
					smartGps = new SmartGPS();
				}
				std::string wzcovStr = (char*)item->buf;
				parseWZCOV(wzcovStr, smartGps);

            }
		}
        if (smartGps != nullptr){
            ygiData->getGpsVector()->push_back(smartGps);
        }
//        LOGI("nmea: end....");

	}


//Exit0:
	return;
}

void parseGNGGA(const std::string &gnggaStr, SmartGPS *smartGps) {

//    Log::info("GPS", "gngga = %s", gnggaStr.c_str());
	std::vector<std::string> vec;
	split(gnggaStr, ",", vec);
	if (vec.size() < 12){
		return;
	}
//    $GPGGA,085407.70,3959.539422,N,11628.036066,E,1,03,22.2,70.511,M,0.000,M,,*52
	for (int i = 2; i < 10; ++i) {
		switch (i){
			// 纬度 y
			case 2: {
				try {
					if (strcmp("", vec.at(i).c_str()) == 0){
						smartGps->setLat(999999);
					} else {
						double y = std::stod(vec.at(i));
						smartGps->setLat((int)y/100 + fmod(y, 100)/60.0);
					}
				} catch (...) {
					smartGps->setLat(999999);
				}

				break;
			}
				// 经度 x
			case 4: {
				try {
					if (strcmp("", vec.at(i).c_str()) == 0){
						smartGps->setLng(999999);
					} else {
						double x = std::stod(vec.at(i));
						smartGps->setLng((int)x/100 + fmod(x, 100)/60.0);
					}
				} catch (...) {
					smartGps->setLng(999999);
				}

				break;
			}
				// gps定位的状态
			case 6: {
				try {
					if (strcmp("", vec.at(i).c_str()) == 0){
						smartGps->setGpsStatus(999999);
					} else {
						int gpsStatus = std::stoi(vec.at(i));
						smartGps->setGpsStatus(gpsStatus);
					}
				} catch (...) {
					smartGps->setGpsStatus(999999);
				}

				break;
			}
				// 卫星的数量
			case 7: {
				try {
					if (strcmp("", vec.at(i).c_str()) == 0){
						smartGps->setGpsNum(999999);
					} else {
						int gpsNumber = std::stoi(vec.at(i));
						smartGps->setGpsNum(gpsNumber);
					}
				} catch (...) {
					smartGps->setGpsNum(999999);
				}
				break;
			}
				// HDOP
			case 8: {
				try {
					if (strcmp("", vec.at(i).c_str()) == 0){
						smartGps->setHdop(999999);
					} else {
						double hdop = std::stof(vec.at(i));
						smartGps->setHdop(hdop);
					}
				} catch (...) {
					smartGps->setHdop(999999);
				}
				break;
			}
				// 高度
			case 9: {
				try {
					if (strcmp("", vec.at(i).c_str()) == 0){
						smartGps->setAltitude(999999);
					} else {
						double height = std::stof(vec.at(i));
						double geoidSep = std::stof(vec.at(i + 2));
						smartGps->setAltitude(height + geoidSep);
					}
				} catch (...) {
					smartGps->setAltitude(999999);
				}
				break;
			}
			default:{
				break;
			}
		}
	}


}
void parseGNRMC(const std::string &gpgstStr, SmartGPS *smartGps) {
	std::vector<std::string> vec;
	split(gpgstStr, ",", vec);
	if (vec.size() < 9){
		return;
	}
	for (int i = 7; i < 9; ++i) {
		switch (i){
			// 速度
			case 7: {
				try {
					if (strcmp("", vec.at(i).c_str()) == 0){
						smartGps->setSpeed(999999);
					} else {
						double speed = std::stof(vec.at(i));
						// 海里转成 m/s
						smartGps->setSpeed(speed * 0.514444);
					}
				} catch (...) {
					// catch 块
					smartGps->setSpeed(999999);
				}
				break;
			}
			// 航向
			case 8: {
				try {
					if (strcmp("", vec.at(i).c_str()) == 0){
						smartGps->setBearing(999999);
					} else {
						double course = std::stof(vec.at(i));
						smartGps->setBearing(course);
					}
				} catch (...) {
					// catch 块
					smartGps->setBearing(999999);
				}
				break;
			}
			default:{
				break;
			}
		}
	}
}

void parseGPGST(const std::string &gpgstStr, SmartGPS *smartGps) {
	std::vector<std::string> vec;
	split(gpgstStr, ",", vec);

	// $GPGST,085407.80,0.000,0.000,0.000,0.0,0.000,0.000,0.000*51
//	for(auto& item : vec){
//		Log::info("parseGPGST:item", "%s", item.c_str());
//	}

	if (vec.size() < 9){
		return;
	}
	for (int i = 6; i < 9; ++i) {
		switch (i){
			case 6: {
				double latErrorDeviation = 0.0;
				try {
					if (strcmp("", vec.at(i).c_str()) == 0){
						latErrorDeviation = 999999;
					} else {
						latErrorDeviation = std::stof(vec.at(i));
					}
				} catch (...) {
					// catch 块
					latErrorDeviation = 999999;
				}
				smartGps->setLatErrorDeviation(latErrorDeviation);
				break;
			}
			case 7: {
				double lngErrorDeviation = 0.0;

				try {
					if (strcmp("", vec.at(i).c_str()) == 0){
						lngErrorDeviation = 999999;
					} else {
						lngErrorDeviation = std::stof(vec.at(i));
					}
				} catch (...) {
					// catch 块
					lngErrorDeviation = 999999;
				}

				smartGps->setLngErrorDeviation(lngErrorDeviation);
				break;
			}
			case 8: {

				std::vector<std::string> subVec;
				split(vec.at(i), "*", subVec);

				if(!subVec.empty() && !subVec[0].empty()){
					double altErrorDeviation = 0.0;
					try {
						altErrorDeviation = std::stof(subVec.at(0));
					} catch (...) {
						// catch 块
						altErrorDeviation = 999999;
					}

					smartGps->setAltErrorDeviation(altErrorDeviation);
				} else {
					smartGps->setAltErrorDeviation(999999);
					Log::info("parseGPGST", "最后一个元素为空");
				}

				break;
			}
			default:{
				break;
			}
		}
	}
}

void parseWZCOV(const std::string &wzcovStr, SmartGPS *smartGps) {
	std::vector<std::string> vec;
	split(wzcovStr, ",", vec);

//	for (auto &item : vec) {
//		Log::info("parseWZCOV:item", "%s", item.c_str());
//	}

	if (vec.size() < 14) {
		return;
	}
	for (int i = 2; i < 14; ++i) {
		switch (i) {
			case 2: {
				double Cxx = 0.0;
				try {
					if (strcmp("", vec.at(i).c_str()) == 0){
						Cxx = 999999;
					} else {
						Cxx = std::stof(vec.at(i));
					}
				} catch (...) {
					// catch 块
					Cxx = 999999;
				}

				smartGps->setCxx(Cxx);
				break;
			}
			case 3: {
				double Cxy = 0.0;

				try {
					if (strcmp("", vec.at(i).c_str()) == 0){
						Cxy = 999999;
					} else {
						Cxy = std::stof(vec.at(i));
					}
				} catch (...) {
					// catch 块
					Cxy = 999999;
				}

				smartGps->setCxy(Cxy);
				break;
			}
			case 4: {
				double Cxz = 0.0;
				try {
					if (strcmp("", vec.at(i).c_str()) == 0){
						Cxz = 999999;
					} else {
						Cxz = std::stof(vec.at(i));
					}
				} catch (...) {
					// catch 块
					Cxz = 0.0;
				}

				smartGps->setCxz(Cxz);
				break;
			}
			case 5: {
				double Cyy = 0.0;

				try {
					if (strcmp("", vec.at(i).c_str()) == 0){
						Cyy = 999999;
					} else {
						Cyy = std::stof(vec.at(i));
					}

				} catch (...) {
					// catch 块
					Cyy = 999999;
				}
				smartGps->setCyy(Cyy);
				break;
			}
			case 6: {
				double Cyz = 0.0;

				try {
					if (strcmp("", vec.at(i).c_str()) == 0){
						Cyz = 999999;
					} else {
						Cyz = std::stof(vec.at(i));
					}
				} catch (...) {
					// catch 块
					Cyz = 999999;
				}
				smartGps->setCyz(Cyz);
				break;
			}
			case 7: {
				double Czz = 0.0;
				try {
					if (strcmp("", vec.at(i).c_str()) == 0){
						Czz = 999999;
					} else {
						Czz = std::stof(vec.at(i));
					}
				} catch (...) {
					// catch 块
					Czz = 999999;
				}
				smartGps->setCzz(Czz);
				break;
			}
			case 8: {
				double Vxx = 0.0;

				try {
					if (strcmp("", vec.at(i).c_str()) == 0){
						Vxx = 999999;
					} else {
						Vxx = std::stof(vec.at(i));
					}
				} catch (...) {
					// catch 块
					Vxx = 999999;
				}

				smartGps->setVxx(Vxx);
				break;
			}
			case 9: {
				double Vxy = 0.0;

				try {
					if (strcmp("", vec.at(i).c_str()) == 0){
						Vxy = 999999;
					} else {
						Vxy = std::stof(vec.at(i));
					}
				} catch (...) {
					// catch 块
					Vxy = 0.0;
				}

				smartGps->setVxy(Vxy);
				break;
			}
			case 10: {
				double Vxz = 0.0;

				try {
					if (strcmp("", vec.at(i).c_str()) == 0){
						Vxz = 999999;
					} else {
						Vxz = std::stof(vec.at(i));
					}
				} catch (...) {
					// catch 块
					Vxz = 999999;
				}

				smartGps->setVxz(Vxz);
				break;
			}
			case 11: {
				double Vyy = 0.0;

				try {
					if (strcmp("", vec.at(i).c_str()) == 0){
						Vyy = 999999;
					} else {
						Vyy = std::stof(vec.at(i));
					}
				} catch (...) {
					// catch 块
					Vyy = 999999;
				}

				smartGps->setVyy(Vyy);
				break;
			}
			case 12: {
				double Vyz = 0.0;

				try {
					if (strcmp("", vec.at(i).c_str()) == 0){
						Vyz = 999999;
					} else {
						Vyz = std::stof(vec.at(i));
					}
				} catch (...) {
					// catch 块
					Vyz = 999999;
				}
				smartGps->setVyz(Vyz);
				break;
			}
			case 13: {

				std::vector<std::string> subVec;
				split(vec.at(i), "*", subVec);

				if(!subVec.empty() && !subVec[0].empty()){

					double Vzz = 0.0;
					try {
						Vzz = std::stof(subVec.at(0));
					} catch (...) {
						// catch 块
						Vzz = 999999;
					}

					smartGps->setVzz(Vzz);
				} else {
					smartGps->setVzz(999999);
					Log::info("parseWZCOV", "最后一个元素为空");
				}
				break;

			}
			default: {
				break;
			}
		}
	}
}


static int32_t FirstValidIndexInReaderFifo(DataExchangeBufInfo* info) {
	int32_t result = -1;

	PROCESS_ERROR(info->cur > 0);

	result = info->cur - (info->total - 1);
	if (result < 0)
		result = 0;

Exit0:
	return result;
}


int32_t FirstReaderFrameReady(void *buf) {
	int32_t result = -1;
	int32_t start = 0;
    DataExchangeHeader* header = (DataExchangeHeader*)buf;  
    uint32_t flag = FRAME_READY_FLAG_ESSENTIAL;

    start = FirstValidIndexInReaderFifo(&header->frames);
	PROCESS_ERROR(start >= 0);

	for (int32_t i = start ; i < header->frames.cur; i++) {

		DataExchangeFrame* frame = ReaderQueryFrameByOffset(buf, i);
		PROCESS_ERROR(frame);

		if (flag == (frame->flags & flag)) {
				result = i;
				break;
		}
	}

Exit0:
	return result;
}

int32_t LastReaderFrameReady(void *buf) {
	int32_t result = -1;
	int32_t start = 0;

    DataExchangeHeader* header = (DataExchangeHeader*)buf;
	uint32_t flag = FRAME_READY_FLAG_ESSENTIAL;

    start = FirstValidIndexInReaderFifo(&header->frames);
	PROCESS_ERROR(start >= 0);

	for (int32_t i = header->frames.cur - 1; i >= start; i--) {

		DataExchangeFrame* frame = ReaderQueryFrameByOffset(buf, i);
		PROCESS_ERROR(frame);

		if (flag == (frame->flags & flag)) {
				result = i;
				break;
			}
	}

Exit0:
	return result;
}

/*Example for polling mode*/
void ReaderPollingFrameReadyData(void* buf) {
    int32_t itemToProc;
    DataExchangeHeader* header = (DataExchangeHeader*)buf;

    while (1) {

		itemToProc = LastReaderFrameReady(buf);
		if (itemToProc < 0) {
			usleep(30000);
			continue;
		}
        ALOGW("ReaderPollingFrameData1 =%d", itemToProc);
		while (1) {
			DataExchangeFrame* frame = NULL;

			if (itemToProc >= header->frames.cur) {
				usleep(10000);
				continue;
			}
            ALOGW("ReaderPollingFrameData2 =%d:%d:%d", itemToProc,header->frames.cur,FirstValidIndexInReaderFifo(&header->frames));
			if (itemToProc < FirstValidIndexInReaderFifo(&header->frames)) {
				break;
			}

			frame = ReaderQueryFrameByOffset(buf, itemToProc);
			if (!frame) {
				break;
			}
            ALOGW("ReaderPollingFrameData3 =%d", itemToProc);
			if (FRAME_READY_FLAG_ESSENTIAL != (frame->flags & FRAME_READY_FLAG_ESSENTIAL)) {
				usleep(10000);
				continue;
			}

			ALOGW("Handler frame: %d", itemToProc);
			// 输出ygi数据
            HanderReaderFramebuf(itemToProc, buf, nullptr);
			itemToProc++;
		}
	}

	return;
}
int32_t ReaderLogDataFromDataExchange(void* buf, int32_t start, int32_t end) {

    DataExchangeHeader* header = (DataExchangeHeader*)buf;
    if (start < FirstValidIndexInReaderFifo(&header->infos)) {
        start = FirstValidIndexInReaderFifo(&header->infos);
    }

    if (end > header->infos.cur -1 ) {
        end = header->infos.cur -1;
    }

    return end + 1;
}

/*Example for polling mode*/
void ReaderPollingFrameData(void* buf) {
	int32_t itemToProc;
	int32_t ImageFrameIndex = -1;
	int32_t ImuFrameIndex = 0;
	int32_t nmeaFrameIndex = 0;
    int32_t infoFrameIndex = 0;
	DataExchangeHeader* header = (DataExchangeHeader*)buf;

	while (1) {

		DataExchangeFrame* frameInfo = NULL;
		uint8_t* yuv = NULL;
		//query camera image
		if (header->frames.cur > 0) {
			if(ImageFrameIndex != header->frames.cur-1) {
              //  Handle_ImageYUVFrame(ImageFrameIndex);
				ImageFrameIndex = header->frames.cur-1;
			}
		}
		//query imu
		if (header->imuFrames.cur > 0) {
			while (ImuFrameIndex < header->imuFrames.cur) {
				IMUFloatData* imuData = ReaderQueryImuByOffset(buf, ImuFrameIndex);
                /*
				if (imuData) {
					ALOGW(
							"IMU:%d\t%lu\t%f\t%f\t%f\t%f\t%f\t%f\t%f\r\n",
							imuData->source, imuData->ts,
							imuData->gyro[0], imuData->gyro[1], imuData->gyro[2],
							imuData->accel[0], imuData->accel[1], imuData->accel[2],
							imuData->temp);
				}*/
				ImuFrameIndex++;
			}
		}
		//query NMEA
		if (header->nmeas.cur > 0) {
			while  ( nmeaFrameIndex < header->nmeas.cur) {
				NmeaSentenceBuf* item = ReaderQueryNmeaSentenceByOffset(buf, nmeaFrameIndex);
				if (item) {
					//ALOGW("NMEA:%d\t%s\r\n", nmeaFrameIndex, item->buf);
				}
                nmeaFrameIndex++;
			}
		}
        //query info
        /*
        if (header->infos.cur > 0) {
            while(infoFrameIndex < header->infos.cur) {
                InfoSentenceBuf* item = ReaderQueryInfoSentenceByOffset(buf, infoFrameIndex);
                if (item) {
                    ALOGW("LOG:%s", item->buf);
                }
                infoFrameIndex++;
            }
        }*/
		usleep(1000);
	}

	return;
}
void ReaderPollingGnssRaw(void* buf) {

    DataExchangeHeader* header = (DataExchangeHeader*)buf;
	static int gnssRawIndex = header->gnssRaws.cur;
	// 因为smarteye重启后 会导致日志积累 第一次读取先舍弃掉 否则日志太大
	static bool isFirst = true;
    if (header->nmeas.cur <= 0) {
		return;
    }
    Log::info("ReaderPollingGnssRaw", "read gnssRaws.cur = %d", gnssRawIndex);
	while (gnssRawIndex < header->nmeas.cur) {
		RawSentenceBuf *item = ReaderQueryRawSentenceByOffset(buf, gnssRawIndex);
		if (item) {
			int len = item->size;
			if (len <= 0) {
				gnssRawIndex++;
				break;
			}
			if (!isFirst) {
				unsigned char data[len];
				memcpy(data, item->buf, len);
				Log::info("GNSSRaw", reinterpret_cast<const char *>(data), len);
			}
		}
		gnssRawIndex++;
	}
	Log::info("ReaderPollingGnssRaw", "readed gnssRaws.cur = %d", gnssRawIndex);
	isFirst = false;
}
void ReaderPollingNmeaData(void* buf) {

	DataExchangeHeader* header = (DataExchangeHeader*)buf;
	static int nmeaFrameIndex = header->nmeas.cur;
	if (header->nmeas.cur > 0) { //update GPS NMEA textView
		while (nmeaFrameIndex < header->nmeas.cur) {
			NmeaSentenceBuf *item = ReaderQueryNmeaSentenceByOffset(buf, nmeaFrameIndex);
			if (item) {
				Log::info("Nmea", reinterpret_cast<const char *>(item->buf));
			}
			nmeaFrameIndex++;
		}
	}
}