package com.seraphim.td.nativ;

import java.io.IOException;

import com.seraphim.td.omx.QSink;
import com.seraphim.td.omx.QTrackParam;

public class QMP4Creater implements QSink {
	//生产多个MP4文件

	
	public QMP4Creater(int sampleFile,String baseName,int countTrack,QTrackParam[] parms){
		n_init(sampleFile,baseName,countTrack,parms);
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
		n_add(data,offset,len,trackID);
		return false;
	}

	@Override
	public void close() {
		// TODO Auto-generated method stub
		n_stop();
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
	native void n_AddSpsPps(byte[] pps,int lenPps,byte[] Sps,int lenSps);
	native void n_add(byte[] sample,int offset ,int len,int trackID);
	native void n_init(int samplleFile,String baseName,int countTrack,QTrackParam[] parms);
	native void n_stop();
	public static native void log(String str);
}
