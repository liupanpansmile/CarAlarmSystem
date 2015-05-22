#ifndef   _MACRO_LogModule
#define   _MACRO_LogModule
#include <stdio.h>
#include <pthread.h>
 #include<unistd.h>  
#include "macro_define.h"

/* 每个线程的buffer size*/
#define   _LOG_BUFFSIZE  1024*1024*4
/* 当前进程的 Stream IO buffer size*/
#define   _SYS_BUFFSIZE  1024*1024*8
/* log 文件字符串路径最大长度*/
#define	  _LOG_PATH_LEN  250
/* 日志对应的模块名*/
#define   _LOG_MODULE_LEN 32

typedef  enum LogLevel {  
	LL_DEBUG = 1,
	LL_TRACE = 2,
	LL_INFO = 3, 
	LL_WARNING = 4, 
	LL_ERROR = 5,
	LL_OFF	= 6
}LogLevel;

/**
*	Log_Writer  日志类
*/
typedef struct _Log_Writer
{
		enum LogLevel m_system_level;
		FILE* fp;
		char m_filelocation[_LOG_PATH_LEN];
		pthread_mutex_t m_mutex;
		//The __thread specifier may be applied to any global, file-scoped static, function-scoped static, 
		//or static data member of a class. It may not be applied to block-scoped automatic or non-static data member
		//in the log  scence,It's safe!!!!
		//一言以蔽之，此场景不用担心__thread带来资源leak,同时也不用担心多个Log_Writer会干扰，
		//因为一个线程同一时间只有一个Log_Writer在干活，干完之后m_buffer就reset了
		//所以即便一个线程用户多个Log_Write串行(因为一个线程内的运行态只有串行) 也是线程安全的！！！
}Log_Writer;

/**
 * LogLevel 日志级别
 * p_modulename 模块名 如mysql
 * p_logdir  日志输出目录
 * */
extern Log_Writer Logger;
bool _write(char *_pbuffer, int len);
bool oa_log( LogLevel l, char* logformat,...);
int  loggetlevel(char *level);
bool loginit(LogLevel l, const  char *filelocation);
bool logwrite_init();
bool logclose();
#endif
