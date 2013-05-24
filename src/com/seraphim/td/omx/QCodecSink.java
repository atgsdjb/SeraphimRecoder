package com.seraphim.td.omx;

import java.io.IOException;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.TimeUnit;

import com.seraphim.td.nativ.QMP4Creater;

import android.media.MediaCodec;
import android.media.MediaFormat;
import android.media.MediaCodec.BufferInfo;
import android.util.Log;

public class QCodecSink implements QSink{
	private  QSink  mSink=null;
	private  ArrayBlockingQueue<byte[]> buffer;
	private MediaFormat mFormat=null;
	private final String TAG ="com.seraphim.td.omx.QEncoder";
	private MediaCodec codec=null;
	private boolean isRun = false;
	private int bufferMaxSize=500*1024;
	private int mTrackID;
//	private  int mFramePreFile;
	static private final String BIND="AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
	int t_index = 0;
	private Runnable encodeTask = new Runnable() {
		
		@Override
		public void run() {
			byte[] outData = new byte[bufferMaxSize];
			byte[] inData;
			while(isRun){
				try{
					
				inData = buffer.poll(500,TimeUnit.MILLISECONDS);
				if(inData==null || inData.length<0)
					continue;
				int len  = encodec2(outData,inData);
				
				if(len <=0 || len >=bufferMaxSize)
					continue;
				Log.e(TAG,"add  a frame  index = "+indexOut+"\t len ="+ len);
				mSink.write(outData, 0, len,mTrackID);
				mSink.write(BIND.getBytes(),0,BIND.getBytes().length,mTrackID);
//				QMP4Creater.log("--add frame ---- = "+indexOut+"-------");
//				out.write(outData);
				}catch(Exception e){
					Log.e(TAG,"error in task workThread");
				}
			}
		}
	};
	private String codecType;

	public QCodecSink(String type,String name,MediaFormat format,int trackID,QSink sink){
		this.buffer = new ArrayBlockingQueue<byte[]>(2*1024*1024, true);
		mSink = sink;
		mTrackID = trackID;
		mFormat = format;
		initCodec(name);
		start();
	}
	public QCodecSink(String type,MediaFormat format,int trackID,QSink sink) {
		this.codecType = type;
		mFormat = format;
		this.buffer = new ArrayBlockingQueue<byte[]>(2*1024*1024, true);
		mSink = sink;
		mTrackID = trackID;
		initCodec();
		start();
	};
	
	private void initCodec(String name){
		if(codec!=null){
			releaseCodec();
		}
		codec =  MediaCodec.createByCodecName(name);
		codec.configure(mFormat, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);
		codec.start();
	}
	
	private void initCodec(){
		if(codec!=null){
			releaseCodec();
		}
		codec =  MediaCodec.createEncoderByType(codecType);
		codec.configure(mFormat, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);
		codec.start();
	}
	private void releaseCodec(){
		if(codec!=null){
			codec.stop();
			codec.release();
			codec = null;
		}
	}
	public void start(){
		isRun = true;
		new Thread(encodeTask).start();
		
	}
	/**
	 * stop Encode  
	 * release codec
	 */
	public void stop(){
		isRun = false;
//		Thread.
	}
	
	@Override
	public boolean write(byte[] data, int offset, int len,int trackID) throws IOException {
		// TODO Auto-generated method stub
		try {
			buffer.put(data);
			return true;
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			Log.e(TAG,e.getMessage());
		}
		return false;
	}

