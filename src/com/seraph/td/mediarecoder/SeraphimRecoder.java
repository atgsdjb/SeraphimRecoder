package com.seraph.td.mediarecoder;

import java.util.List;

import android.app.Activity;
import android.content.Intent;
import android.hardware.Camera;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Spinner;
/**

List<Integer>	 getZoomRatios()
Gets the zoom ratios of all zoom values.
 * @author root
 *
 */
public class SeraphimRecoder extends Activity {
	private int mCameraId;
	private Camera mCamera;
	private ArrayAdapter<String> antibanding;
	private ArrayAdapter<String> colorEffects;
	private ArrayAdapter<String> flashModes;
	private ArrayAdapter<String> focusModes;
	private ArrayAdapter<Camera.Size> jpegThumbnailSize;
	private ArrayAdapter<Integer> pictureFormats;
	private ArrayAdapter<Camera.Size> pictureSizes;
	private ArrayAdapter<Integer> previewFormats;
	private ArrayAdapter<int[]>   previewFpsRange;
	private ArrayAdapter<Camera.Size>  	previewSizes;
	private ArrayAdapter<String>  	sceneModes;
	private ArrayAdapter<Camera.Size>  videoSizes;
	private ArrayAdapter<String> whiteBalance;
	private ArrayAdapter<Integer> zoomRations;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.layout_seraphim_recoder);
		if(initCamera())
			initView();
		
	}
	
	@Override
	protected void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
		if(mCamera != null)
			mCamera.release();
	}

	boolean initView(){
		Spinner spinner ;
		spinner = (Spinner) findViewById(R.id.antibanding);
		spinner.setAdapter(antibanding);
		spinner = (Spinner) findViewById(R.id.colorEffects);
		spinner.setAdapter(colorEffects);
		spinner = (Spinner) findViewById(R.id.flashModes);
		spinner.setAdapter(flashModes);
		spinner = (Spinner) findViewById(R.id.focusModes);
		spinner.setAdapter(focusModes);		
		spinner = (Spinner) findViewById(R.id.jpegThumbnailSize);
		spinner.setAdapter(jpegThumbnailSize);
		spinner = (Spinner) findViewById(R.id.pictureFormats);
		spinner.setAdapter(pictureFormats);
		spinner = (Spinner) findViewById(R.id.pictureSizes);
		spinner.setAdapter(pictureSizes);
		spinner = (Spinner) findViewById(R.id.previewFormats);
		spinner.setAdapter(previewFormats);
		spinner = (Spinner)findViewById(R.id.previewFpsRange);
		spinner.setAdapter(previewFpsRange);
		spinner = (Spinner) findViewById(R.id.previewSizes);
		spinner.setAdapter(previewSizes);
		spinner = (Spinner) findViewById(R.id.sceneModes);
		spinner.setAdapter(sceneModes);
		spinner = (Spinner) findViewById(R.id.videoSizes);
		spinner.setAdapter(videoSizes);
		spinner = (Spinner) findViewById(R.id.whiteBalance);
		spinner.setAdapter(whiteBalance);
		spinner = (Spinner) findViewById(R.id.zoomRations);
		spinner.setAdapter(zoomRations);
		return true;
	}
	
	boolean initCamera(){
		try{
			
			mCamera = Camera.open(0);
			setmCameraId(0);
		}catch(RuntimeException  e){
			try{
				mCamera = Camera.open(1);
				setmCameraId(1);
			}catch(RuntimeException e1){
				return false;
			}
		}
		Camera.Parameters param = mCamera.getParameters();
		antibanding = new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1, param.getSupportedAntibanding());
		colorEffects =new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1, param.getSupportedColorEffects());
		flashModes =new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1, param.getSupportedFlashModes());
		focusModes =new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1, param.getSupportedFocusModes());
		jpegThumbnailSize =new ArrayAdapter<Camera.Size>(this,android.R.layout.simple_list_item_1, param.getSupportedJpegThumbnailSizes());
		pictureFormats =new ArrayAdapter<Integer>(this,android.R.layout.simple_list_item_1, param.getSupportedPictureFormats());
		pictureSizes =new ArrayAdapter<Camera.Size>(this,android.R.layout.simple_list_item_1, param.getSupportedPictureSizes());
		previewFormats =new ArrayAdapter<Integer>(this,android.R.layout.simple_list_item_1, param.getSupportedPreviewFormats());
		previewFpsRange = new ArrayAdapter<int[]>(this,android.R.layout.simple_list_item_1,  param.getSupportedPreviewFpsRange());
		previewSizes = new ArrayAdapter<Camera.Size>(this,android.R.layout.simple_list_item_1,  param.getSupportedPreviewSizes());
		sceneModes = new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1,  param.getSupportedSceneModes());
		videoSizes = new ArrayAdapter<Camera.Size>(this,android.R.layout.simple_list_item_1,  param.getSupportedVideoSizes());
		whiteBalance =new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1,   param.getSupportedWhiteBalance());
		zoomRations =new ArrayAdapter<Integer>(this,android.R.layout.simple_list_item_1,   param.getZoomRatios());
		return true;
	}

	public int getmCameraId() {
		return mCameraId;
	}

	public void setmCameraId(int mCameraId) {
		this.mCameraId = mCameraId;
	}

	public Camera getmCamera() {
		return mCamera;
	}
	public void startActivity(View view){
		Intent intent= new Intent(this,UuseeMediaRecorder.class);
		startActivity(intent);
	}
	
}
