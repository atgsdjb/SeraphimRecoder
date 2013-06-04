#include"context.h" 
#include"SMp4Creater.h"
#include"SSyncBuf.h"
#include"STrackParam.h"
#include"us_log.h"

using namespace Seraphim;

const char* lg_baseName="/mnt/sdcard/seraphim/E3DAFD9C-0000-0000-FFFF-FFFFFFFFFF14_%d.mp4";
void* workTask(void* param){
	int fileIndex = 0;
	char fileName[64]={0};
	sprintf(fileName,context->baseName,fileIndex++);
	//  wait  pps  sps
	do{
		td_printf("--wiat pps sps---\n");
		sleep(1);
	}while(g_PPS == NULL || g_SPS==NULL || g_first ==NULL);
	while(context->runing){
		char fileName[64]={0};
		sprintf(fileName,/*context->baseName */lg_baseName,fileIndex++);
		td_printf("-----begin---%s---\n",fileName);
		SMp4Creater creater(fileName,10,context->idAndParm,context->idAndBuf,false);
		creater.addPPS(g_PPS,g_lenPPS,0);
		creater.addSPS(g_SPS,g_lenSPS,0);
		creater.addFirstSample(g_first,  g_lenFirst);
		creater.startEncode();
		td_printf("-----end-----%s---\n",fileName);
	}
//	td_printf("-----workTask exit---------\n");
	return 0;

}

extern"C"{
#include<stdio.h>
}
 void* aacTask(void* /*param*/){
	 	/**
	 *
	 */
	//default get trackID =1 ;

	 //faacEncHandle aacHandler = context->aacHandler;
//	FILE* f = fopen("/mnt/sdcard/seraphim/test.aac","wb+");
//	FILE* pcmF = fopen("/mnt/sdcard/seraphim/test.pcm","wb+");
	 /************************************************************************/
	 td_printf("---aacTask0----\n");
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
//			td_printf("-------------pcmBuffer == NULL----------------\n");
			return 0;
		}
		if(aacBuffer ==NULL){
//			td_printf("------aacBuffer ==NULL------------\n");
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
//			fwrite(pcm,1,1024*2,pcmF);
//			fflush(pcmF);
			if(enc_len <=0){
				continue;
			}
			uint8_t* l_b = new uint8_t[enc_len];
			memcpy(l_b,l_sample,enc_len);
			aacBuffer->write(l_b,enc_len);
//			fwrite(l_b,1,enc_len,f);
//			fflush(f);
			delete[] pcm;
		}
	td_printf("---aacTask exit---------------\n");
	return 0;
}
// void* aacTask2(void* /*param*/){
//	 	/**
//	 *
//	 */
//	FILE* aacfile = fopen("d:\\1video\\t23.aac","wb+");
//
//	//default get trackID =1 ;
//	//faacEncHandle aacHandler = context->aacHandler;
//	/************************************************************************/
//	unsigned long sampleRate = 44100;//audioP->sampleRate;
//	unsigned long bitRate = 32*1024;//audioP->bitRate;
//	int numChannels = 1;
//	unsigned long inputBuffSize;
//	unsigned long outBuffSize;
//	faacEncHandle aacHandler = faacEncOpen(sampleRate,numChannels,&inputBuffSize,&outBuffSize);
//
//	/************************************************************************/
//	faacEncConfigurationPtr conf = faacEncGetCurrentConfiguration(aacHandler);
//	conf->bitRate = 32*1024;
//	conf->inputFormat = FAAC_INPUT_16BIT;
//	conf->mpegVersion = MPEG4;
//	conf->aacObjectType == LOW;
//	faacEncSetConfiguration(aacHandler,conf);
//		SSyncBuffer *pcmBuffer = context->pcmBuf ;
//		uint8_t* pcm;
//
//		SSyncBuffer *aacBuffer= context->idAndBuf[1];
//		if(pcmBuffer == NULL || aacBuffer ==NULL){
//			return 0;
//		}
//		int len=-1;
//		uint8_t *l_sample=new uint8_t[1024*2];
//		size_t enc_len;
//		while((len = pcmBuffer->read(&pcm)) != -1){
//			if(len == 0)
//				continue;
//			int pcmOffset;
//			for(pcmOffset=0;pcmOffset<len;pcmOffset+=1024*2){
//				enc_len = faacEncEncode(aacHandler,(int32_t*)(pcm+pcmOffset),1024,l_sample,1024*2);
//				if(enc_len <=0){
//					continue;
//				}
//				uint8_t* l_b = new uint8_t[enc_len];
//				memcpy(l_b,l_sample,enc_len);
//				//td_printf("-add aac sample addr=%p len=%d-----\n",l_b,enc_len);
//				aacBuffer->write(l_b,enc_len);
//				fwrite(l_b,1,enc_len,aacfile);
//				fflush(aacfile);
//			}
//
//			//delete[] pcm;
//		}
//	return 0;
//}

