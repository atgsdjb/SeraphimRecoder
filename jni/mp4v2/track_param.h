namespace Seraphim{
class QTrackParam{
public:
	int type;
	int timeScale;
	int bitRate;
	int sampleRate;
	int duration;
	int renderingOffset;
	bool usedSoft;
	QTrackParam(int _type,int _timeScale,int _bitRate,int _sampleRate,int _duration, int _renderingOffset,bool _usedSoft)
	:type(_type),timeScale(_timeScale),bitRate(_bitRate),sampleRate(_sampleRate),duration(_duration),renderingOffset(_renderingOffset),usedSoft(_usedSoft){};
	/*virtual*/ int getType(){return type;}
				int getTimeScale(){return timeScale;};
				int getBitRate(){return bitRate;};
				int getSampleRate(){return sampleRate;};
//	virtual ~QTrackParam(){};
};
class QAudioTrackParam  :public  QTrackParam{
public :
	QAudioTrackParam(int _timeScale,int _bitRate,int _sampleRate,int _duration,int _renderingOffset,bool _usedSoft)
	:QTrackParam(1,_timeScale,_bitRate,_sampleRate,_duration,_renderingOffset,_usedSoft){};
};
class QVideoTrackParm : public QTrackParam{
public:
	int width;
	int height;
	QVideoTrackParm(int _timeScale,int _width,int _height,int _bitRate,int _sampleRate,int _duration, int _renderingOffset,bool _usedSoft)
	:QTrackParam(0,_timeScale,_bitRate,_sampleRate,_duration,_renderingOffset,_usedSoft),width(_width),height(_height){};
};
};
