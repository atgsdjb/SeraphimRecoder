package com.seraphim.td.nativ;
/**
MPEG ID's 
#define MPEG2 1
#define MPEG4 0

// AAC object types 
#define MAIN 1
#define LOW  2
#define SSR  3
#define LTP  4

// Input Formats
#define FAAC_INPUT_NULL    0
#define FAAC_INPUT_16BIT   1
#define FAAC_INPUT_24BIT   2
#define FAAC_INPUT_32BIT   3
#define FAAC_INPUT_FLOAT   4
 * @author root
 *
 */
public class FAACNative {
	//mpegID's 
	public static final int MPEG2 = 1;
	public static final int MPEG4 = 0;
	
	
	//object type
	public static final int MAIN =1;
	public static final int LOW =2;
	public static final int SSR = 3;
	public static final int LTP =4;
	
	
	//input format 
	public static final int FAAC_INPUT_NULL= 0;
	public static final int FAAC_INPUT_16BIT=1;
	public static final int FAAC_INPUT_24BIT=2;
	public static final int FAAC_INPUT_32BIT=3;
	public static final int FAAC_INPUT_FLOAT =4;
	
	public static native void faacInit(int aacObjectType ,int sampleRate,int bitRate,int MPEGVesrsion,int inputFormat);
	public static native void addPCMSample(byte[] sample,int len);
	
	
}
