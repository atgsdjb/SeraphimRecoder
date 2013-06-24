package com.seraphim.td.tools;

import java.io.IOException;

import org.apache.http.HttpException;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;



/**
 * NSString* strurl = [NSString stringWithFormat:@"http://home.uusee.com/app/user
 * 								/userchannel/?userId=%@&nickName=%@&title=%@&deviceName=%@&deviceOS=%@&deviceSerialNo=%@&longitude=%@&latitude=%@",
						self.username,self.nickname,self.channelTitle,deviceName,@"iphoneOS",deviceSerial,self.longitude,self.latitude];
 * @author root
 *
 */

public class UusseGetGUID {
	
	private String userName = "seraphim";
	private String nickName = "android";
	private String channelTitle ="mytest";
	private String deviceName ="nexus7";
	private String system = "linux-android";    //system
	private String deviceSerial ="0000000000";
	private String longitude ="135";  //int
	private String latitude ="148" ;   //int
	
	public String getGUID(){
		String  url = String.format("http://home.uusee.com/app/user" +
					"/userchannel/?userId=%s&nickName=%s&title=%s&deviceName=%s&deviceOS=%s&deviceSerialNo=%s&longitude=%s&latitude=%s",userName,
					nickName,channelTitle,deviceName,system,
					deviceSerial,longitude,latitude);

			String guid = getGuid(url);
			return guid;
	}
	
	
//		public static void  main(String[] argv){
//			System.out.println("-----------------");
//			UusseGetGUID  geter = new UusseGetGUID();
//			String  url = String.format("http://home.uusee.com/app/user" +
//  								"/userchannel/?userId=%s&nickName=%s&title=%s&deviceName=%s&deviceOS=%s&deviceSerialNo=%s&longitude=%s&latitude=%s",geter.userName,
//  								geter.nickName,geter.channelTitle,geter.deviceName,geter.system,
//  								geter.deviceSerial,geter.longitude,geter.latitude);
//			
//			String guid = geter.getGuid(url);
//			if(guid != null)
//				System.out.println(guid);
//		}
		
		
		
		private String getGuid(String url){
			String guid = null;
			HttpClient httpClient = new DefaultHttpClient();//new HttpClient();
			  HttpGet getMethod = new HttpGet(url);
			  
			  try {
			   HttpResponse response = httpClient.execute(getMethod);
			   int statusCode = response.getStatusLine().getStatusCode();
			   if (statusCode != HttpStatus.SC_OK) {
				   
			    return guid;
			   }
//			   int len = 0;
			   byte[] buffer = new byte[1024];
			   int len = response.getEntity().getContent().read(buffer);
			   guid = new String(buffer,0,len);

			  } catch (IOException e) {
			   e.printStackTrace();
			  } finally {
//				  httpClient.getConnectionManager().closeExpiredConnections();
			  }
			return guid;
		}
}
