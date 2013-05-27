package com.seraphim.td.omx;

public class QAudioTrackParam extends QTrackParam {
	
	final int type = 1;
	public QAudioTrackParam(int timeScale, int bitRate, int sampleRate,
			int duration, int renderingOffset) {
		super(1,timeScale,bitRate,sampleRate,duration,renderingOffset,true);
	}
	public int getType(){
		return type;
	}
}
