package com.seraph.td.mediarecoder;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;

import android.app.Activity;
import android.media.MediaCodec;
import android.media.MediaCodec.BufferInfo;
import android.media.MediaCodecInfo;
import android.media.MediaCodecList;
import android.media.MediaFormat;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class CodecActivity extends Activity implements SurfaceHolder.Callback{

	static final String  TAG="com.seraphi.encode";
	SurfaceView mView ;
	Surface  mSurface;
 	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		mView = new SurfaceView(this);
		mView.getHolder().addCallback(this);
		setContentView(mView);
		queryMediaCodec();
	
	}
	private void  queryMediaCodec(){
		int count = MediaCodecList.getCodecCount();
		for(int i=0;i<count;i++){
			MediaCodecInfo info = MediaCodecList.getCodecInfoAt(i);
			String msg = info.getName();
			Log.e(TAG,msg);
		}
	}
	private void encode() throws IOException{
	
		File inFile = new File(Environment.getExternalStorageDirectory().getPath()+"/test_w352_h288_f2000.yuv");
		File outFile = new File(Environment.getExternalStorageDirectory().getPath()+"/test_w352_h288_f1000.h264");
		final int  sizeY = 352*288;
		final int  sizeU = 352*288/4;
		final int  sizeV = sizeU;
		final int  sizePreFrame = sizeY+sizeU+sizeV;
		byte[] buffer = new byte[sizePreFrame];
		
		if(!inFile.exists()){
			return;
		}
		InputStream in =null;
		OutputStream out = null;
		
		if(outFile.exists()){
			outFile.delete();
		}
		outFile.createNewFile();
		in = new FileInputStream(inFile);
		out = new FileOutputStream(outFile);
		MediaCodec codec = MediaCodec.createByCodecName("OMX.Nvidia.h264.encoder");
		Log.e(TAG,Environment.getExternalStorageDirectory().getPath() );
		MediaFormat format = MediaFormat.createVideoFormat("video/avc", 352,288);
		format.setInteger(MediaFormat.KEY_WIDTH,352);
		format.setInteger(MediaFormat.KEY_HEIGHT,288);
		format.setInteger(MediaFormat.KEY_COLOR_FORMAT, MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420Planar);
		format.setInteger(MediaFormat.KEY_FRAME_RATE, 25);
		format.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL,1);
		format.setInteger(MediaFormat.KEY_BIT_RATE,10240);
		format.setInteger(MediaFormat.KEY_MAX_INPUT_SIZE,1024*127);
		format.setInteger(MediaFormat.KEY_DURATION,1000*120);
		codec.configure(format, null, null,  MediaCodec.CONFIGURE_FLAG_ENCODE);
		codec.start();
		ByteBuffer[]  inputBuffer ;
		ByteBuffer[]  outputBuffer;
		inputBuffer = codec.getInputBuffers();
		outputBuffer = codec.getOutputBuffers();
		int len;
		final long tiemoutUs =1000;
		int numberOfFrame = 0;
		int inNum = 0;
		int outNum = 0;
		boolean flgEOF = false;
		for (numberOfFrame = 0;numberOfFrame<1000;) {
				   int inputBufferIndex = codec.dequeueInputBuffer(1000);
				   if (inputBufferIndex >= 0 && !flgEOF) {
				     // fill inputBuffers[inputBufferIndex] with valid data
					   
					   len =  in.read(buffer);
					   if(len != -1){
						   inputBuffer[inputBufferIndex].clear();
						   inputBuffer[inputBufferIndex].put(buffer);
						   Log.e(TAG,"in number===="+(inNum++));
						   
						   codec.queueInputBuffer(inputBufferIndex,0,sizePreFrame,1000,MediaCodec.BUFFER_FLAG_SYNC_FRAME);
					   }else{
						   flgEOF = true;
					   }
					   
				   }
				   BufferInfo  info = new BufferInfo();
				   int outputBufferIndex = codec.dequeueOutputBuffer(info,tiemoutUs);
				   if (outputBufferIndex >= 0) {
					   Log.e(TAG,"out number===="+(outNum++));
				     // outputBuffer is ready to be processed or rendered
					   numberOfFrame++;
					   int size_buf = info.size;
					   byte[] h264 = new  byte[size_buf];//outputBuffer[outputBufferIndex].array();
					   outputBuffer[outputBufferIndex].get(h264);
					   out.write(h264, 0, h264.length);
					   out.flush();
				     codec.releaseOutputBuffer(outputBufferIndex,false);
				   } else if (outputBufferIndex == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
				     outputBuffer = codec.getOutputBuffers();
				   } else if (outputBufferIndex == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
				     // Subsequent data will conform to new format.
				     MediaFormat l_format = codec.getOutputFormat();
				     Log.d(TAG,l_format.toString());
				   }
				
			 }
			 in.close(); 
			 out.close();
		     codec.stop();
			 codec.release();
			 codec = null;
	}
	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		// TODO Auto-generated method stub
		mSurface  = holder.getSurface();
		new Thread(){

			@Override
			public void run() {
				// TODO Auto-generated method stub
				try {
					encode();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			
		}.start();
	}
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		// TODO Auto-generated method stub
		
	}
	
}
