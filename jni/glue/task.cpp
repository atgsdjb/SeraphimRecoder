#include"context.h" 
#include"SMp4Creater.h"
#include"SSyncBuf.h"
#include"STrackParam.h"
#include"us_log.h"
extern "C"{
#include<stdio.h>
#include<string.h>

}
using namespace Seraphim;
const char* path="/mnt/sdcard/seraphim/";
const char* lg_baseName="/mnt/sdcard/seraphim/E3DAFD9C-0000-0000-FFFF-FFFFFFFFFF14_%d.mp4";
void* workTask(void* param){
	td_printf("start  workTask   ---------\n");
	static bool firstFile = true;
	char baseName[64]={0};
	char allName[128] ={0};
	char* guid = context->guid;
	memcpy(baseName,(const char*)guid,strlen((const char*)guid));
	strcat(baseName,"_%d.mp4");
	memcpy(allName,path,strlen(path));
	strcat(allName,baseName);
	int fileIndex = 1;
	char fileName[128]={0};
	//  wait  pps  sps
	do{
		//td_printf("--wiat pps sps---\n");
		sleep(1);
	}while(g_PPS == NULL || g_SPS==NULL );
	while(context->runing){
		char fileName[128]={0};
		sprintf(fileName,/*context->baseName */allName,fileIndex++);
	
		SMp4Creater creater(fileName,context->duration,context->idAndParm,context->idAndBuf,false);
		//creater.addVideoHead(0, g_PPS, g_lenPPS, g_SPS, g_lenSPS, !firstFile);
		creater.addPPS(g_PPS, g_lenPPS,0);
		creater.addSPS(g_SPS,g_lenSPS,0);
		td_printf("---start mp4 file -----%s--\n",fileName);
		creater.startEncode();
	
		if(firstFile){
			firstFile = false;
		}
		td_printf("---end   mp4 file ------%s----\n",fileName);
	}
	td_printf("-----workTask exit---------\n");
	return 0;

}

extern"C"{
#include<stdio.h>
}
 void* aacTask(void* /*param*/){
	unsigned long sampleRate = 44100;//audioP->sampleRate;
	unsigned long bitRate = 32*1024;//audioP->bitRate;
	int numChannels = 1;
	unsigned long inputBuffSize;
	unsigned long outBuffSize;
	faacEncHandle aacHandler = context->aacHandler;//faacEncOpen(sampleRate,numChannels,&inputBuffSize,&outBuffSize);

	/************************************************************************/
	faacEncConfigurationPtr conf = faacEncGetCurrentConfiguration(aacHandler);
	conf->bitRate = 32*1024;
	conf->inputFormat = FAAC_INPUT_16BIT;
	conf->mpegVersion = MPEG4;
	//conf->psymodelidx =1;
	conf->aacObjectType = LOW;
	faacEncSetConfiguration(aacHandler,conf);
		SSyncBuffer *pcmBuffer = context->pcmBuf ;
		uint8_t* pcm=new uint8_t[1024*2];

		SSyncBuffer *aacBuffer= context->idAndBuf[1];
		if(pcmBuffer == NULL ){
			td_printf("-------------pcmBuffer == NULL----------------\n");
			return 0;
		}
		if(aacBuffer ==NULL){
			td_printf("------aacBuffer ==NULL------------\n");
			return 0;
		}
		int len=-1;
		uint8_t *l_sample=new uint8_t[1024*2];
		size_t enc_len;
		int sampleIndex =0;
		while((len = pcmBuffer->read(&pcm)) != -1){
			if(len == 0)
				continue;
			int pcmOffset;
			enc_len = faacEncEncode(aacHandler,(int32_t*)(pcm),1024,l_sample,1024*2);
			if(enc_len <=0){
				continue;
			}
			uint8_t* l_b = new uint8_t[enc_len];
			memcpy(l_b,l_sample,enc_len);
			aacBuffer->write(l_b,enc_len);
			delete[] pcm;
		}
	td_printf("---aacTask exit---------------\n");
	return 0;
}
