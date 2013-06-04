#include"SMp4Creater.h"
#ifdef DEBUG
#include<iostream>
#include<cassert>
#include"SNaluHelp.h"
#endif

/**test**/
#ifndef SDEBUG

#endif



#include"us_log.h"
extern"C"{
#include<stdio.h>
#include<string.h>
};


namespace Seraphim{
	/**test****************/
	void SMp4Creater::addFirstSample(uint8_t * _first, int _len){
				first = _first;
				lenFirst = _len;
				MP4WriteSample(file,trackS[0],first,lenFirst);
				trackTimesTampS[0] += ((SVideoTrackParm*)trackParamS[0])->durationPreFrame; 
				uint32_t l_len ;
				uint8_t* p_len =(uint8_t*) &l_len;
				p_len[0] = _first[3];
				p_len[1] = _first[2];
				p_len[2] = _first[1];
				p_len[3] = _first[0];
				char l_type = _first[4] & 0x1f;
				td_printf("---------add first sample l_len =%u::%x ---len =%d---type=%d-----\n",l_len,l_len,_len,l_type);
		}

	/**test****************/


	/*
	static void* encode_task(void *handle){
		SMp4Creater *creater = (SMp4Creater*)handle;
		int i =0;
		for(;i<19;i++){
			cout<<i<<endl;
		}
		return 0;
	}
	*/
	SMp4Creater::SMp4Creater(const char* _name,uint32_t _duration,map<uint8_t,STrackParam*> _trackParamS,map<uint8_t,SSyncBuffer*> _trackBufS,bool _isAsyn/* =false */,CompleteListener _listener/* =0 */)
	:name(_name),trackParamS(_trackParamS),trackBufS(_trackBufS),duration(_duration),isAsyn(_isAsyn),listener(_listener){
		assert(_trackBufS.size() == _trackParamS.size());
		trackCount = _trackBufS.size(); 
		for(int i = 0;i< trackCount;i++){
			trackS[i] = MP4_INVALID_TRACK_ID;
		}
		initTracks();
	}


	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	SMp4Creater::SMp4Creater(const char* _name,uint32_t _duration,const vector<STrackParam*>&_trackParam,const vector<SSyncBuffer*>& _trackBufS,bool _isAsyn,CompleteListener _listener)
	:name(_name),duration(_duration),listener(_listener),isAsyn(_isAsyn){
		int i = 0;
		assert(_trackParam.size() == _trackBufS.size());
		trackCount = _trackParam.size();
		for(i;i<trackCount;i++){
			trackS[i] = MP4_INVALID_TRACK_ID;
			trackBufS[i] = _trackBufS[i];
			trackParamS[i] = _trackParam[i];
		}
		initTracks();
	}


	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	SMp4Creater::SMp4Creater(const char* _name,uint32_t _duration,uint8_t _trackCount,STrackParam* _trackParam,SSyncBuffer* _trackBufS,bool _isAsyn,CompleteListener _listener)
		:name(_name),trackCount(_trackCount),duration(_duration),listener(_listener),isAsyn(_isAsyn){
		int i;
		for (i = 0;i<trackCount;i++){
			trackS[i] = MP4_INVALID_TRACK_ID;
			trackBufS[i] = &(_trackBufS[i]);
			trackParamS[i] = &(_trackParam[i]);
		}
		initTracks();
	}
	void SMp4Creater::addSample8(uint8_t *sample,size_t size,uint8_t trackIndex){
		//trackBufS[trackIndex].write23(sample,size);
	}
	void SMp4Creater::addSample16(uint16_t*sample,size_t size,uint8_t trackIndex){

	}
	void SMp4Creater::startEncode(){
		if(isAsyn){
			encodeLoop();		
		}else{
			encodeLoop();
		}
		
	}

	MP4TrackId SMp4Creater::createAudioTrack(STrackParam * param){
		SAudioTrackParam *p =(SAudioTrackParam*)param;
		 MP4Duration timeScale = p->timeScale;
		 MP4Duration sampleDuration = p->durationPreFrame;
		return MP4AddAudioTrack(file,timeScale,sampleDuration);
	}
	MP4TrackId SMp4Creater::createVideoTrack(STrackParam* param){
		SVideoTrackParm *p =(SVideoTrackParm*)param;
		uint32_t timeScale=p->timeScale;
		MP4Duration sampleDuration = p->durationPreFrame;
		uint16_t width =p->width;
		uint16_t height = p->height;
		//return MP4AddH264VideoTrack(file,timeScale,sampleDuration,width,height,0x64,0x00,0x1f,3);//MP4AddVideoTrack(file,timeScale,sampleDuration,width,height);
		return MP4AddH264VideoTrack(file,timeScale,sampleDuration,width,height,0x4d/*MP4_MPEG4_VIDEO_TYPE*/,0x40/*0x00 */,0x1e/*0x1f*/,3);//MP4AddVideoTrack(file,timeScale,sampleDuration,width,height);
	}

