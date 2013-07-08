package com.seraph.td.mediarecoder;
 
import java.io.IOException;
import java.nio.ByteBuffer;
// 
import android.media.MediaCodec;
import android.media.MediaCodecInfo;
import android.media.MediaFormat;
 
public class AvcEncoder {
 
	private MediaCodec mediaCodec;
	
	private byte[] sps;
	private byte[] pps;
//	private ParameterSetsListener parameterSetsListener;
 
	public AvcEncoder() {
		mediaCodec = MediaCodec.createEncoderByType("video/avc");
		MediaFormat mediaFormat = MediaFormat.createVideoFormat("video/avc", 320, 240);
		mediaFormat.setInteger(MediaFormat.KEY_BIT_RATE, 125000);
		mediaFormat.setInteger(MediaFormat.KEY_FRAME_RATE, 15);
		mediaFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT, MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420Planar);
		mediaFormat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, 5);
		mediaCodec.configure(mediaFormat, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);
		mediaCodec.start();
	}
 
	public void close() throws IOException {
		mediaCodec.stop();
		mediaCodec.release();
	}
 
	public void offerEncoder(byte[] input) {
		try {
			ByteBuffer[] inputBuffers = mediaCodec.getInputBuffers();
			ByteBuffer[] outputBuffers = mediaCodec.getOutputBuffers();
			int inputBufferIndex = mediaCodec.dequeueInputBuffer(-1);
			if (inputBufferIndex >= 0) {
				ByteBuffer inputBuffer = inputBuffers[inputBufferIndex];
				inputBuffer.clear();
				inputBuffer.put(input);
				mediaCodec.queueInputBuffer(inputBufferIndex, 0, input.length, 0, 0);
			}
 
			MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();
			int outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 0);
			while (outputBufferIndex >= 0) {
				ByteBuffer outputBuffer = outputBuffers[outputBufferIndex];
				byte[] outData = new byte[bufferInfo.size];
				outputBuffer.get(outData);
				if (sps != null && pps != null) {
					ByteBuffer frameBuffer = ByteBuffer.wrap(outData);
					frameBuffer.putInt(bufferInfo.size - 4);
//					frameListener.frameReceived(outData, 0, outData.length);
				} else {
					ByteBuffer spsPpsBuffer = ByteBuffer.wrap(outData);
					if (spsPpsBuffer.getInt() == 0x00000001) {
						System.out.println("parsing sps/pps");
					} else {
						System.out.println("something is amiss?");
					}
					int ppsIndex = 0;
//					while(!(spsPpsBuffer.get() == 0x00 && spsPpsBuffer.get() == 0x00 && iPpsBuffer.get() == 0x00 && spsPpsBuffer.get() == 0x01)) {
// 
//					}
					ppsIndex = spsPpsBuffer.position();
					sps = new byte[ppsIndex - 8];
					System.arraycopy(outData, 4, sps, 0, sps.length);
					pps = new byte[outData.length - ppsIndex];
					System.arraycopy(outData, ppsIndex, pps, 0, pps.length);
//					if (null != parameterSetsListener) {
//						parameterSetsListener.avcParametersSetsEstablished(sps, pps);
//					}
				}
				mediaCodec.releaseOutputBuffer(outputBufferIndex, false);
				outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 0);
			}
		} catch (Throwable t) {
			t.printStackTrace();
		}
 
	}
}