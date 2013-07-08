#include"SMp4Creater.h"
#ifdef DEBUG
#include<iostream>
#include<cassert>
#include"SNaluHelp.h"
#endif

/**test**/
#ifndef SDEBUG
extern"C"{
#include<stdio.h>
};
#endif
#include"us_log.h"
namespace Seraphim{
	SMp4Creater::SMp4Creater(const char* _name,uint32_t _duration,map<uint8_t,STrackParam*> _trackParamS,map<uint8_t,SSyncBuffer*> _trackBufS,bool _isAsyn/* =false */,CompleteListener _listener/* =0 */)
	:name(_name),trackParamS(_trackParamS),trackBufS(_trackBufS),duration(_duration),isAsyn(_isAsyn),listener(_listener){
		assert(_trackBufS.size() == _trackParamS.size());
		trackCount = _trackBufS.size(); 
		for(int i = 0;i< trackCount;i++){
			trackS[i] = MP4_INVALID_TRACK_ID;
		}
		firstSample = 0;
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

	void SMp4Creater::addVideoHead(int trackIndex,uint8_t * pps, int lenPPS, uint8_t * sps, int lenSPS, bool addFirstSample)
	{
		addSPS(sps, lenSPS,trackIndex);
		addPPS(pps, lenPPS,trackIndex);
		if(addFirstSample){
			//td_printf("addVideoHead\n");
			firstSample = new uint8_t[lenSPS+lenPPS];
			MP4WriteSample2(file,trackS[trackIndex],firstSample,lenPPS+lenSPS,false);
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
		}
	}
#include<iostream>
	int indexV=0;
	int indexA=0;
	void SMp4Creater::encodeLoop(){
		int i ;
		uint8_t* sample;
		int videoIndex=0;
		while(!comlete()){
			for(i=0;i<trackCount;i++){
				bool isSync=false;
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
					isSync = isIFrame(sample);
					if(trackTimesTampS[i] > trackDurationS[i]){   
						
						uint8_t l_c =sample[5];
						uint8_t l_d =sample[4];						
						if(isSync){
							trackBufS[i]->writeBack(sample,len);							
							trackCompleteS[i]=true;
							continue;
						}
					}else{									     
						trackTimesTampS[i] += ((SVideoTrackParm*)trackParamS[i])->durationPreFrame; 
						//fwrite(sample,1,len,h264File);
					}
					/************/
				}else if(type == 1){
					isSync = false;
					trackTimesTampS[i]+=((SAudioTrackParam*)trackParamS[i])->durationPreFrame;
					trackCompleteS[i] = trackTimesTampS[i] >= trackDurationS[i]; 
				}
				
				MP4WriteSample2(file,trackS[i],sample,len,isSync);
				delete sample;


			}
		}
		MP4Close(file);
		//fflush(h264File);
		//fclose(h264File);
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
