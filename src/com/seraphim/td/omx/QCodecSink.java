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
	OutputStream out=null;
	String str="------------------------------------------";
	byte[] band = str.getBytes();
	private Runnable encodeTask = new Runnable() {
		
		@Override
		public void run() {
			byte[] outData = new byte[bufferMaxSize];
			byte[] inData;
//			OutputStream out=null;
			String str="------------------------------------------";
			byte[] band = str.getBytes();
			try{
				File file = new File("/mnt/sdcard/seraphim/test.h264");
				file.deleteOnExit();
				out = new FileOutputStream(file);
			}catch(Exception e){
				
			}
			while(isRun){
				try{
					
				inData = buffer.poll(500,TimeUnit.MILLISECONDS);
				if(inData==null || inData.length<0)
					continue;
				int len  = encodec2(outData,inData);
				
				if(len <=0 || len >=bufferMaxSize)
					continue;
				mSink.write(outData, 0, len,mTrackID);
				
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
				   byte[] naluHead= new byte[4];
				   /**
				    * uint8_t	 naluHead[4]={0};
					  uint32_t sizePayload = pNals[i].i_payload-3-pNals[i].b_long_startcode ;
				      uint32_t naluSize = sizePayload +4 ;
				      naluHead[0] = (sizePayload >> 24) & 0xff;
				      naluHead[1] = (sizePayload >> 16) & 0xff;
				      naluHead[2] = (sizePayload >> 8)  & 0xff;
				      naluHead[3] = sizePayload & 0xff;
				      if(g_PPS == NULL || g_SPS==NULL){
						int len  = pNals[i].i_payload-3- pNals[i].b_long_startcode;
						uint8_t  *l_bs = new uint8_t[len];
						memcpy(l_bs,pNals[i].p_payload +3 +pNals[i].b_long_startcode ,len);
					if(pNals[i].i_type == NAL_PPS){
						g_PPS = l_bs;
						g_lenPPS = len;
					}else if(pNals[i].i_type == NAL_SPS){
						g_SPS = l_bs;
						g_lenSPS = len;
					}
				}
				    */
				   outputBuffer[indexOutput].get(h264);
				   try{
					  out.write(h264, 0, size_buf);
					  out.write(band, 0, band.length);
					  out.flush();
					   
				   }catch(Exception ee){
					   
				   }
				   int i_long_startCode = h264[2]==0x01?0:1;
				   int sizeHead = 3 +i_long_startCode;
				   int sizePayload = size_buf-sizeHead;
				   if(notPPS || notSPS){
						   if(mSink instanceof QMP4Creater){
						   byte[] sps = new byte[9];
						   byte[] pps = new byte[4];
						   for(int i=0;i<9;i++){
							   sps[i] = h264[4+i];
						   }
						   for(int i =0 ;i<4;i++){
							   pps[i]=h264[16+i];
						   }
						   QMP4Creater creater =(QMP4Creater)mSink;
						   creater.addSPS(sps, 9);
						   creater.addPPS(pps, 4);
						   notPPS = false;
						   notSPS = false;
					   }

				   }
				   dst[len++] = (byte)((sizePayload >> 24) & 0xff);
				   dst[len++] = (byte)((sizePayload >> 16) & 0xff);
				   dst[len++] = (byte)((sizePayload >> 8) & 0xff);
				   dst[len++] = (byte)(sizePayload  & 0xff);
				   for(int i = 0;i<sizePayload;i++){
					   if(len <bufferMaxSize)
					   dst[len++]  = h264[i+sizeHead];
					   else
						   Log.e(TAG,"dst[] overflow len ="+len);
				   }
//				   for(int i = 0;i<info.size;i++){
//				   if(len <bufferMaxSize)
//				   dst[len++]  = h264[i];
//				   else
//					   Log.e(TAG,"dst[] overflow len ="+len);
//			   }
				 codec.releaseOutputBuffer(indexOutput, false);
			     indexOutput = codec.dequeueOutputBuffer(info,100);
			}
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



//					   int i=0;
//					   if(h264[sizeHead] ==qSPS){
//						   byte[] sps = new byte[sizePayload];
//						   for(i = 0;i<sizePayload;i++){
//							   sps[i] = h264[sizeHead+i];
//						   }
//						   notSPS = false;
//						   if(mSink instanceof QMP4Creater){
//							   QMP4Creater creater =(QMP4Creater)mSink;
//							   creater.addSPS(sps, i);
//						   }
//						  
//					   }else if(h264[sizeHead]==qPPS){
//						   byte[] pps = new byte[sizePayload];
//						   for( i = 0;i<sizePayload;i++){
//							   pps[i] = h264[sizeHead+i];
//						   }
//						   if(mSink instanceof QMP4Creater){
//							   QMP4Creater creater =(QMP4Creater)mSink;
//							   creater.addPPS(pps, i);
//						   }
//						   notPPS = false;
//					   }****/