	@Override
	public void close() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void flush() {
		// TODO Auto-generated method stub
		
	}
private	long indexIn=0;
private	long indexOut=0;
	OutputStream l_out;
	private int encodec2(byte[] dst,byte[] src){
		int len = 0;
		try{
			// TODO Auto-generated method stub
			ByteBuffer[]  inputBuffer ;
			ByteBuffer[]  outputBuffer;
			inputBuffer = codec.getInputBuffers();
			outputBuffer = codec.getOutputBuffers();
			int indexInput;
			int indexOutput;
			BufferInfo  info = new BufferInfo();
			byte[] inData = src;// = buffer.poll();
		
			indexInput = codec.dequeueInputBuffer(-1);
			if(indexInput >= 0){
				 inputBuffer[indexInput].clear();
				   inputBuffer[indexInput].put(inData);
				   codec.queueInputBuffer(indexInput,0,inData.length,0,MediaCodec.BUFFER_FLAG_CODEC_CONFIG);
				   //inData = null;
			}
			indexOutput = codec.dequeueOutputBuffer(info, 0);
			if(indexOutput >=0){
				indexOut++;
			}else{
//				Log.e(TAG,"empty out frame numOut = "+indexOut);
			}
			while(indexOutput>=0){
				//get a nalu 
				 int size_buf = info.size;
				   byte[] h264 =  new  byte[size_buf];
				   outputBuffer[indexOutput].get(h264);
				   for(int i = 0;i<info.size;i++){
					   if(len <bufferMaxSize)
					   dst[len++]  = h264[i];
					   else
						   Log.e(TAG,"dst[] overflow len ="+len);
				   }
				 codec.releaseOutputBuffer(indexOutput, false);
			     indexOutput = codec.dequeueOutputBuffer(info,100);
			}
	
//			len--;
			
		}catch(Exception e){
			Log.e(TAG,"exception in encode2 msg="+e.getMessage()+"");
		}
		return len;

	}

	
	
}

/*****************/
//private Thread task=new Thread("encode-thread"){
//
//
//@Override
//public void run() {
//	// TODO Auto-generated method stub
//	codec.start();
//	ByteBuffer[]  inputBuffer ;
//	ByteBuffer[]  outputBuffer;
//	inputBuffer = codec.getInputBuffers();
//	outputBuffer = codec.getOutputBuffers();
//	int len;
//	final long tiemoutUs =1000;
//	int inNum = 0;
//	int outNum = 0;
//	boolean flgEOF = false;
//	byte[] data = null;
//	while(isRun) {
//			   int inputBufferIndex = codec.dequeueInputBuffer(1000);
//			   if (inputBufferIndex >= 0 && !flgEOF) {
//				   Log.e(TAG+".dong","input number===="+(inputBufferIndex));
//			     // fill inputBuffers[inputBufferIndex] with valid data
//				   
//				   try {
//					data = buffer.poll(500,TimeUnit.MILLISECONDS);
//				} catch (InterruptedException e) {
//					// TODO Auto-generated catch block
//					e.printStackTrace();
//				}
//				   if(data == null){
//					   Log.e(TAG,"read null data");
//					   continue;
//				   }
//				   len = data.length;
//				   if(len != -1){
//					   inputBuffer[inputBufferIndex].clear();
//					   inputBuffer[inputBufferIndex].put(data);
//					   Log.e(TAG,"in number===="+(inNum++));
//					   
//					   codec.queueInputBuffer(inputBufferIndex,0,len,1000,0/*MediaCodec.BUFFER_FLAG_SYNC_FRAME */);
//					   data = null;
//				   }else{
//					   flgEOF = true;
//				   }
//				   
//			   }
//			   BufferInfo  info = new BufferInfo();
//			   int outputBufferIndex = codec.dequeueOutputBuffer(info,tiemoutUs);
//			   while(outputBufferIndex >=0){
//				   Log.e(TAG+".dong","out number===="+(outputBufferIndex));
//				   int size_buf = info.size;
//				   byte[] h264 = new  byte[size_buf];//outputBuffer[outputBufferIndex].array();
//				   outputBuffer[outputBufferIndex].get(h264);
//			     codec.releaseOutputBuffer(outputBufferIndex,false);
//			     outputBufferIndex = codec.dequeueOutputBuffer(info,tiemoutUs);
//			   };//while();
//			 if (outputBufferIndex == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
//		     outputBuffer = codec.getOutputBuffers();
//			 } else if (outputBufferIndex == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
//		     // Subsequent data will conform to new format.
//		     MediaFormat l_format = codec.getOutputFormat();
//		     Log.d(TAG,l_format.toString());
//			 }
//			
//		 }
//	     codec.stop();
//		 codec.release();
//		 mSink.close();
//		 codec = null;
//}
//
//};
