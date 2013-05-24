package com.seraph.td.mediarecoder;

import java.io.IOException; 

import android.content.Context; 
import android.graphics.Canvas; 
import android.hardware.Camera; 
import android.hardware.Camera.PreviewCallback; 
import android.util.AttributeSet; 
import android.util.Log; 
import android.view.SurfaceHolder; 
import android.view.SurfaceView; 

public class CamPreview extends SurfaceView implements SurfaceHolder.Callback { 
Camera mCamera; 
SurfaceHolder mHolder; 

public CamPreview(final Context context, final AttributeSet attrs) { 
 super(context, attrs); 
 Log.d("METHODCALL", "CamPreview"); 
 this.mHolder = this.getHolder(); 
 this.mHolder.addCallback(this); 
 this.mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS); 
} 

public void surfaceCreated(final SurfaceHolder holder) { 
 Log.d("METHODCALL", "surfaceCreated"); 
 // The Surface has been created, acquire the camera and tell it where 
 // to draw. 
 this.mCamera = Camera.open(); 
 try { 
  this.mCamera.setPreviewDisplay(holder); 

  this.mCamera.setPreviewCallback(new PreviewCallback() { 

   public void onPreviewFrame(final byte[] data, final Camera arg1) { 
    Log.d("METHODCALL", "onPreviewFrame"); 
   } 
  }); 
 } catch (final IOException e) { 
  e.printStackTrace(); 
 } 
} 

public void surfaceDestroyed(final SurfaceHolder holder) { 
 Log.d("METHODCALL", "surfaceDestroyed"); 
 // Surface will be destroyed when we return, so stop the preview. 
 // Because the CameraDevice object is not a shared resource, it's very 
 // important to release it when the activity is paused. 
 this.mCamera.stopPreview(); 
 this.mCamera = null; 
} 

public void surfaceChanged(final SurfaceHolder holder, final int format, 
  final int w, final int h) { 
 Log.d("METHODCALL", "surfaceChanged"); 
 // Now that the size is known, set up the camera parameters and begin 
 // the preview. 
 // final Camera.Parameters parameters = this.mCamera.getParameters(); 
 // parameters.setPreviewSize(w, h); 
 // this.mCamera.setParameters(parameters); 
 this.mCamera.startPreview(); 
} 

@Override 
public void onDraw(final Canvas canvas) { 
 Log.d("METHODCALL", "onDraw"); 
} 
}