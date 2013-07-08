package com.seraphim.td.omx;

import java.io.IOException;

public  interface QSink {
	static final String TAG="SERAPHIM";
	void close();
	void flush();
	boolean write(byte[] data, int offset, int len, int trackID)
			throws IOException;
	
}
