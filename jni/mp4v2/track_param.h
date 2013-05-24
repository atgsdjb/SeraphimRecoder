namespace Seraphim{
class QTrackParam{
public:
	int type;
	int timeScale;
	int bitRate;
	int sampleRate;
	int duration;
	int renderingOffset;
	QTrackParam(int _type,int _timeScale,int _bitRate,int _sampleRate,int _duration, int _renderingOffset)
	:type(_type),timeScale(_timeScale),bitRate(_bitRate),sampleRate(_sampleRate),duration(_duration),renderingOffset(_renderingOffset){};
	/*virtual*/ int getType(){return type;}
				int getTimeScale(){return timeScale;};
				int getBitRate(){return bitRate;};
				int getSampleRate(){return sampleRate;};
//	virtual ~QTrackParam(){};
};
class QAudioTrackParam  :public  QTrackParam{
public :
	QAudioTrackParam(int _timeScale,int _bitRate,int _sampleRate,int _duration,int _renderingOffset)
	:QTrackParam(1,_timeScale,_bitRate,_sampleRate,_duration,_renderingOffset){};
};
class QVideoTrackParm : public QTrackParam{
public:
	int width;
	int height;
	QVideoTrackParm(int _timeScale,int _width,int _height,int _bitRate,int _sampleRate,int _duration, int _renderingOffset)
	:QTrackParam(0,_timeScale,_bitRate,_sampleRate,_duration,_renderingOffset),width(_width),height(_height){};
};
};
