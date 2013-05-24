#ifndef __SERAPHIM_TRACK_PARAM_H
#define __SERAPHIM_TRACK_PARAM_H
namespace Seraphim{
class STrackParam{
public:
	int type;
	int timeScale;
	int bitRate;
	int sampleRate;
	int duration;
	//int renderingOffset;
	STrackParam(int _type,int _timeScale,int _bitRate,int _sampleRate,int _duration)
	:type(_type),timeScale(_timeScale),bitRate(_bitRate),sampleRate(_sampleRate),duration(_duration){};
	/*virtual*/ int getType(){return type;}
				int getTimeScale(){return timeScale;};
				int getBitRate(){return bitRate;};
				int getSampleRate(){return sampleRate;};
//	virtual ~QTrackParam(){};
};
class SAudioTrackParam  :public  STrackParam{
public :
	
	uint32_t durationPreFrame;
	SAudioTrackParam(int _timeScale,int _bitRate,int _sampleRate,int _duration)
	:STrackParam(1,_timeScale,_bitRate,_sampleRate,_duration),durationPreFrame(_timeScale*1024/_sampleRate){};
};
class SVideoTrackParm : public STrackParam{
public:
	int width;

	int height;
	uint32_t durationPreFrame;
	SVideoTrackParm(int _timeScale,int _width,int _height,int _bitRate,int _sampleRate,int _duration )
	:STrackParam(0,_timeScale,_bitRate,_sampleRate,_duration),width(_width),height(_height),durationPreFrame(_timeScale/_sampleRate){};
};
};
#endif