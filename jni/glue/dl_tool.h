#ifndef _TD_SO_TOOL_H
#define _TD_SO_TOOL_H
#include<dlfcn.h>
#include"up_task.h"
#ifdef __cplusplus
extern "C" {
#endif

//	typedef void (* UUSee_Upload_CallBack)(char *channelId,UUSee_Event eventcode,void* content);
//	int UUSee_Upload_init(UUSee_Upload_CallBack callback);
//	int UUSee_Upload_quit();
//	int UUSee_Upload_start(int type,const char* channelId,const char* filepath,int startunp,int endunp,const char* serverip,int serverport);
//	int UUSee_Upload_stop(const char* channelId);
//	int UUSee_Channel_delete(const char*   channelId,const char* filepath);



void* upload_module_init(const char* path);
void upload_module_quit();
int upload_module_start(int type,const char* channelld,const char* filepath,int startunp,int endunp,const char* serverip,int serverport);
int upload_module_stop(const char* channelid);
int upload_module_delete(const char* channelid,const char* filepath);
static void taskProcess(char* channelld,UUSee_Event,void* content);//{
#ifdef __cplusplus
}
#endif
#endif
