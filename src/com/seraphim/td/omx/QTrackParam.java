package com.seraphim.td.omx;

public class QTrackParam {
	
	public QTrackParam(int _type,int timeScale, int bitRate, int sampleRate,
			int duration, int renderingOffset,boolean usedSoft) {
		super();
		type = _type;
		this.timeScale = timeScale;
		this.bitRate = bitRate;
		this.sampleRate = sampleRate;
		this.duration = duration;
		this.renderingOffset = renderingOffset;
		this.usedSoft = usedSoft;
		
	}
	 boolean usedSoft;
	 int type;
	 int timeScale;
	 int bitRate;
	 int sampleRate;
	 int duration;
	 int renderingOffset;
}
