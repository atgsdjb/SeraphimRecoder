////#include <iostream>
////#include <string>
//#include"yuv420.h"
////using namespace std;
//using namespace Seraphim;
//#include"mp4.h"
//#include<queue>
//using namespace std;
//extern "C"
//{
//#include<stdint.h>
//#include "x264.h"
//};
////using namespace std;
//static void initParam(x264_param_t* pX264Param,int width ,int height){
//	pX264Param->i_threads = X264_SYNC_LOOKAHEAD_AUTO;	//* ȡ�ջ��������ʹ�ò�����ı�֤.//* video Properties
//	pX264Param->i_frame_total = 0;						//* ������֡��.��֪����0.
//	pX264Param->i_keyint_max = 10;
//	pX264Param->i_bframe = 5;
//	pX264Param->b_open_gop = 0;
//	pX264Param->i_bframe_pyramid = 0;
//	pX264Param->i_bframe_adaptive = X264_B_ADAPT_TRELLIS;
//	pX264Param->b_annexb = 1;
//	pX264Param->i_log_level = X264_LOG_DEBUG;
//	pX264Param->rc.i_bitrate = 1024 * 10;				//* ����(������,��λKbps), muxing parameters
//	pX264Param->i_fps_den = 1;							//* ֡�ʷ�ĸ
//	pX264Param->i_fps_num = 25;							//* ֡�ʷ���
//	pX264Param->i_timebase_den = pX264Param->i_fps_num;
//	pX264Param->i_timebase_num = pX264Param->i_fps_den;
//	pX264Param->i_width = width;
//	pX264Param->i_height=height;
//}
//static  int  encode(){
//	int iResult = 0;
//	int iNal;
//	x264_nal_t* pNals= new x264_nal_t;
//	x264_t * pX264Handle = NULL;
//	x264_param_t *param= new x264_param_t;
//	x264_param_default(param);
//	initParam(param,176,144);
//	x264_param_apply_profile (param, x264_profile_names[1]);
//	pX264Handle = x264_encoder_open (param);
//	assert (pX264Handle);
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
//	//FILE * pFile = fopen ("agnt.264", "wb");
//	 MP4FileHandle file = MP4CreateEx("test.mp4", MP4_DETAILS_ALL, 0, 1, 1, 0, 0, 0, 0);//����mp4�ļ�
//	 assert(file !=MP4_INVALID_FILE_HANDLE);
//	 MP4SetTimeScale(file,90000);
//	 MP4TrackId video = MP4AddH264VideoTrack(file,90000,90000/25,176,144,0x64,0x00,0x1f,3);
//	 assert(video !=MP4_INVALID_TRACK_ID);
//     MP4SetVideoProfileLevel(file, 0x7F);
//	Yuv420 *yuv = new Yuv420(176,144,0,"test3.yuv");
//	int iDataLen = param->i_width * param->i_height;
//	uint8_t *y = yuv->getY();
//	uint8_t* sampleBuffer = new uint8_t[1024*1024*100];
//	while(y!=NULL){
//		pPicIn->img.plane[0]=y;
//		pPicIn->img.plane[1]=yuv->getU();
//		pPicIn->img.plane[2]=yuv->getV();
//		iResult = x264_encoder_encode(pX264Handle,&pNals,&iNal,pPicIn,pPicOut);
//		if(iResult<0){
//			return -1;
//		}else if(iResult ==0){
//
//		}else{
//			int l_postion = 0;
//			for (int i = 0; i < iNal; ++i)
//			{
//			//fwrite (pNals[i].p_payload, 1, pNals[i].i_payload, pFile);
//				memcpy(sampleBuffer+l_postion,pNals[i].p_payload,pNals[i].i_payload);
//				l_postion+=pNals[i].i_payload;
//			}
//			MP4WriteSample(file,video,sampleBuffer,l_postion);
//			//
//		}
//		y = yuv->getY();
//	}
//	MP4Close(file);
//	return 0;
//}
//static void createMp4(){
//     MP4FileHandle file = MP4CreateEx("test.mp4", MP4_DETAILS_ALL, 0, 1, 1, 0, 0, 0, 0);//����mp4�ļ�
//	 assert(file !=MP4_INVALID_FILE_HANDLE);
//	 MP4SetTimeScale(file,90000);
//	 MP4TrackId video = MP4AddH264VideoTrack(file,90000,90000/25,176,144,0x64,0x00,0x1f,3);
//	 assert(video !=MP4_INVALID_TRACK_ID);
//     MP4SetVideoProfileLevel(file, 0x7F);
//
//};
//int main2g()
//{
//	encode();
//	return 0;
//}
