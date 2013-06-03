#ifndef __1CAED58E_6CFE_4de9_8F5A_DFFC9D225CC8__
#define __1CAED58E_6CFE_4de9_8F5A_DFFC9D225CC8__


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 模块给UI层的回调消息类型
// 
typedef enum enumLiveUploadWrapperState
{
    LIVE_UPLOAD_WRAPPER_MSG_TYPE_NONE,

    // 
    // int[0]   : type
    // 
    LIVE_UPLOAD_WRAPPER_MSG_TYPE_START_SUCCESS,
    LIVE_UPLOAD_WRAPPER_MSG_TYPE_START_FAILED,
    LIVE_UPLOAD_WRAPPER_MSG_TYPE_STARTING,

    LIVE_UPLOAD_WRAPPER_MSG_TYPE_DISCONNECTED,
    LIVE_UPLOAD_WRAPPER_MSG_TYPE_STOP_FAILED,
    LIVE_UPLOAD_WRAPPER_MSG_TYPE_STOPPING,
    LIVE_UPLOAD_WRAPPER_MSG_TYPE_STOPPED,

    // 
    // int[0]   : type
    // int[1]   : send len this time( not total send len )
    // 
    LIVE_UPLOAD_WRAPPER_MSG_TYPE_UPLOADING,

    // 
    // int[0]   : type
    // int[1]   : send len this time( not total send len )
    // 
    // str[0]   : file name
    // 
    LIVE_UPLOAD_WRAPPER_MSG_TYPE_UNP_END
} enumLiveUploadWrapperState;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 回调函数定义
// 

// 
// lpszUnpFileName不是unp全路径名， 只是文件名
// 
typedef int ( * live_upload_callback )( void * hTask, 
                                       enumLiveUploadWrapperState eCallbackState, 
                                       int nSendLen, 
                                       const char * lpszUnpFileName );

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 模块环境初始化，如此初始化
// 

// 
// 说明：初始化模块环境。这是个阻塞函数，不过会马上返回，没有回调
// 
// lpszModulePath   : (跨平台)动态库目录绝对位置,ios下设置为""
// lpszLogPath      : 保存日志的绝对路径
// 
// 成功：返回0
// 失败：返回非0，表示错误码
// 
int live_upload_wrapper_init( live_upload_callback pCallback, const char * lpszModulePath, const char * lpszLogPath );


// 
// 释放模块。需要停止所有上传任务后调用。
// 
// 说明：这是个阻塞函数，不过会马上返回，没有回调
// 
// 成功：返回0
// 失败：返回非0，表示错误码
// 
int live_upload_wrapper_term();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 上传任务方法
// 

// 
// 创建一个上传任务对象
// 
// 说明：这是个阻塞函数，不过会马上返回，没有回调
// 
// 成功：返回模块分配的，但还没有成功初始化的句柄。需要调用open来打开上传通道
// 失败：返回0
// 
void * live_upload_wrapper_create( const char * lpszChannelId, const char * lpszServerIp, unsigned short usServerPort );


// 
// 销毁一个上传任务对象
// 
// 说明：这是个阻塞函数，不过会马上返回，没有回调
// 
// 成功：返回0
// 失败：返回非0
// 
int live_upload_wrapper_destroy( void * hTask );


// 
// 打开上传通道
// 
// 说明：需要等待回调返回成功，才可以使用它上传
// 
// 成功：返回0
// 失败：返回非0，表示参数不正确，如：空
// 
int live_upload_wrapper_open( void * hTask );


// 
// 关闭上传通道
// 
// 说明：这个函数是异步操作，需要等待回调返回成功
// 
// 执行close后,不会销毁句柄。需调用destroy方法来销毁句柄。
// 
// 成功： 0. 表示提交请求成功
// 失败： 非0
// 
int live_upload_wrapper_close( void * hTask );


// 
// 上传unp文件
// 
// 这个函数是异步操作，会有回调状态
// 
// 成功： 0. 表示提交请求成功
// 失败： 非0，表示提交请求失败
// 
int live_upload_wrapper_upload( void * hTask, const char * lpszFullFileName, const char * lpszFileName );


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 

#endif // __1CAED58E_6CFE_4de9_8F5A_DFFC9D225CC8__
