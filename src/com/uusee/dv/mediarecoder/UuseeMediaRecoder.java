package com.uusee.dv.mediarecoder;

import java.util.Timer;
import java.util.TimerTask;

import android.content.Context;
import android.hardware.Camera;
import android.media.MediaRecorder;
import android.net.LocalServerSocket;
import android.net.LocalSocket;
/**
 * 
 * @author root
 * 1） 设计目标
 *用H264-MP4格式编码摄像头采集的数据.
 *每隔S秒处理一次，处理的结果从LocalSocket 里取出，传递给C模块
 *测试情况，最终数据传给SOCKET发送到PC
 *2)外部接口
 */
public class UuseeMediaRecoder {
	private Context mContext;
	private MediaRecorder mRecorder;
	private Camera mCamera;
	private LocalServerSocket mLServerSoclet;
	private LocalSocket mLSocket;
	private Timer  mTimer;
	private TimerTask mTimerTask;
	
}
