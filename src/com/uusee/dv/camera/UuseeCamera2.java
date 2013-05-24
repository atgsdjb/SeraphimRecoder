package com.uusee.dv.camera;

import android.hardware.Camera;
import android.view.SurfaceHolder;

public class UuseeCamera2 implements Camera.PreviewCallback,Camera.ErrorCallback , UuseeYVUWriter.CompleteListener{
 /**
  * 
  */
	private UuseeYVUWriter mWriter;
	private Camera mCamera;
	private Camera.Parameters mParameters;
//	private 
	private SurfaceHolder mHolder;
	public UuseeCamera2(Camera mCamera,SurfaceHolder mHolder,Camera.Parameters mParameters){
		this.mCamera =mCamera;
		this.mHolder = mHolder;
		this.mParameters = mParameters;
		this.mCamera .setParameters(mParameters);
		mWriter = new UuseeYVUWriter("/mnt/sdcard/raw.data", this);
	}
	public UuseeCamera2(Camera mCamera,SurfaceHolder mHolder){
		this.mCamera = mCamera;
		this.mHolder = mHolder;
		defaultInit();
	}
	public UuseeCamera2(Camera mCamera){
		this.mCamera = mCamera;
		defaultInit();
	}

@Override
public void onError(int error, Camera camera) {
	// TODO Auto-generated method stub
	
}

@Override
public void onPreviewFrame(byte[] data, Camera camera) {
	// TODO Auto-generated method stub
	
}
public void startPreView(){
	
}
public void stopPreView(){
	
}

public SurfaceHolder getmHolder() {
	return mHolder;
}

public void setmHolder(SurfaceHolder mHolder) {
	this.mHolder = mHolder;
}
private void defaultInit() {
	// TODO Auto-generated method stub
	
}
@Override
public void OnComplente() {
	// TODO Auto-generated method stub
	
}
}
