/*
 *  us_task.h
 *  UUSeeCast
 *
 *  Created by wuwg on 10-12-30.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _UP_TASK_H_
#define _UP_TASK_H_



#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <stdio.h>
#include<pthread.h>
#include"i_live_upload_wrapper.h"
	
	
//最大支持上传任务数	
#define UUSEE_TASK_MAXNUM	6

	//事件码定义
	typedef enum {
		UUSEE_GUID_GET_SUCCESS = 1,//成功获取guid
		UUSEE_GUID_GET_FAILED = 2,//获取guid失败
		UUSEE_RECORD_STRAT_SUCESS = 3,//开始采集视音频数据
		UUSEE_RECORD_START_FAILED = 4, //采集视音频数据失败
		UUSEE_UPLOAD_START_SUCESS = 5, //开始上传数据
		UUSEE_UPLOAD_START_FAILED = 6,//开始上传数据失败，该事件不影响用户继续采集数据，单独上传时需页面做提示处理
		UUSEE_UPLOAD_DISCONNECTED = 7,//上传连接异常，该事件不影响用户继续采集数据，单独上传时需页面做提示处理
		UUSEE_RECOARD_UNP_ID = 8,//采集生成的unp包号
		UUSEE_UPLOAD_UNP_ID = 9,//上传成功的unp包号
		UUSEE_UPLOAD_UNP_RATE = 10,//上传速率，单位k
		UUSEE_UPLOAD_COMPLETION = 11,//完成上传
		UUSEE_RECORD_STOP = 12,//停止采集
		UUSEE_RECORD_RETURN = 13,//退出采集画面
	} UUSee_Event;
	typedef void (* UUSee_Upload_CallBack)(char *channelId,UUSee_Event eventcode,void* content);
	
	//任务状态码定义
	enum {
		UUSEE_IDLE = 0,//空闲
		UUSEE_WORK, //工作
		UUSEE_DESTROY,
	};
//#pragma pack(push, 1)
	//转unp任务结构
	typedef struct tag_UUSee_UnpTask_s
	{
		int status;//任务状态
		int fileid;//目前正在处理的文件序号
		int unpid;//处理的unp序号
		char channelId[37];//该任务对应的channel id
		char filepath[256];//初始文件名
		pthread_t unpthread;//unp任务对应的线程
	} UUSee_Unp_Task_s;

	//上传任务结构
	typedef struct tag_UUSee_UploadTask_s
	{
		int type;//类型1:直播 2：点播 3:纯上传
		int status;//任务状态
		int fileid;//目前正在处理的文件序号
		int sendedlen;//目前已经上传的长度
		int startunp;//unp文件的开始号
		int endunp;//unp文件的结束号
		int serverport;//server端口号
		void *uhandle;//上传任务句柄
		pthread_cond_t threadCond;//任务条件变量
		pthread_mutex_t threadMutex;//任务锁
		char channelId[37];//该任务对应的channel id
		char filepath[256];//初始文件名
		pthread_t upthread;//上传任务对应的线程
		char serverip[20];//server ip地址
	} UUSee_Upload_Task_s;
	
	//上传全局控制结构
	typedef struct tag_UUSee_Task_Control_s
	{
		UUSee_Upload_Task_s taskList[UUSEE_TASK_MAXNUM];//上传任务列表
		UUSee_Unp_Task_s unpTask;//转unp任务
		UUSee_Upload_CallBack uploadCallBack;//回调函数
	}UUSee_Task_Control_s;
//#pragma pack(pop)
	

	//外部函数
	int UUSee_Upload_init(UUSee_Upload_CallBack callback);
	int UUSee_Upload_quit();
	int UUSee_Upload_start(int type,const char* channelId,const char* filepath,int startunp,int endunp,const char* serverip,int serverport);
	int UUSee_Upload_stop(const char* channelId);
	int UUSee_Channel_delete(const char* channelId,const char* filepath);

	
	//内部函数
	static UUSee_Upload_Task_s* getUploadTask(const char *channelId);
	static UUSee_Upload_Task_s* getUploadTaskByHandle(void *uhandle);
	static void* upload_task_run(void* id);
	static void* unp_task_run(void* id);
	static int GUIDString(char *guid);
	
	int upload_callback ( void * hTask, enumLiveUploadWrapperState eCallbackState, int nSendLen, const char * lpszUnpFileName );


#ifdef __cplusplus
}
#endif

#endif
