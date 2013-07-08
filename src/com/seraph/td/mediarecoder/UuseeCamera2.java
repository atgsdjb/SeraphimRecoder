package com.seraph.td.mediarecoder;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.Arrays;
//import java.util.Arrays;
import java.util.List;

import com.seraphim.td.omx.QCodecSink;
import com.seraphim.td.omx.QSink;
import com.uusee.dv.camera.UuseeYVUWriter;
import android.content.Context;
import android.content.res.Configuration;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.media.MediaCodecInfo;
import android.media.MediaFormat;
import android.os.Environment;
import android.util.Log;
import android.view.SurfaceHolder;
/**
 *  Carmera 封装工具类
 * @author root
 * 功能 ： 1）Carmera 初始化 2）Preview 3）存贮YUV420SP原始格式文件 4）错误异常处理  5）内存管理 
 * 接口：
 *
 */
public class UuseeCamera2  implements Camera.PreviewCallback,Camera.ErrorCallback,UuseeYVUWriter.CompleteListener  {
 private Camera camera;
 
 @SuppressWarnings("unused")
 private Camera.Parameters mParameters;
 private Context context;
 private final String TAG = "com.uusee.pp.UuseeCamera";
 private int mPreviewWidth = -1;
 private int mPreviewHeight = -1;
 private boolean isPreview = false;
// private UuseeYVUWriter mWriter;
 private QCodecSink mEncoder;
 private QSink mQSink;
 private QCodecSink fileSink;
 
// private QSink   fileSink;
 
/***
 * 测试数据
 * @param context
 * @param cameraId
 */
 public UuseeCamera2(Context context, int cameraId,int width,int height,QSink sink){
	 	this.context = context;
	 	this.camera = Camera.open(cameraId);
		setmPreviewWidth(width);
		setmPreviewHeight(height);
		 initCamera();
		 mQSink = sink;
		 createSink();
		 createFile();
	}
 File f;
 OutputStream out;
 private  void createFile(){
		try{
		f = new File(Environment.getExternalStorageDirectory().getPath()+"/test_w640_h480_f2000.yuv");
		if(f.exists()){
			f.delete();
		}
		f.createNewFile();
		out = new FileOutputStream(f);
	}catch(Exception e){
		
	}
 }
 private void createSink(){
	    MediaFormat mediaFormat = MediaFormat.createVideoFormat("video/avc", getmPreviewWidth(),getmPreviewHeight());
	    mediaFormat.setInteger(MediaFormat.KEY_BIT_RATE, 500000);
	    mediaFormat.setInteger(MediaFormat.KEY_FRAME_RATE, 15);
	    mediaFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT, MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420Planar);
	    mediaFormat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, 5);
		try{
	
			mEncoder = new QCodecSink("","OMX.Nvidia.h264.encoder", mediaFormat,0,mQSink);
		}catch(Exception e){
			Log.e(TAG,e.getMessage());
		}
		
 }
private void initCamera() {
	// TODO Auto-generated method stub
	   Log.i(TAG, "going into initCamera");
	   if(null != camera)
	   {
		 camera.setErrorCallback(this);
	     camera.setPreviewCallback(this);
	     camera. setPreviewCallbackWithBuffer(this);
	     Camera.Parameters parameters = camera.getParameters();
	     /**
	      * 通用设置
	      * 
	      */
		 // 横竖屏镜头自动调整
	     if (context.getResources().getConfiguration().orientation != Configuration.ORIENTATION_LANDSCAPE) 
	     {
//	      parameters.set("orientation", "portrait"); //
//	      parameters.set("rotation", 180); // 镜头角度转90度（默认摄像头是横拍） 
//	      camera.setDisplayOrientation(180); // 在2.2以上可以使用
//	      Log.e(TAG,"origntation=portrait");
	     } else// 如果是横屏
	     {
//	      parameters.set("orientation", "landscape"); //
//	    	 parameters.set("rotation", 180);
//	      camera.setDisplayOrientation(180); // 在2.2以上可以使用
//	      camera.setDisplayOrientation(180); // 在2.2以上可以使用
//	      Log.e(TAG,"origntation=landscape");
	     } 
	     /**
	      * preview  设置
	      */
	     parameters.setPreviewFormat(ImageFormat.NV21);
//	     parameters.setPreviewFpsRange(15, 20);
	     parameters.setPreviewFrameRate(15);
	     parameters.setPreviewSize(mPreviewWidth, mPreviewHeight);
	     /**
	      * picture 设置
	      */
	     camera.setParameters(parameters); // 将Camera.Parameters设定予Camera    
	     // 【调试】设置后的图片大小和预览大小以及帧率
	     
	   }
	
}
/**
 * 
 */
@SuppressWarnings("unused")
private Size getMiniSize(List<Size> sizeList) {
	// TODO Auto-generated method stub
	Size size = sizeList.get(0);
	for(int i = 1; i <sizeList.size() ;i++){
		if(size.width  >  sizeList.get(i).width) size = sizeList.get(i); 
	}
	return size;
}
/**
 * 取，LIST中，size的宽度最大的对象
 * @param previewSizes
 * @return  Size
 */
@SuppressWarnings("unused")
private Size getMaxSize(List<Size> sizeList) {
	// TODO Auto-generated method stub
	Size size = sizeList.get(0);
	for(int i = 1; i <sizeList.size() ;i++){
		if(size.width  <  sizeList.get(i).width) size = sizeList.get(i); 
	}
	return size;
}
public void startPreview() {
	isPreview = true;
	camera.startPreview();
	mEncoder.start();
}

public void stopPreview(){
	isPreview = false;
//	camera.stopPreview();
	close();
	mEncoder.stop();
}
public void  release(){
	if(isPreview) stopPreview();
	camera.release();
}
public void setPreviewDisplay(SurfaceHolder mHolder) {
	// TODO Auto-generated method stub
	try {
		this.camera.setPreviewDisplay(mHolder);
		this.camera.setPreviewCallback(this);
	} catch (IOException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	}
}
int i = 0;
@Override
public void onPreviewFrame(byte[] data, Camera camera) {
	// TODO Auto-generated method stub
	byte[] ld=Arrays.copyOf(data, data.length);
	
	try {
		//duo yu de canshu  TrackID;
		mEncoder.write(ld, 0, ld.length,0);
	} catch (IOException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
		Log.e(TAG,"form camera  write  into Encoder occur eror msg="+e.getMessage());
	}

}
@Override
public void onError(int error, Camera camera) {
	// TODO Auto-generated method stub
	Log.e(TAG,"write  data size"+error);
}
public class NullHolderException extends Exception{

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	@Override
	public String getMessage() {
		// TODO Auto-generated method stub
		return " SurfaceHolder is null , please Call startPreview(SurfaceHolder )";
	}
}

private BufferedOutputStream outputStream;


public void close() {
    try {
    	mEncoder.stop();
//        mediaCodec.stop();
//        mediaCodec.release();
        outputStream.flush();
        outputStream.close();
    } catch (Exception e){ 
        e.printStackTrace();
    }
}



// called from Camera.setPreviewCallbackWithBuffer(...) in other class
static int numberIN=0;
static int numberOU=0;



@Override
public void OnComplente() {
	// TODO Auto-generated method stub
	
}
public int getmPreviewWidth() {
	return mPreviewWidth;
}
private void setmPreviewWidth(int mPreviewWidth) {
	this.mPreviewWidth = mPreviewWidth;
}
public int getmPreviewHeight() {
	return mPreviewHeight;
}
private void setmPreviewHeight(int mPreviewHeight) {
	this.mPreviewHeight = mPreviewHeight;
}
}
