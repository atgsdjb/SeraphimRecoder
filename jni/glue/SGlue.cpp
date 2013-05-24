extern"C"{
#include<stdio.h>
#include<stdint.h>
#include"pthread.h"
};
#include<iostream>
#include<map>
#include"STrackParam.h"
#include"context.h"
#include"SMp4Creater.h"
extern"C"{
#include"x264.h"
#include"../aac/faac.h"
#include"../aac/faaccfg.h"
};
using namespace std;
using namespace Seraphim;

SEncoderContext *context=0;
uint8_t *g_PPS;
uint8_t *g_SPS;
size_t g_lenPPS;
size_t g_lenSPS;

/************************************************************************/
/*                                                                      */
/************************************************************************/

void initAAC(int trackIndex,STrackParam* param){
	//default get trackID =1 ;
	STrackParam *trackParam = context->idAndParm[1];// h->paramS[1];
	//if(trackParam->type ==1){
		//SAudioTrackParam *audioP = (SAudioTrackParam*)trackParam;
		//unsigned long sampleRate = 44100;//audioP->sampleRate;
		//unsigned long bitRate = 32*1024;//audioP->bitRate;
		//int numChannels = 1;
		//unsigned long inputBuffSize;
		//unsigned long outBuffSize;
		//context->aacHandler = faacEncOpen(sampleRate,numChannels,&inputBuffSize,&outBuffSize);
		//context->pcmBuf = new SSyncBuffer;
		/*faacEncConfigurationPtr conf = faacEncGetCurrentConfiguration(context->aacHandler);
		conf->bitRate = bitRate;
		conf->inputFormat = FAAC_INPUT_16BIT;
		conf->mpegVersion = MPEG4;
		conf->aacObjectType == LOW;
		faacEncSetConfiguration(context->aacHandler,conf);*/
	//}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
static void initParam(x264_param_t* pX264Param,SVideoTrackParm* videoParam){
	pX264Param->i_threads = X264_SYNC_LOOKAHEAD_AUTO;	//* 取空缓冲区继续使用不死锁的保证.//* video Properties
	pX264Param->i_frame_total = 0;						//* 编码总帧数.不知道用0.
	pX264Param->i_keyint_max = 10;	
	pX264Param->i_bframe = 5;
	pX264Param->b_open_gop = 0;
	pX264Param->i_bframe_pyramid = 0;
	pX264Param->i_bframe_adaptive = X264_B_ADAPT_TRELLIS;
	pX264Param->b_annexb = 1;
	pX264Param->i_log_level = X264_LOG_NONE;
	pX264Param->rc.i_bitrate = videoParam->bitRate;//1024 * 500;				//* 码率(比特率,单位Kbps), muxing parameters
	pX264Param->i_fps_den = 1;							//* 帧率分母
	pX264Param->i_fps_num = 25;							//* 帧率分子
	pX264Param->i_timebase_den = pX264Param->i_fps_num;
	pX264Param->i_timebase_num = pX264Param->i_fps_den;
	pX264Param->i_width = videoParam->width;
	pX264Param->i_height=videoParam->height;
	//pX264Param->i_threads = X264_SYNC_LOOKAHEAD_AUTO;	//* 取空缓冲区继续使用不死锁的保证.//* video Properties
	//pX264Param->i_frame_total = 0;						//* 编码总帧数.不知道用0.
	//pX264Param->i_keyint_max = 10;	
	//pX264Param->i_bframe = 5;
	//pX264Param->b_open_gop = 0;
	//pX264Param->i_bframe_pyramid = 0;
	//pX264Param->i_bframe_adaptive = X264_B_ADAPT_TRELLIS;
	//pX264Param->b_annexb = 1;
	//pX264Param->i_log_level = X264_LOG_NONE;
	//pX264Param->rc.i_bitrate = 1024 * 500;				//* 码率(比特率,单位Kbps), muxing parameters
	//pX264Param->i_fps_den = 1;							//* 帧率分母
	//pX264Param->i_fps_num = 25;							//* 帧率分子
	//pX264Param->i_timebase_den = pX264Param->i_fps_num;
	//pX264Param->i_timebase_num = pX264Param->i_fps_den;
	//pX264Param->i_width = width;
	//pX264Param->i_height=height;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

void initAVC(int trackIndex,SVideoTrackParm* videoParam){
	//default trackId = 0;
	x264_param_t *param= new x264_param_t;
	x264_param_default(param);
	//initParam(param,352,288);
	initParam(param,videoParam);
	x264_t* pX264Handle = x264_encoder_open (param);
	context->x264Handler = pX264Handle;
	x264_param_apply_profile (param, x264_profile_names[1]);
	Yuv420 *yuv = new Yuv420(352,288,0,"D:\\1video\\test_w352_h288_f2000.yuv");
	context->x264Handler = pX264Handle;
	context->x264Param = param;
	context->yuvData = yuv;
	return ;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
#include<vector>
void init(char* baseName,uint8_t countTrack,vector<STrackParam*> paramS,bool videoSoftEncoded,bool audioSoftEncoded){
	g_lenPPS=-1;
	g_lenSPS=-1;
	g_PPS = 0;
	g_SPS = 0;
	context = new SEncoderContext;
	//audioId = 1, videoId =  0;
	context->baseName = "d:\\1video\\%4d.mp4";
	context->countTrack = countTrack;
	context->runing = true;
	for(int i =0;i<countTrack;i++){
		context->idAndBuf[i] = new SSyncBuffer;
		context->idAndNSample[i] = 0;
		context->idAndParm[i] =paramS[i];
		if(audioSoftEncoded && context->idAndParm[i]->type==1){
			initAAC(i,context->idAndParm[i]);
			pthread_t aTid;
			pthread_create(&aTid,NULL,aacTask,NULL);
		}
		if(videoSoftEncoded && context->idAndParm[i]->type == 0){
			SVideoTrackParm* param = (SVideoTrackParm*) context->idAndParm[i];
			initAVC(i,param);
			pthread_t vTid;
			pthread_create(&vTid,NULL,avcTask,NULL);
		}
	}
	pthread_t tid ;
	pthread_create(&tid,NULL,workTask,NULL);
}
void addSample(uint8_t trackId,uint8_t* sample,size_t len){

}


int main(int argc,char** argv){
	char baseName[]="d:\\1video\\%04d.mp4";
	STrackParam *v_param = new SVideoTrackParm(90000,352,288,1024 * 500,25,120*90000);
	STrackParam *a_param = new SAudioTrackParam(44100,128*1024,44100,120 * 44100);
	vector<STrackParam*> paramVect ;
	paramVect.push_back(v_param);
	paramVect.push_back(a_param);
	init(baseName,2,paramVect,true,true);
	pthread_t tid ;
	context->pcmBuf = new SSyncBuffer;
	pthread_create(&tid,NULL,readPCM,NULL);
	printf("-----seraphim--------\n");
	
	int i;

	cin>>i;
	return 0;
}