	void SMp4Creater::initTracks(){
		/*****test*******/
		char h264Name[128]={0};
		strcpy(h264Name,name);
		int index = strlen(name);
		h264Name[index-3]='2';
		h264Name[index-2]='6';
		h264Name[index-1]='4';
		h264File = fopen(h264Name,"wb+");
		/*****test*******/
		file = MP4CreateEx(name, MP4_DETAILS_ALL, 0, 1, 1, 0, 0, 0, 0);//����mp4�ļ�
		assert(file !=MP4_INVALID_FILE_HANDLE);
		MP4SetTimeScale(file,90000);
		MP4SetVideoProfileLevel(file, 0x7F);
		MP4SetAudioProfileLevel(file,0x2);
		int i = 0;
		for(i;i<trackCount;i++){
			int type = trackParamS[i]->type;
			MP4TrackId id  = MP4_INVALID_TRACK_ID;
			if(type == 0){
				id = createVideoTrack(trackParamS[i]);
			}else if(type == 1){
				id = createAudioTrack(trackParamS[i]);
			}else{
				assert(0);
			}
			trackS[i] = id;
			trackCompleteS[i]=false;
			trackDurationS[i] = duration*trackParamS[i]->timeScale;
			trackTimesTampS[i] = 0;
			//td_printf("---initTracks  index =%d duration=%d ----------------\n",i,trackDurationS[i]);
		}
	}
#include<iostream>
	int indexV=0;
	int indexA=0;
	void SMp4Creater::encodeLoop(){
		int i ;
		uint8_t* sample;
		
		while(!comlete()){
			for(i=0;i<trackCount;i++){
				if(trackCompleteS[i]){
					continue;
				}
				int len = trackBufS[i]->read(&sample);
				if(len==0){

					continue;
				}
				if(len == -1){
					trackCompleteS[i]=true;
					continue;
				}
				int type = trackParamS[i]->type;
				if(type==0){
					if(trackTimesTampS[i] > trackDurationS[i]){   //��֤ûһvideo track  ��SPS PPS��ʼ
						//td_printf("-   GET I FRAME   type=%x-----",sample[4]);
						/**/
						uint8_t l_c =sample[5];
						uint8_t l_d =sample[4];
						/**/
						if(isIFrame(sample)){
						
							trackBufS[i]->writeBack(sample,len);
							
							trackCompleteS[i]=true;
							continue;
						}
					}else{									     //��д��A FRAME
						//mp
						//MP4WriteSample(file,trackS[i],sample,len);
						//û����ַ���Ƶ���
						trackTimesTampS[i] += ((SVideoTrackParm*)trackParamS[i])->durationPreFrame; 

					}
				}else if(type == 1){

					trackTimesTampS[i]+=((SAudioTrackParam*)trackParamS[i])->durationPreFrame;
					trackCompleteS[i] = trackTimesTampS[i] >= trackDurationS[i]; 
					//td_printf("write into mp4 a audio addr = %p len=%d  index=%d-trackTimesTampS=%d---trackDurationS=%d-----\n",
					//							      sample,len,indexA++,trackTimesTampS[i],trackDurationS[i]);
				}
				if(type==0){
					uint32_t l_len ;
					uint8_t* p_len =(uint8_t*) &l_len;
					p_len[0] = sample[3];
					p_len[1] = sample[2];
					p_len[2] = sample[1];
					p_len[3] = sample[0];
					char l_type = sample[4] & 0x1f;
					td_printf("---------add video sample l_len =%u::%x ---len =%d---type=%d-----\n",l_len,l_len,len,l_type);
					fwrite(sample,1,len,h264File);
					fwrite("SERAPHIM",1,8,h264File);
				}
				MP4WriteSample(file,trackS[i],sample,len);
				delete sample;


			}
		}
		fflush(h264File);
		fclose(h264File);
		MP4Close(file);
		if(isAsyn && listener != 0)
			listener();

		
	}
	bool SMp4Creater::comlete(){
		map<uint8_t,bool>::iterator i ;// trackCompleteS.begin();
		for(i=trackCompleteS.begin();i!=trackCompleteS.end();i++){
			if(!i->second)
				return false;
		}
		return true;
	}
	bool SMp4Creater::addPPS(uint8_t* sps ,int lenSPS,int trackIndex){
		return MP4AddH264PictureParameterSet(file,trackS[trackIndex],sps,lenSPS);
	}
	bool SMp4Creater::addSPS(uint8_t* pps ,int lenPPS,int trackIndex){
		return MP4AddH264SequenceParameterSet(file,trackS[trackIndex],pps,lenPPS);
	}
	SSyncBuffer* SMp4Creater::getBuffer(int trackIndex){
		return trackBufS[trackIndex];
	}

	STrackParam* SMp4Creater::getTrackParam(int trackIndex){
		return trackParamS[trackIndex];
	}

	
};
