#ifndef _SERAPHIM_MP4_CREATER_H
#define _SERAPHIM_MP4_CREATER_H

#define DEBUG


#include<cstdint>
#include"SSyncBuf.h"
#include"StrackParam.h"
#include"pthread.h"
#include"../mp4/mp4.h"
#include<map>
#include<vector>
//class STrackParam;
//class SAudioTrackParam;
//class SVideoTrackParm;
//class SyncBuffer;
typedef void(*CompleteListener)(void);
using namespace std;


namespace Seraphim{
class SMp4Creater{
private:
	const char		*name;
	uint32_t	duration;//sec
	uint8_t		trackCount;
	map<uint8_t,int>	trackS;
	map<uint8_t,STrackParam*> trackParamS;
	map<uint8_t,SSyncBuffer*>  trackBufS;
	map<uint8_t,bool> trackCompleteS;
	map<uint8_t,MP4Duration> trackDurationS;
	map<uint8_t,MP4Duration> trackTimesTampS;

	bool isAsyn;
	CompleteListener listener;
	MP4FileHandle file;
	MP4TrackId createAudioTrack(STrackParam* param);
	MP4TrackId createVideoTrack(STrackParam* param);
	bool comlete();
	void initTracks();
	void encodeLoop();
public:
	SMp4Creater(const char* _name,uint32_t _duration,uint8_t _trackCount,STrackParam *_trackParam,SSyncBuffer* _trackBufS, bool _isAsyn=false, CompleteListener _listener=0);
	SMp4Creater(const char* _name,uint32_t _duration,const vector<STrackParam*>& _trackParam,const vector<SSyncBuffer*>& _trackBufS,bool _isAsyn=false,CompleteListener _listener=0);	
	SMp4Creater(const char* _name,uint32_t _duration,map<uint8_t,STrackParam*> _trackParamS,map<uint8_t,SSyncBuffer*> _trackBufS,bool _isAsyn=false,CompleteListener _listener=0);
	void addSample8(uint8_t *sample,size_t size,uint8_t trackIndex);
	void addSample16(uint16_t* sample,size_t size,uint8_t trackIndex);
	void startEncode();
	bool addPPS(uint8_t *pps ,int lenPPS,int trackIndex);
	bool addSPS(uint8_t *sps ,int lenSPS,int trackIndex);
	SSyncBuffer* getBuffer(int trackIndex);
	STrackParam* getTrackParam(int trackIndex);


	friend void* encode_task(void*);

};
};
#endif