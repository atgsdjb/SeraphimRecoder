package com.uusee.dv.camera;

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;

import android.util.Log;

public class UuseeYVUWriter {
	private static final String TAG = "com.seraphim.td.UuseeYUVWritert";
	private String fileName;
	private File mFile;
	private OutputStream mOut;
	private boolean runFlg = false;
	private CompleteListener mListener;
	private DataQueue mQueue = new DataQueue();
	public UuseeYVUWriter(String name,CompleteListener listener){
		this.fileName = name;
		mListener =  listener;
	}
	private Thread workThread = new Thread(){

		@Override
		public void run() {
			// TODO Auto-generated method stub
			super.run();
			while(runFlg){
				try{
					
					while(mQueue.getSize() >0 ){
						byte[] data = getData();
						mOut.write(data);
					}
				}catch (Exception e) {
					// TODO: handle exception
					Log.e(TAG,e.getMessage());
				}
			}
		}
		
	};
	synchronized public  void start(){
		if(runFlg) return;
		runFlg =true;
		mFile = new File(fileName);
		try{
			mOut  = new FileOutputStream(mFile);
			workThread.start();
		}catch (Exception e) {
			// TODO: handle exception
		}

	}
	synchronized public void upDate(byte[] data){
		mQueue.add(data);
	}
	synchronized public void stop(){
		if(!runFlg) return;
		runFlg = false;
		try{
			mOut.flush();
			mOut.close();
		}catch (Exception e) {
			// TODO: handle exception
		}
	}
	synchronized private byte[] getData(){
		return mQueue.get();
	}
	private class DataQueue{
		 List<byte[]> buffer = new ArrayList<byte[]>();
		 int count = 0;
		 
		synchronized void add(byte[] data){
			buffer.add(data);
			count++;
		 }
		synchronized byte[] get(){
			byte[] result = null;
			if(count > 0){
				count--;
				result = buffer.get(0);
				buffer.remove(0);
			}
			
			 return result;
		 }
		synchronized int getSize(){
			return count;
		}
	}
	public interface CompleteListener{
		void OnComplente();
	}
	//简单读写测试
	public void testWrite(){
		byte[] data = new byte[10];
		//write
		/**seraph_test**/
//		int length = 320*240*3/2;
		for(int i = 1;i<=10; i++){
			for(int j = 0;j<data.length;j++){
				byte d = (byte)i;
				data[j] = d;
			}
			mQueue.add(data);
		}
		//read
		while(mQueue.getSize() > 0){
			byte[] result = mQueue.get();
			if(result == null) break;
			for(int i = 0 ; i<result.length;i++)
				System.out.println("current size ==" + (mQueue.getSize()+1)+"\t str==="+result[i]);
		}
	}
}
