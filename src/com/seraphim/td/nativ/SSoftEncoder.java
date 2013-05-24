package com.seraphim.td.nativ;

public class SSoftEncoder {
	public static native void start(
							int widthView,
							int hegihtView,
							int kVideoBitRate,
							int kIFramesIntervalSec,
							int kFramerate,
							int countFrameOfFile);
	public static native void stop();
	public static native void add(byte[] data ,int size );
	public static native void addAudio(short[] data,int size);

}
