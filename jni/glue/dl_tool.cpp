#include<dlfcn.h>
#include<dl_tool.h>
#include<stdlib.h>
#include<android/log.h>
#include<up_task.h>
#include<errno.h>
#include"us_log.h"
//com.seraph.td.mediarecoder
const char *LIB_PATH="/data/data/com.seraph.td.mediarecoder/lib/libus_upload.0.2.so";
//'					  /data/data/com.uusee.mini/lib/libus_upload.0.2.so
static void* g_handle ;
typedef int (*fInit)(UUSee_Upload_CallBack callback);
typedef int (*fquit)(void);
typedef int (*fstart)(int type,const char* channelId,const char* filepath,int startunp,int endunp,const char* serverip,int serverport);
typedef int (*fstop)(const char* channelId);
typedef int (*fdelete)(const char*   channelId,const char* filepath);

void* upload_module_init(const char* path){
	td_printf("------------upload_module_init--------------0---------\n");
	g_handle =  dlopen(LIB_PATH, RTLD_LAZY);
	td_printf("------------upload_module_init--------------1--------\n");
	fInit f =(fInit) dlsym(g_handle,"UUSee_Upload_init");
	td_printf("------------upload_module_init--------------2---------\n");
	if(g_handle == NULL){
		const char* temp = dlerror();
		__android_log_write(ANDROID_LOG_ERROR,"","LOADING LIBRARY ERROR");
		char ts[1024] = {0};
		if(temp!=NULL){
			sprintf(ts,"Seraphim---error msg====%s",temp);
					__android_log_write(ANDROID_LOG_ERROR,"",ts);
		}else{
			__android_log_write(ANDROID_LOG_ERROR,"","MSG  =====nnnnnnnnnnnnnUuuuuuuuuuuuuullllllllllll");
		}


	}else{
		__android_log_write(ANDROID_LOG_ERROR,""," LIBRARY OKKKKK");
	}
	td_printf("------------upload_module_init--------------3---------\n");
	f(taskProcess);
	td_printf("------------upload_module_init--------------4---------\n");
	return NULL;
}
void upload_module_quit(){
 if(g_handle == NULL)
	 return;
 fquit f = (fquit)dlsym(g_handle,"UUSee_Upload_quit");
 f();
}

int upload_module_start(int type,const char* channelld,const char* filepath,int startunp,int endunp,const char* serverip,int serverport){
	td_printf("-------------------------upload_module_start----------0----------------\n");
	if(g_handle==NULL){
		td_printf("----------g_handler==NULL---------\n");
		return -1;
	}
	fstart f =(fstart) dlsym(g_handle,"UUSee_Upload_start");
	int result = f(type,channelld,filepath,startunp,endunp,serverip,serverport);
	td_printf("-------------------------upload_module_start----------code = %d ---------------\n",result);
	return result;
}

int upload_module_stop(const char* channelid){
	if(g_handle == NULL)
		return -1;
	fstop f =(fstop) dlsym(g_handle,"UUSee_Upload_stop");

	return f(channelid);
}

int upload_module_delete(const char* channelid,const char* filepath){
	return 0;
}

static void taskProcess(char* channelld,UUSee_Event event,void* content){
		td_printf("------------------%s-----------------\n",channelld);
}
