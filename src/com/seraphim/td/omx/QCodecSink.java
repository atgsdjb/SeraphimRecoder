package com.seraphim.td.omx;

import java.io.File;
import java.io.FileOutputStream;
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
	int t_index = 0;
	/*******seraphim3********/
	static  OutputStream headFile;
	static int indexHead;
	static{
		try{
			File file = new File("/mnt/sdcard/seraphim/head.dat");
			file.deleteOnExit();
			file.createNewFile();
			headFile = new  FileOutputStream(file);
			indexHead=0;
		}catch(Exception e){
			
		}
	}
	/*******seraphim3********/
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
				mSink.write(outData, 0, len,mTrackID);
				if(indexHead<10){
					indexHead++;
					headFile.write(outData, 0, len);
					headFile.write("------------".getBytes());
				}else if(indexHead==10){
					headFile.flush();
					headFile.close();
				}
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
	}
	public QCodecSink(String type,MediaFormat format,int trackID,QSink sink) {
		this.codecType = type;
		mFormat = format;
		this.buffer = new ArrayBlockingQueue<byte[]>(2*1024*1024, true);
		mSink = sink;
		mTrackID = trackID;
		initCodec();
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
	boolean notPPS = true;
	boolean notSPS = true;
	byte qPPS = 0x67;
	byte qSPS = 0x68;
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
			}
			indexOutput = codec.dequeueOutputBuffer(info, 0);
			while(indexOutput>=0){
				//get a nalu 
				 int size_buf = info.size;
				   byte[] h264 =  new  byte[size_buf];
				   outputBuffer[indexOutput].get(h264);
				   int i_long_startCode = h264[2]==0x01?0:1;
				   int sizeHead = 3 +i_long_startCode;
				   int sizePayload = size_buf-sizeHead;
				   if(notPPS || notSPS){
//						   if(mSink instanceof QMP4Creater){
					   byte[] sps = new byte[13];
					   byte[] pps = new byte[8];
					   int ii =0;
					   for(ii=0;ii<13;ii++){
						   sps[ii] = h264[ii];
					   }
					   for(ii=0;ii<8;ii++){
						   pps[ii]=h264[ii+13];
					   }
					   QMP4Creater creater =(QMP4Creater)mSink;
					   creater.addSPS(sps, 13);
					   creater.addPPS(pps, 8);
					   notPPS = false;
					   notSPS = false;
					   codec.releaseOutputBuffer(indexOutput, false);
					   indexOutput = codec.dequeueOutputBuffer(info,100);
					   continue;
//					   }
				   }
				   dst[len++] = (byte)((sizePayload >> 24) & 0xff);
				   dst[len++] = (byte)((sizePayload >> 16) & 0xff);
				   dst[len++] = (byte)((sizePayload >> 8) & 0xff);
				   dst[len++] = (byte)(sizePayload  & 0xff);
				   for( int i=0 ;i<sizePayload;i++){
					   if(len <bufferMaxSize)
					   dst[len++]  = h264[i+sizeHead];
					   else
						   Log.e(TAG,"dst[] overflow len ="+len);
				 }
				 codec.releaseOutputBuffer(indexOutput, false);
			     indexOutput = codec.dequeueOutputBuffer(info,100);
			}
		}catch(Exception e){
			Log.e(TAG,"exception in encode2 msg="+e.getMessage()+"");
		}
		return len;

	}

	
	
}