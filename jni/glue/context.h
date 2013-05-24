#ifndef  __SERAPHIM_SGLUE_H
#define  __SERAPHIM_SGLUE_H
extern"C"{
#include<stdint.h>
#include"x264.h"
#include"../aac/faac.h"
#include"../aac/faaccfg.h"
};
#include<map>
#include"STrackParam.h"
#include"SSyncBuf.h"
#include"yuv420.h"
using namespace  Seraphim;
#define AVC_SAMPLE_BUFFER_SIZE  1024 *500
typedef struct{
	char* baseName;
	uint8_t countTrack;
	uint64_t duration;
	std::map<uint8_t,STrackParam*> idAndParm;
	std::map<uint8_t,uint64_t>     idAndNSample;
	std::map<uint8_t,SSyncBuffer*> idAndBuf;
	//
	SSyncBuffer* pcmBuf;
	void* aacHandler;
	//
	SSyncBuffer* yuvBuf;
	Yuv420* yuvData;
	x264_t * x264Handler;
	x264_param_t *x264Param;
	bool runing;
}SEncoderContext;
extern size_t g_lenPPS;
extern size_t g_lenSPS;
extern uint8_t* g_PPS;
extern uint8_t* g_SPS;
extern SEncoderContext* context;

void* workTask(void*);
void* aacTask(void*);
void* avcTask(void*);
void* readPCM(void* /*pram*/);

#endif