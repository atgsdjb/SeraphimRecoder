extern"C"{
#include<stdio.h>
#include<stdint.h>
#include"pthread.h"
};
#include<map>
#include"STrackParam.h"
#include"context.h"
#include"SMp4Creater.h"
extern"C"{
#include"../x264/x264.h"
#include"../aac/faac.h"
#include"../aac/faaccfg.h"
#include<jni.h>
#include"us_log.h"
};
#include"dl_tool.h"
#include<android/log.h>
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

void initAAC(){
	td_printf("---------initAAC-------\n");
	//default get trackID =1 ;
	unsigned long sampleRate = 44100;//audioP->sampleRate;
	unsigned long bitRate = 32*1024;//audioP->bitRate;
	int numChannels = 1;
	unsigned long inputBuffSize;
	unsigned long outBuffSize;
	context->aacHandler = faacEncOpen(sampleRate,numChannels,&inputBuffSize,&outBuffSize);
	context->pcmBuf = new SSyncBuffer;
	pthread_t aacTid;
	pthread_create(&aacTid,NULL,aacTask,NULL);
//	}
}
/**********************************************INTFACE */

extern "C" {
/*
 * Class:     com_seraphim_td_nativ_QMP4Creater
 * Method:    n_init
 * Signature: (ILjava/lang/String;I[Lcom/seraphim/td/omx/QTrackParam;)V
 */
JNIEXPORT void JNICALL Java_com_seraphim_td_nativ_QMP4Creater_n_1init
(JNIEnv *env, jobject obj, jint countSample, jstring baseName, jint countTrack, jobjectArray trackParamS){
	td_printf("----------------------------init ------0------------\n");
	int i = 0;
	jclass c_track = env->FindClass("com/seraphim/td/omx/QTrackParam");
	if(c_track == NULL){
		return ;
	}
	jfieldID f_t_type = env->GetFieldID(c_track,"type","I");
	td_printf("----------------------------init ------1------------\n");
	//QAudioTrakParam  class
	jclass c_aTrack = env->FindClass("com/seraphim/td/omx/QAudioTrackParam");
	jfieldID f_a_timeScale = env->GetFieldID(c_aTrack,"timeScale","I");
	jfieldID f_a_bitRate = env->GetFieldID(c_aTrack,"bitRate","I");
	jfieldID f_a_sampleRate = env->GetFieldID(c_aTrack,"sampleRate","I");
	jfieldID f_a_duration = env->GetFieldID(c_aTrack,"duration","I");
	jfieldID f_a_usedSoftEncode = env->GetFieldID(c_aTrack,"usedSoft","Z");
	//QVideoTrackParam class
	jclass c_vTrack = env->FindClass("com/seraphim/td/omx/QVideoTrackParam");
	td_printf("----------------------------init ------2------------\n");
	jfieldID f_v_timeScale = env->GetFieldID(c_vTrack,"timeScale","I");
	jfieldID f_v_width = env-> GetFieldID(c_vTrack,"width","I");
	jfieldID f_v_height = env->GetFieldID(c_vTrack,"height","I");
	jfieldID f_v_bitRate = env->GetFieldID(c_vTrack,"bitRate","I");
	jfieldID f_v_sampleRate = env->GetFieldID(c_vTrack,"sampleRate","I");
	jfieldID f_v_duration = env->GetFieldID(c_vTrack,"duration","I");
	jfieldID f_v_usedSoftEncode = env->GetFieldID(c_vTrack,"usedSoft","Z");
	context = new SEncoderContext;
	const char* l_str = env->GetStringUTFChars(baseName,JNI_FALSE);
	int l_sampleFile = countSample;
	int i2;
	for(i2 = 0;i2<countTrack;i2++){
		jobject obj = env->GetObjectArrayElement(trackParamS,i2);
		int type = env->GetIntField(obj,f_t_type);
		STrackParam* l_track;
		/**
		 *
		 */
		if(type == 0){
			int timeScale = env->GetIntField(obj,f_v_timeScale);
			int width = env->GetIntField(obj,f_v_width);
			int height = env->GetIntField(obj,f_v_height);
			int bitRate = env->GetIntField(obj,f_v_bitRate);
			int sampleRate = env->GetIntField(obj,f_v_sampleRate);
			int duration =env->GetIntField(obj,f_v_duration);
			bool usedSoft = env->GetBooleanField(obj,f_v_usedSoftEncode);
			STrackParam *v_param = new SVideoTrackParm(90000,352,288,1024 * 500,25,20);
			context->idAndParm[i2] = v_param;

		}else if(type == 1){
			int timeScale = env->GetIntField(obj,f_a_timeScale);
			int bitRate = env->GetIntField(obj,f_a_bitRate);
			int sampleRate = env->GetIntField(obj,f_a_sampleRate);
			int duration =env->GetIntField(obj,f_a_duration);
			bool usedSoft=env->GetBooleanField(obj,f_a_usedSoftEncode);

			l_track = new SAudioTrackParam(timeScale,bitRate,sampleRate,duration,usedSoft);
			if(usedSoft){
				initAAC();
			}
			STrackParam *a_param = new SAudioTrackParam(441000,32*1024,44100,20 );
			context->idAndParm[i2] = a_param;
		}else{
			//ERROR
		}
		context->idAndBuf[i2] = new SSyncBuffer;
	}
	char* name = new char[128];
	strcpy(name,l_str);
	context->baseName = name;
	context->countTrack = countTrack;
	context->duration = countSample;
	context->runing = true;
	//start upLoad model
	td_printf("----------------------------init ------a0------------\n");
	
	upload_module_init("");
	td_printf("----------------------------init ------a1------------\n");
	upload_module_start(1,"E3DAFD9C-0000-0000-FFFF-FFFFFFFFFF14","/mnt/sdcard/seraphim/",0,0,"219.237.241.176",5601);
	td_printf("----------------------------init ------a2------------\n");
	pthread_t tid;
	pthread_create(&tid,NULL,workTask,0);
}

/*
 * Class:     com_seraphim_td_nativ_QMP4Creater
 * Method:    n_addSample
 * Signature: ([BI)V
 */
JNIEXPORT void JNICALL Java_com_seraphim_td_nativ_QMP4Creater_n_1addSample
  (JNIEnv *env, jobject obj, jbyteArray sample ,jint offset,jint len, jint trackIndex){
	static int g_index = 0;
	jbyte* l_data = (jbyte*)env->GetByteArrayElements(sample, 0);
	uint8_t* data = new uint8_t[len];
	memcpy(data,(uint8_t*)l_data,len);
	if(data==NULL){
		td_printf("----------------t0-----------------\n");
		return;
	}
	if(trackIndex==0 && (!g_PPS || !g_SPS)){
		uint8_t *s_pps = NULL;
		uint8_t	*s_sps =NULL;
		uint8_t *p;
		p =data;
		while(p<data+len){
			if(*p==0x00 && *(p+1)==0x00 && *(p+2)==0x00 && *(p+3)==0x01){
				if(s_pps == NULL || s_sps==NULL){
					//td_printf("----------------t3-----------------\n");
					if(*(p+4)==0x67)
					{
						s_pps = p;
					}else if(*(p+4)==0x68){
						s_sps = p;
					}
				}else{

					g_lenPPS = s_sps > s_pps? s_sps - s_pps:p-s_pps;
					g_lenSPS=  s_pps > s_sps? s_pps-  s_sps:p - s_sps;
					g_SPS = new uint8_t[g_lenSPS];
					g_PPS = new uint8_t[g_lenPPS];
					memcpy(g_SPS,s_sps,g_lenSPS);
					memcpy(g_PPS,s_pps,g_lenPPS);
					const  char * fragment = "pps sps ------";
					break;
				}
			}
		p++;
		}
	}
	//td_printf("------------add sample trackIndex =%d-----addr=%p---len=%d------------\n",trackIndex,data,len);
	env->ReleaseByteArrayElements(sample,l_data,0);
	context->idAndBuf[trackIndex]->write(data,len);
}

/*
 * Class:     com_seraphim_td_nativ_QMP4Creater
 * Method:    n_addPCM
 * Signature: ([B)V
 */
int pcmIndex=0;
JNIEXPORT void JNICALL Java_com_seraphim_td_nativ_QMP4Creater_n_1addPCM
  (JNIEnv *env, jobject obj, jbyteArray sample){
	//td_printf("call add PCM\n");
	if(context->pcmBuf!=NULL){
		jsize  len = env->GetArrayLength(sample);
		jbyte* l_data = (jbyte*)env->GetByteArrayElements(sample, 0);
		uint8_t* data = new uint8_t[len];
		memcpy(data,l_data,len);
		context->pcmBuf->write(data,len);
		env->ReleaseByteArrayElements(sample,l_data,0);
	}else{
		td_printf("pcmBuf=NULL \n");
	}

}

/*
 * Class:     com_seraphim_td_nativ_QMP4Creater
 * Method:    n_addYUV
 * Signature: ([B)V
 */
JNIEXPORT void JNICALL Java_com_seraphim_td_nativ_QMP4Creater_n_1addYUV
 (JNIEnv *env, jobject obj, jbyteArray data){

}
/*
 * Class:     com_seraphim_td_nativ_QMP4Creater
 * Method:    n_addPPS
 * Signature: ([BI)V
 */
JNIEXPORT void JNICALL Java_com_seraphim_td_nativ_QMP4Creater_n_1addPPS
  (JNIEnv *env, jobject obj, jbyteArray pps , jint len){
	log4("--------------add PPS 0--------------\n");
	jsize  size = env->GetArrayLength(pps);
	log4("--------------add PPS 1  size=%d--------------\n",size);
	jbyte* l_data = (jbyte*)env->GetByteArrayElements(pps, 0);
	g_PPS = new uint8_t[len];
	g_lenPPS = len;
	memcpy(g_PPS,l_data,len);
	log4("add pps data[0]%x data[1]%x data[2]%x data[3]%x len=%d\n",g_PPS[0],g_PPS[1],g_PPS[2],g_PPS[3],len);
	env->ReleaseByteArrayElements(pps,l_data,0);

}

/*
 * Class:     com_seraphim_td_nativ_QMP4Creater
 * Method:    n_addSPS
 * Signature: ([BI)V
 */
JNIEXPORT void JNICALL Java_com_seraphim_td_nativ_QMP4Creater_n_1addSPS
  (JNIEnv *env, jobject obj, jbyteArray sps, jint len){
	jbyte* l_data = (jbyte*)env->GetByteArrayElements(sps, 0);
	log4("--------------add SPS 0--------------\n");
	jsize  size = env->GetArrayLength(sps);
	log4("--------------add PPS 1  size=%d------len=%d--------\n",size,len);
	g_SPS = new uint8_t[len];
	log4("--------------add SPS 1--------------\n");
	g_lenSPS = len;
	log4("--------------add SPS 2--------------\n");
	memcpy(g_SPS,l_data,len);
	log4("--------------add SPS 0-------------len=%d--0=%x- 1=%x 2=%x 3=%x ---\n",len,g_SPS[0],g_SPS[1],g_SPS[2],g_SPS[3]);
	env->ReleaseByteArrayElements(sps,l_data,0);
}

}
