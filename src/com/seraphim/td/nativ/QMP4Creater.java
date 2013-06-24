package com.seraphim.td.nativ;

import java.io.IOException;

import com.seraphim.td.omx.QSink;
import com.seraphim.td.omx.QTrackParam;

public class QMP4Creater implements QSink {
	//生产多个MP4文件

	
	public QMP4Creater(int sampleFile,String baseName,int countTrack,QTrackParam[] parms,String guid){
		n_init(sampleFile,baseName,countTrack,parms,guid);
	}
	public QMP4Creater(){};
	public void init(int sampleFile,String baseName,int countTrack,QTrackParam[] parms,String guid){
		n_init(sampleFile,baseName,countTrack,parms,guid);
	}
	public void addPCM(byte[] pcm,int offset,int len){
		n_addPCM(pcm);
	}
	public void addYUV(byte[] yuv,int offset,int len){
		
	}
	public void addPPS(byte[] pps,int len){
		n_addPPS(pps,len);
	}
	public void addSPS(byte[] sps,int len){
		n_addSPS(sps,len);
	}
	/**
	 * 
	 * @param data
	 * @param offset
	 * @param len
	 * @param trackID  videoId = 0  audioId =1 ,2.....
 	 * @return
	 * @throws IOException
	 */
	@Override
	public boolean write(byte[] data, int offset, int len,int trackID) throws IOException {
		// TODO Auto-generated method stub
		n_addSample(data,offset,len,trackID);
		return false;
	}

	@Override
	public void close() {
		// TODO Auto-generated method stub
//		n_stop();
	}

	@Override
	public void flush() {
		// TODO Auto-generated method stub

	}
/**
 * 	const int sizeFile = p->framePreFile;
	const char* baseName = p->mp4BaseName;
	const int widht = p->width;
	const int height = p->height;
	const int bitRate = p->bitRate;
	const timeScale = p->timeScale;
 * @param sample
 */
	native void n_init(int samplleFile,String baseName,int countTrack,QTrackParam[] parms,String guid);
	native void n_addSample(byte[] data,int offset,int len,int trackIndex);
	native void n_addPPS(byte[] data,int len);
	native void n_addSPS(byte[] data,int len);
	native void n_addPCM(byte[] data);
	native void n_addYUV(byte[] data);
}
