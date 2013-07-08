/*
 *  us_log.c
 *  uuseedown
 *
 *  Created by wuwg on 09-11-19.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "us_log.h"
#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <assert.h>
#include<android/log.h>
#ifdef SHOW_UUSEE_LOG
void getCurTime(char *_cur_time)
{
	struct tm *tt2;
	struct timeval tstruct1;
	struct timezone tzp;
	gettimeofday(&tstruct1,&tzp);
	tt2 = localtime(&tstruct1.tv_sec);
	sprintf(_cur_time,"%04d%02d%02d %02d:%02d:%02d.%06d",
			tt2->tm_year+1900,tt2->tm_mon+1,tt2->tm_mday,tt2->tm_hour,tt2->tm_min,tt2->tm_sec,tstruct1.tv_usec);
}

void ExportLog(const char* str)
{}
void ExportLog2(const char* str)
{}

void UUSee_Printf_Hex(const  char *fragment, unsigned short length, const unsigned char *name)
{
	
    const unsigned char *s;
    const unsigned short data_per_line=16;
    int i, j;
    unsigned char *c1, *c2, buf[256];
	
    if (fragment==NULL)
		return;
	
    td_printf("  %s at 0x%x.",name, (unsigned int)fragment);
    td_printf("      length: %d byte%s\r\n", length, (length>0)?"s":"");
	
    s=fragment;
    j=length;
    while (j>0) {
		memset(buf, ' ',256);
		/* c1+=sprintf(c1=buf, "    data: "); */
		memcpy(buf, "    data: ", 11);
		c1=(buf+10);
		c2=c1+(3*data_per_line)+1;
		for (i=((j>data_per_line)?data_per_line:j); i>0; i--, j--) {
			*c1=(*s>>4); *(c1)+=(*c1<0x0a)?'0':('a'-0x0a); c1++;
			*c1=(*s&0x0f); *(c1)+=(*c1<0x0a)?'0':('a'-0x0a); c1++;
			*c1++=' ';
			if (isprint(*s))
				*c2++=*s;
			else
				*c2++='.';
			s++;
		}
		*c2=0;
		td_printf("%s\r\n",buf);
    }
}

void td_printf(const char* fmt,...){
	const int MAX_DBG_STR = 1024;

	    int written = 0;
	    char buffer[MAX_DBG_STR];
		memset(buffer,0,MAX_DBG_STR);
		__android_log_write(1,"PORT","LOGError-----------------------0");

	    va_list va;
	    va_start( va, fmt );
	    written = vsprintf( &buffer[0], fmt, va);
	    va_end(va);
		FILE *myLog = NULL;
		myLog = fopen("/mnt/sdcard/seraphim/seraphim.log","a+");
		__android_log_write(1,"PORT","LOGError----------------------1");
		if (myLog != NULL)
		{
			char tmp[50] = {0};
			getCurTime((char *)tmp);

			int thepid = (int)getpid();
			char  buf[18];
			memset( buf, 0x0, sizeof( buf ) );
			snprintf(buf,sizeof(buf),"%d",thepid );
			fputs("[PID:",myLog);
			fputs(buf,myLog);
			fputs("]",myLog);
			pthread_t tid = pthread_self();
			memset( buf, 0x0, sizeof( buf ) );
			snprintf(buf,sizeof(buf),"%d", (unsigned int)tid);
			fputs("[TID:",myLog);
			fputs(buf,myLog);
			fputs("]",myLog);
			fputs(tmp,myLog);
			fputs("  ",myLog);
			fputs(buffer,myLog);
		}else{
			__android_log_write(1,"PORT","LOGError--------------2");
		}
		fclose(myLog);


}
void log4(const char* fmt,...){
	const int MAX_DBG_STR = 1024;

	    int written = 0;
	    char buffer[MAX_DBG_STR];
		memset(buffer,0,MAX_DBG_STR);

	    va_list va;
	    va_start( va, fmt );
	    written = vsprintf( &buffer[0], fmt, va);
	    va_end(va);
		FILE *myLog = NULL;
		myLog = fopen("/mnt/sdcard/seraphim/seraphim.log","a+");
		if (myLog != NULL)
		{
//			char tmp[50] = {0};
//			getCurTime((char *)tmp);

//			int thepid = (int)getpid();
//			char  buf[18];
//			memset( buf, 0x0, sizeof( buf ) );
//			snprintf(buf,sizeof(buf),"%d",thepid );
//			fputs("[PID:",myLog);
//			fputs(buf,myLog);
//			fputs("]",myLog);
//			pthread_t tid = pthread_self();
//			memset( buf, 0x0, sizeof( buf ) );
//			snprintf(buf,sizeof(buf),"%d", (unsigned int)tid);
//			fputs("[TID:",myLog);
//			fputs(buf,myLog);
//			fputs("]",myLog);
//			fputs(tmp,myLog);
//			fputs("  ",myLog);
			fputs(buffer,myLog);
		}else{
			__android_log_write(1,"PORT","LOGError");
		}
		fclose(myLog);


}
void UUSee_Printf(const char* fmt, ... )
{
	const int MAX_DBG_STR = 1024;

	    int written = 0;
	    char buffer[MAX_DBG_STR];
		memset(buffer,0,MAX_DBG_STR);

	    va_list va;
	    va_start( va, fmt );
	    written = vsprintf( &buffer[0], fmt, va);
	    va_end(va);
		FILE *myLog = NULL;
		myLog = fopen("/mnt/sdcard/seraphim/seraphim.log","a+");
		if (myLog != NULL)
		{
			char tmp[50] = {0};
			getCurTime((char *)tmp);

			int thepid = (int)getpid();
			char  buf[18];
			memset( buf, 0x0, sizeof( buf ) );
			snprintf(buf,sizeof(buf),"%d",thepid );
			fputs("[PID:",myLog);
			fputs(buf,myLog);
			fputs("]",myLog);
			pthread_t tid = pthread_self();
			memset( buf, 0x0, sizeof( buf ) );
			snprintf(buf,sizeof(buf),"%d", (unsigned int)tid);
			fputs("[TID:",myLog);
			fputs(buf,myLog);
			fputs("]",myLog);
			fputs(tmp,myLog);
			fputs("  ",myLog);
			fputs(buffer,myLog);
		}else{
			__android_log_write(1,"PORT","LOGError");
		}
		fclose(myLog);

}
//�鿴BUFFER״̬
void UUSee_Printf_buffer(const char* fmt, ... )
{	
    const int MAX_DBG_STR = 1024;
	
    int written = 0;
    char buffer[MAX_DBG_STR];
	memset(buffer,0,MAX_DBG_STR);
	
    va_list va;
    va_start( va, fmt );
    written = vsprintf( &buffer[0], fmt, va);
    va_end(va);
	ExportLog2(buffer);

}

void UUSee_Printfile(const char* fmt, ... )
{	
    const int MAX_DBG_STR = 1024;
	
    int written = 0;
    char buffer[MAX_DBG_STR];
	memset(buffer,0,MAX_DBG_STR);
	
    va_list va;
    va_start( va, fmt );
    written = vsprintf( &buffer[0], fmt, va);
    va_end(va);
	
	if(strlen(buffer))
		ExportLog(buffer);
}


void UUSee_AssertFail(const char *cond, const char *file, int line)
{
	printf("Assert Fail: %s, #%d, (%s)\n",file, line, cond);
	UUSee_Printfile("Assert Fail: %s, #%d, (%s)\n",file, line, cond);
	assert(0);
}

#endif
