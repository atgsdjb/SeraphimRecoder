package com.seraph.td.mediarecoder;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import com.seraphim.td.nativ.FAACNative;
import com.seraphim.td.nativ.QMP4Creater;
import com.seraphim.td.omx.QCodecSink;
import com.seraphim.td.omx.QFileSink;
import com.seraphim.td.omx.QSink;
import android.content.Context;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaCodecInfo;
import android.media.MediaFormat;
import android.media.MediaRecorder;
import android.os.Environment;
import android.util.Log;

public class UuseeAudioRecorder  {
	Context mContext;
	boolean workFLG = true;
	AudioRecord mAudioRecord;
	
	final String TAG="com.seraphim.td.mediacoder.UuseeAudioRecoder";
	private QCodecSink aacEncoder;
	private QSink mQSink;
	
	private boolean usedSoft;
	private int audioSource = MediaRecorder.AudioSource.MIC;
	private int sampleRateInHz = 44100;
	private int channelConfig = AudioFormat.CHANNEL_IN_MONO;
	private int audioFormat = AudioFormat.ENCODING_PCM_16BIT;
	private int bufferSizeB ;
	private byte[] readBuf;
	private boolean runFlg =false;
	private  Thread workTask= new Thread(){

		@Override
		public void run() {
			// TODO Auto-generated method stub
			mAudioRecord.startRecording();
			record();
//			}
			mAudioRecord.stop();
		}
		
	};
	public UuseeAudioRecorder(Context context,QMP4Creater sink,boolean usedSoft){
		mContext = context;
		initRecoder();
		this.usedSoft = usedSoft;
		this.mQSink = sink;
	}
	public UuseeAudioRecorder(Context context,QSink sink){
		mContext = context;
		initRecoder();
		mQSink = sink;
		createCodec();
	}
	
	public UuseeAudioRecorder(Context context){
		mContext = context;
		initRecoder();
		mQSink = null;
		createCodec();
	}
	private void initRecoder(){
		bufferSizeB = AudioRecord.getMinBufferSize(sampleRateInHz, channelConfig, audioFormat);
		mAudioRecord = new AudioRecord(audioSource, sampleRateInHz, channelConfig, audioFormat, bufferSizeB);
		readBuf = new byte[1024*2];
	}
	
	private void createCodec(){
		try{
			QFileSink file = new QFileSink(Environment.getExternalStorageDirectory().getPath()+"/seraphim/20130508.aac");
			MediaFormat format = new MediaFormat();
			format.setString(MediaFormat.KEY_MIME, "audio/mp4a-latm");
			format.setInteger(MediaFormat.KEY_BIT_RATE, 128000);
			format.setInteger(MediaFormat.KEY_CHANNEL_COUNT, 1);
			format.setInteger(MediaFormat.KEY_SAMPLE_RATE, 44100);
			format.setInteger(MediaFormat.KEY_AAC_PROFILE, MediaCodecInfo.CodecProfileLevel.AACObjectLC);
			aacEncoder = new QCodecSink("audio/mp4a-latm", format,1, file);
		}catch(Exception e){
			Log.e(TAG,"create AACEncoder exception"+e.getMessage());
		}
	}

	public void startRecord(){
		runFlg = true;
		if(aacEncoder !=null && !usedSoft){
			aacEncoder.start();
		}
		
		workTask.start();
	}
	
	public void stopRecord(){
		runFlg = false;
		try {
			workTask.join(100);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	
	/**
	 * 
	 */
	
	@SuppressWarnings("unused")
	private void record(){
		mAudioRecord.startRecording();
		int channelCount = mAudioRecord.getChannelCount();
		QMP4Creater creater = null;
		if(usedSoft){
			if(mQSink instanceof QMP4Creater){
				creater = (QMP4Creater)mQSink;
			}
		}
		
		while(runFlg){
			int len = mAudioRecord.read(readBuf, 0, readBuf.length);	
			if(usedSoft){
				creater.addPCM(readBuf, 0, len);
//				Log.e("com.seraphim.td.nativ","add PCM");
			}else{
				
				try {
					aacEncoder.write(readBuf, 0, readBuf.length, 1);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				FAACNative.addPCMSample(readBuf, len);
			}
		}
	}
	/**
	 * 
	 */
	@SuppressWarnings("unused")
	private void record3(){
		try{
			File file = new File(Environment.getExternalStorageDirectory().getPath()+"/seraphim/seraphim.pcm");
			InputStream in = new FileInputStream(file);
			int lenRead = 0;
			int lenWrite = 0;
			byte[] buffer =new byte[2048];
			do{
				lenRead = in.read(buffer);
				aacEncoder.write(buffer, 0, 2048, 1);
				
			}while(lenRead ==2048 );
			in.close();
			aacEncoder.close();
		}catch (Exception e) {
			// TODO: handle exception
		}
		
	}
	@SuppressWarnings("unused")
	private void record2(){
		File file = new File(Environment.getExternalStorageDirectory().getPath()+"/seraphim/t4.pcm");
		QMP4Creater creater = null;
		if(usedSoft){
			if(mQSink instanceof QMP4Creater){
				creater = (QMP4Creater)mQSink;
			}
		}
		OutputStream out;

		try{
			if(file.exists()){
				file.delete();
			}
			file.createNewFile();
			out = new FileOutputStream(file);
			long index;
			int len;
			for(index=0 ;index <17200;index++){
				Log.e(TAG,"BEGIN read index sample'S  " +"\t index =" +index);
				len = mAudioRecord.read(readBuf, 0, readBuf.length);
				Log.e(TAG,"END   read index sample'S len = "+ len +"\t index =" +index);
//				index++;
				creater.addPCM(readBuf, 0, len);
				out.write(readBuf, 0, len);
				out.flush();
			}
			Log.e(TAG,"--------------------------------");
		}catch (Exception e) {
			// TODO: handle exception
			Log.e(TAG,"record audio exception  msg="+e.getMessage());
		}
	}
	

}
