package com.seraph.td.mediarecoder;


import com.seraphim.td.nativ.QMP4Creater;
import com.seraphim.td.omx.QAudioTrackParam;
import com.seraphim.td.omx.QSink;
import com.seraphim.td.omx.QTrackParam;
import com.seraphim.td.omx.QVideoTrackParam;
import android.app.Activity;
import android.media.MediaCodecInfo;
import android.media.MediaCodecList;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.Window;
import android.widget.Button;
public class UuseeCameraActivity extends Activity  implements SurfaceHolder.Callback{
	private UuseeCamera2  mUuseeCamera;
	private UuseeAudioRecorder audioRcoder;
	private SurfaceView  mView;
	private SurfaceHolder mHolder;
	private boolean atPreview = false;
	private boolean holderIsCreate = false;
	private QSink mMp4Sink;
	
	private int widthVideo ;
	private int heightVideo;
	
	Button mCtrlButton;
	private String TAG;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);  
		setContentView(R.layout.layout_uuseecamera);
		widthVideo = 640;
		heightVideo = 480;
		mView  = (SurfaceView) findViewById(R.id.surface);
		mHolder = mView.getHolder();
		mHolder.addCallback(this);
		mCtrlButton = (Button) findViewById(R.id.button);
		mCtrlButton.setEnabled(true);
//		testMediaCodec();
		createQSink();
	}

	/**
	 * 
	 */
	private void createQSink(){
		QTrackParam params[] = new QTrackParam[2];
		int secDuration = 300;
		/*************************************************/
		int videoTimeScale = 90000;
		int vieoBitRate = 500000;
		int videoSampleRate = 15;
		int videoHeight = heightVideo;
		int videoWidth = widthVideo;
		int videoRenderingOffset = 0;
		int videoDuration = videoTimeScale * secDuration;
		QVideoTrackParam videoTrack = new QVideoTrackParam(videoTimeScale, vieoBitRate, videoSampleRate,
									videoDuration, videoRenderingOffset, videoWidth, videoHeight);
		
		/*************************************************/
		int audioTimeScale=44100*1024;
		int audioBitRate = 32000;
		int audioSampleRate = 44100;
		int audioDuration = secDuration * audioTimeScale;
		int audioRenderingOffset =-1;
		QAudioTrackParam audioTrack = new QAudioTrackParam(audioTimeScale, audioBitRate, audioSampleRate, 
									audioDuration,audioRenderingOffset);
		params[0] = videoTrack;
		params[1] = audioTrack;
		try{
			mMp4Sink = new QMP4Creater(100,
					Environment.getExternalStorageDirectory().getPath()+"/seraphim/uusee%03d.mp4",
					2,params);
			mUuseeCamera = new  UuseeCamera2(this, 0,videoWidth,videoHeight,mMp4Sink);
			audioRcoder = new UuseeAudioRecorder(this,(QMP4Creater)mMp4Sink,true);
		}catch(Exception e){
			Log.e(TAG,""+e.getMessage());
		}
	}
	/**
	 * 
	 */
	void testMediaCodec(){
		for(int i =0;i< MediaCodecList.getCodecCount();i++){
			MediaCodecInfo info = MediaCodecList.getCodecInfoAt(i);
			StringBuilder sb = new StringBuilder("{");
			sb.append("["+info.getName());
			sb.append("]");
			for(String type : info.getSupportedTypes()){
				sb.append(type);
				sb.append("\t|");;
			}
			Log.e(TAG,sb.toString());
		}
	}
	public void ctrl(View view){
		if(!atPreview){
			mCtrlButton.setText("停止");
			if(holderIsCreate){
				atPreview = true;
				audioRcoder.startRecord();
				mUuseeCamera.startPreview();
			}
		}
		else{
			mCtrlButton.setText("开始");
			if(holderIsCreate) mUuseeCamera.stopPreview();
			audioRcoder.stopRecord();
		}
		mCtrlButton.invalidate();
	}
	//@Override
	public void surfaceCreated(SurfaceHolder holder) {
		// TODO Auto-generated method stub
		mUuseeCamera.setPreviewDisplay(holder);
		mCtrlButton.setEnabled(true);
		if(atPreview) mCtrlButton.setText("停止");
		else mCtrlButton.setText("开始");
		mCtrlButton.invalidate();
		holderIsCreate = true;
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {
//		// TODO Auto-generated method stub
	     if (mHolder.getSurface() == null){
	          // preview surface does not exist
	          return;
	        }

//	        // stop preview before making changes
	        try {
//	        	mUuseeCamera.stopPreview();
	        } catch (Exception e){
	          // ignore: tried to stop a non-existent preview
	        }

	        // set preview size and make any resize, rotate or
	        // reformatting changes here

	        // start preview with new settings
	        try {
//	        	mUuseeCamera.setPreviewDisplay(holder);
	        	if( atPreview )mUuseeCamera.startPreview();

	        } catch (Exception e){
	            Log.d(TAG, "Error starting camera preview: " + e.getMessage());
	        }
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		// TODO Auto-generated method stub
		mCtrlButton.setEnabled(false);
		holderIsCreate = false;
		if(atPreview){
			atPreview = false;
//			mUuseeCamera.stopPreview();
		}
		mCtrlButton.setText("STOP");
		mCtrlButton.invalidate();
		mCtrlButton.setEnabled(false);
		mUuseeCamera.release();
	}
	static{
		System.loadLibrary("gnustl_shared");
		System.loadLibrary("tdcodec.02");
	}
}
