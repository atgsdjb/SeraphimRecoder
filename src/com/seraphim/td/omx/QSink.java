package com.seraphim.td.omx;

import java.io.IOException;

public  interface QSink {
	
	void close();
	void flush();
	boolean write(byte[] data, int offset, int len, int trackID)
			throws IOException;
	
}
