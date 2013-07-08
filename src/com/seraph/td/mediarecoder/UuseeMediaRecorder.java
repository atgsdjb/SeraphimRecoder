package com.seraph.td.mediarecoder;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import com.seraph.td.mediarecoder.R;
import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.media.CamcorderProfile;
import android.media.MediaRecorder;
import android.net.LocalServerSocket;
import android.net.LocalSocketAddress;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.Window;
import android.widget.Adapter;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.MediaController;
import android.widget.Spinner;
import android.widget.Toast;
import android.widget.AdapterView.OnItemSelectedListener;

public class UuseeMediaRecorder extends Activity implements 
									  SurfaceHolder.Callback ,
									  MediaRecorder.OnInfoListener,
									  MediaRecorder.OnErrorListener
{
	private Button button_start;
	private Button button_stop;
	private Button button_back;
	private SurfaceView surfaceView;
	private SurfaceHolder surfaceHolder;
	private File storageDir;
	private MediaRecorder mediaRecorder;
	private Camera camera;
	private Spinner spinner;
	private int width;
	private int height;
	private Adapter mAdapter;
	private LocalServerSocket mServerSocket;
	private final String address = "com.seraph.td";
	private LocalSocketAddress mSocketAddress = new LocalSocketAddress(address);
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.requestWindowFeature(Window.FEATURE_NO_TITLE); // no title
        //screen orientation landscape
		this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        setContentView(R.layout.main);
        
        button_start = (Button)this.findViewById(R.id.button_start);
        button_stop = (Button)this.findViewById(R.id.button_stop);
        button_back = (Button)this.findViewById(R.id.button_back);
        surfaceView = (SurfaceView)this.findViewById(R.id.surfaceView);
        surfaceHolder = surfaceView.getHolder();
        surfaceHolder.addCallback(UuseeMediaRecorder.this);
        surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
        spinner = (Spinner)this.findViewById(R.id.spinner_size);
        width = 480;
        height = 320;
        button_start.setOnClickListener(btnStartListener);
        button_stop.setOnClickListener(btnStopListener);
        button_back.setOnClickListener(btnBackListener);
        spinner.setOnItemSelectedListener(spinnerListener);
        boolean sdCardExist = Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED);
        if (sdCardExist) {
        	storageDir = new File("/mnt/sdcard/seraphim/"); //;new File(Environment.getExternalStorageDirectory().toString() + File.separator + "seraphim" + File.separator);
        	if (!storageDir.exists()) {
        		storageDir.mkdir();
        	}
        	button_stop.setVisibility(View.GONE);
        } else {
        	button_start.setVisibility(View.GONE);
        	Toast.makeText(UuseeMediaRecorder.this, "SDcard is not exist", Toast.LENGTH_LONG).show();
        }
	}
    
    private View.OnClickListener btnStartListener = new View.OnClickListener() {
		
		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			
			button_start.setVisibility(View.GONE);
			button_stop.setVisibility(View.VISIBLE);
			spinner.setVisibility(View.GONE);
			try {
				mediaRecorder = new MediaRecorder();
				mediaRecorder.setOnErrorListener(UuseeMediaRecorder.this);
				mediaRecorder.setOnInfoListener(UuseeMediaRecorder.this);
		        camera.unlock();
		        mediaRecorder.setCamera(camera);


		        mediaRecorder.setAudioSource(MediaRecorder.AudioSource.CAMCORDER);
		        mediaRecorder.setVideoSource(MediaRecorder.VideoSource.CAMERA);
		        CamcorderProfile camcorderProfile = CamcorderProfile.get(CamcorderProfile.QUALITY_HIGH);
		        camcorderProfile.videoFrameWidth = 640;
		        camcorderProfile.videoFrameHeight = 480;
		        camcorderProfile.videoCodec = MediaRecorder.VideoEncoder.H264;
		        camcorderProfile.fileFormat = MediaRecorder.OutputFormat.MPEG_4;
		        mediaRecorder.setProfile(camcorderProfile);
				mediaRecorder.setVideoSize(width, height);
				mediaRecorder.setVideoFrameRate(16);
				mediaRecorder.setPreviewDisplay(surfaceHolder.getSurface());
				mediaRecorder.setOutputFile("/mnt/sdcard/seraphim/test.mp4");
				mediaRecorder.setMaxDuration(10000);
				mediaRecorder.prepare();
				mediaRecorder.start();
			} catch (IllegalStateException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}catch(Exception e){
				Log.e(TAG,"msg ====" + e.getMessage());
			}
		}
	}; 
	
	private View.OnClickListener btnStopListener = new View.OnClickListener() {
		
		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			button_start.setVisibility(View.VISIBLE);
			button_stop.setVisibility(View.GONE);
			spinner.setVisibility(View.VISIBLE);
			if (mediaRecorder != null) {
//				mediaRecorder.set
				File file = new File("");
//				int fd = file.
				try {
				camera.stopPreview();
				mediaRecorder.stop();
				mediaRecorder.release();
				} catch(RuntimeException e) {
					 Log.e("com.seraph.td.mediaRecorder",e.getMessage());
				}
			}
		}
	};
	
	private View.OnClickListener btnBackListener = new View.OnClickListener() {
		
		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			finish();
		}
	};
	
	private OnItemSelectedListener spinnerListener = new Spinner.OnItemSelectedListener() {

		@Override
		public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,
				long arg3) {
			// TODO Auto-generated method stub
//			Spinner spinner = (Spinner)arg0;
//			String wh[] = spinner.getSelectedItem().toString().split("\\*");
//			width = Integer.parseInt(wh[0]);
//			height = Integer.parseInt(wh[1]);
//			try {
//				camera.lock();
//				camera.stopPreview();
//				Camera.Parameters para = camera.getParameters();
//				para.setPreviewSize(width, height);
//				camera.setParameters(para);
//				camera.setPreviewDisplay(surfaceHolder);
//				camera.startPreview();
//				Log.d("Demo", camera.getParameters().getPreviewSize().width + "*" 
//				+ camera.getParameters().getPreviewSize().height);
//
//				camera.unlock();
//			} catch (IOException e) {
//				// TODO Auto-generated catch block
//				Log.e("Demo", e.getMessage());
//			}
		}

		@Override
		public void onNothingSelected(AdapterView<?> arg0) {
			// TODO Auto-generated method stub
			
		}
		
	};
	private String TAG = "com.seraph.td.mediaRecorder";
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		// TODO Auto-generated method stub
		try{
//			camera = Camera.open();//2.2 无参数
			camera = Camera.open(0);//2.3 参数
			List<Size> supportedSize = camera.getParameters().getSupportedPreviewSizes();
			if (supportedSize != null) {
				List<String> list = new ArrayList<String>();
				for (Size s: supportedSize) {
					list.add(s.width + "*" + s.height);
				}
				list.remove(0); // 在使用G7的摄像头支持的最大分辨率进行拍摄的时候会出现stop failed错误，无法关闭MediaRecorder，原因不明！
				ArrayAdapter<String> adapter = new ArrayAdapter<String>(UuseeMediaRecorder.this, 
						android.R.layout.simple_spinner_dropdown_item, list);
				spinner.setAdapter(adapter);
			}
		}catch (Exception e) {
			// TODO: handle exception
			Log.e(TAG ,"OPEN CAMERA ERROR  +"+e.getMessage());
		}

		
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		// TODO Auto-generated method stub
		if(camera != null)
			camera.release();
	}

	@Override
	public void onInfo(MediaRecorder arg0, int arg1, int arg2) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onError(MediaRecorder mr, int what, int extra) {
		// TODO Auto-generated method stub
		
	}
	class  SMediaControl extends MediaController{

		public SMediaControl(Context context) {
			super(context);
			// TODO Auto-generated constructor stub
		}
		
	}
	
	
}