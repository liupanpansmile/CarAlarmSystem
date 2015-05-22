#ifndef   _MACRO_LogModule
#define   _MACRO_LogModule
#include <stdio.h>
#include <pthread.h>
 #include<unistd.h>  
#include "macro_define.h"

/* ÿ���̵߳�buffer size*/
#define   _LOG_BUFFSIZE  1024*1024*4
/* ��ǰ���̵� Stream IO buffer size*/
#define   _SYS_BUFFSIZE  1024*1024*8
/* log �ļ��ַ���·����󳤶�*/
#define	  _LOG_PATH_LEN  250
/* ��־��Ӧ��ģ����*/
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
*	Log_Writer  ��־��
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
		//һ���Ա�֮���˳������õ���__thread������Դleak,ͬʱҲ���õ��Ķ��Log_Writer����ţ�
		//��Ϊһ���߳�ͬһʱ��ֻ��һ��Log_Writer�ڸɻ����֮��m_buffer��reset��
		//���Լ���һ���߳��û����Log_Write����(��Ϊһ���߳��ڵ�����ֻ̬�д���) Ҳ���̰߳�ȫ�ģ�����
}Log_Writer;

/**
 * LogLevel ��־����
 * p_modulename ģ���� ��mysql
 * p_logdir  ��־���Ŀ¼
 * */
extern Log_Writer Logger;
bool _write(char *_pbuffer, int len);
bool oa_log( LogLevel l, char* logformat,...);
int  loggetlevel(char *level);
bool loginit(LogLevel l, const  char *filelocation);
bool logwrite_init();
bool logclose();
#endif
