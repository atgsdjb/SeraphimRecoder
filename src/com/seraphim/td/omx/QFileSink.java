package com.seraphim.td.omx;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.util.Log;

public class QFileSink implements QSink {
	private final String TAG= "com.seraphim.td.omx.QFileSink";
	private File file;
	private OutputStream out;
	private boolean isOpen = false;
	public QFileSink(String fileName) throws  IOException{
		file = new File(fileName);
		out = new FileOutputStream(file);
		isOpen =true;
	}
	

	@Override
	public boolean write(byte[] data, int offset, int len,int trackID)throws IOException {
		// TODO Auto-generated method stub
		 boolean result = false;
		if(isOpen){
			 out.write(data, 0, len);
			 out.flush();
			 result = true;
			 
		}
		return result;
	}

	/**
	 * @param args
	 */
	public void flush(){
		try {
			out.flush();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	public void close(){
		try {
			out.flush();
			out.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			Log.e(TAG,e.getMessage());
		}
	}



}
