package com.seraphim.td.omx;

public class QVideoTrackParam extends QTrackParam{
	 int width;
	 int height;
	 int bitRate;
	
	
	
	
	public QVideoTrackParam(int timeScale, int bitRate, int sampleRate,
			int duration, int renderingOffset,   int width,
			int height) {
		super(0,timeScale, bitRate, sampleRate, duration, renderingOffset,false);
		this.width = width;
		this.height = height;
	}
	static final int type =0;
	
	
}