// void* avcTask(void* /*_param*/){
//	SSyncBuffer* syncBuff = context->idAndBuf[0];
//	x264_t* pX264Handle = context->x264Handler;
//	x264_param_t *param= context->x264Param;
//	Yuv420 *yuv =  context->yuvData;
//	int iResult = 0;
//	int iNal;
//	x264_nal_t* pNals= new x264_nal_t;
//	//assert (pX264Handle);
//	iResult = x264_encoder_headers (pX264Handle, &pNals, &iNal);
//	int iMaxFrames = x264_encoder_maximum_delayed_frames (pX264Handle);
//	iNal = 0;
//	pNals = NULL;
//	x264_picture_t * pPicIn = new x264_picture_t;
//	x264_picture_t * pPicOut = new x264_picture_t;
//	x264_picture_init (pPicOut);
//	x264_picture_alloc (pPicIn, X264_CSP_I420, param->i_width,param->i_height);
//	pPicIn->img.i_csp = X264_CSP_I420;
//	pPicIn->img.i_plane = 3;
//	int iDataLen = param->i_width * param->i_height;
//	uint8_t *y = yuv->getY();
//	unsigned int  indexFrame=0;
//	unsigned int  indexNALU = 0;
//	uint8_t *lAVCBuf = 0;
//	while(y!=NULL /*&& lt_index++ < 4*/){
//		pPicIn->img.plane[0]=y;
//		pPicIn->img.plane[1]=yuv->getU();
//		pPicIn->img.plane[2]=yuv->getV();
//		iResult = x264_encoder_encode(pX264Handle,&pNals,&iNal,pPicIn,pPicOut);
//		uint8_t* pps = NULL;
//		uint8_t* sps = NULL;
//		if(iResult<0){
//			return 0;
//
//		}else if(iResult ==0){
//			//cout<<"����ɹ�,���Ǳ�����"<<endl;
//			continue;
//		}else{
//			int l_postion = 0;
//			lAVCBuf = new uint8_t[AVC_SAMPLE_BUFFER_SIZE];
//			for (int i = 0; i < iNal; ++i)
//			{
//				uint8_t	 naluHead[4]={0};
//				uint32_t sizePayload = pNals[i].i_payload-3-pNals[i].b_long_startcode ;
//				uint32_t naluSize = sizePayload +4 ;
//				naluHead[0] = (sizePayload >> 24) & 0xff;
//				naluHead[1] = (sizePayload >> 16) & 0xff;
//				naluHead[2] = (sizePayload >> 8)  & 0xff;
//				naluHead[3] = sizePayload & 0xff;
//				if(g_PPS == NULL || g_SPS==NULL){
//					int len  = pNals[i].i_payload-3- pNals[i].b_long_startcode;
//					uint8_t  *l_bs = new uint8_t[len];
//					memcpy(l_bs,pNals[i].p_payload +3 +pNals[i].b_long_startcode ,len);
//					if(pNals[i].i_type == NAL_PPS){
//						g_PPS = l_bs;
//						g_lenPPS = len;
//					}else if(pNals[i].i_type == NAL_SPS){
//						g_SPS = l_bs;
//						g_lenSPS = len;
//					}
//				}
//				memcpy(lAVCBuf+l_postion,naluHead,4);
//				memcpy(lAVCBuf+l_postion+4,pNals[i].p_payload  + 3+ pNals[i].b_long_startcode,sizePayload);
//				l_postion+=naluSize;
//				//cout<<"����ɹ�,д���ļ�"<<"----------------------"<<indexNALU++<<"------NALU-size=-----"<<pNals[i].i_payload<<"------------"<<endl;
//			}
//
//			syncBuff->write23(lAVCBuf,l_postion);
//			//delete[] y;
//		}
//		y = yuv->getY();
//	}
//	/*fclose(pFile);
//	fclose(lFile); */
//	syncBuff->disable();
//	return 0;
//}
//void* readPCM(void* /*pram*/){
//	uint8_t *pcm = new uint8_t[1024*2];
//	FILE *file = fopen("d:\\1video\\t4.pcm","rb");
//	int len =  0;
//	 len = fread(pcm,1,1024*2,file);
//	 SSyncBuffer *pcmBuf = context->pcmBuf;
//	 while(len >0){
//		 pcm =  new uint8_t[1024*2];
//		 pcmBuf->write23(pcm,1024*2);
//		 len = fread(pcm,1,1024*2,file);
//	 }
//	return 0;
//}
