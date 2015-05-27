#include "log.h"
#include <sys/file.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

Log_Writer Logger;
static __thread char m_buffer[_LOG_BUFFSIZE];
bool logwrite_init()
{
	Log_Writer *lw = &Logger;
	bool bret = false;
	SAFE_CHECK_RET(lw,bret);
	memset(m_buffer,0,_LOG_BUFFSIZE);
	memset(lw,0,sizeof(Log_Writer));
	pthread_mutex_init(&lw->m_mutex,NULL);
	lw->m_system_level = LL_INFO;
	return bret;
}

const char* logLevelToString(LogLevel l) {
        switch ( l ) {
			case LL_DEBUG:
				return "DEBUG";
			case LL_TRACE:
				return "TRACE";
			case LL_INFO:
				return "INFO";
			case LL_WARNING:
				return "WARN" ;
			case LL_ERROR:
				return "ERROR";
			case LL_OFF:
				return "OFF";
			default:
				return "UNKNOWN";
        }
}
	
bool checklevel(LogLevel l)
{
	Log_Writer *lw = (Log_Writer*)&Logger;
	SAFE_CHECK_RET(lw,false);
	if(l >= lw->m_system_level)
		return true;
	else
		return false;
}

bool loginit(LogLevel l, const  char *filelocation)
{
	Log_Writer *lw = (Log_Writer*)&Logger;
	SAFE_CHECK_RET(lw,false);
	if (!lw  || !filelocation)
	{
		return false;
	}
	if(strlen(filelocation) >= (sizeof(lw->m_filelocation) -1))
	{
		fprintf(stderr, "the path of log file is too long:%d limit:%d\n", (int)strlen(filelocation), (int)(sizeof(lw->m_filelocation) -1));
		return false;
	}
	//本地存储filelocation  以防止在栈上的非法调用调用
	strncpy(lw->m_filelocation, filelocation, sizeof(lw->m_filelocation));
	lw->m_filelocation[sizeof(lw->m_filelocation) -1] = '\0';
	
	if('\0' == lw->m_filelocation[0])
	{
		lw->fp = stdout;
		return true;
	}
	
	lw->fp = fopen(lw->m_filelocation, "a");
	if(lw->fp == NULL)
	{
		fprintf(stderr, "cannot open log file,file location is %s\n", lw->m_filelocation);
		return false;
	}
	//setvbuf (fp, io_cached_buf, _IOLBF, sizeof(io_cached_buf)); //buf set _IONBF  _IOLBF  _IOFBF
	lw->m_system_level = l;
	setvbuf (lw->fp,  (char *)NULL, _IOLBF, 0);

	return true;
}

int premakestr(char* m_buffer, LogLevel l)
{
    time_t now;
	now = time(&now);;
	struct tm vtm; 
    localtime_r(&now, &vtm);
    return snprintf(m_buffer, _LOG_BUFFSIZE, "%s: %02d-%02d %02d:%02d:%02d ", logLevelToString(l),
            vtm.tm_mon + 1, vtm.tm_mday, vtm.tm_hour, vtm.tm_min, vtm.tm_sec);
}

bool oa_log( LogLevel l, char* logformat,...)
{
	Log_Writer *lw = (Log_Writer*)&Logger;
	SAFE_CHECK_RET(lw,false);

	if(!checklevel(l))
	{
		return false;
	}
	int _size;
	int prestrlen = 0;
	
	char * star = m_buffer;
	prestrlen = premakestr(star, l);
	star += prestrlen;
	
	va_list args;
	va_start(args, logformat);
	_size = vsnprintf(star, _LOG_BUFFSIZE - prestrlen, logformat, args);
	va_end(args);
	
	if(NULL == lw->fp)
		fprintf(stderr, "%s", m_buffer);
	else
		_write(m_buffer, prestrlen + _size);
	return true;
}

bool _write(char *_pbuffer, int len)
{
	Log_Writer *lw = (Log_Writer*)&Logger;

	if(0 != access(lw->m_filelocation, W_OK))
	{	
		pthread_mutex_lock(&lw->m_mutex);
		//锁内校验 access 看是否在等待锁过程中被其他线程loginit了  避免多线程多次close 和init
		if(0 != access(lw->m_filelocation, W_OK))
		{
			logclose();
			loginit(lw->m_system_level, lw->m_filelocation);
		}
		pthread_mutex_unlock(&lw->m_mutex);
	}

	if(1 == fwrite(_pbuffer, len, 1, lw->fp)) //only write 1 item
	{
		*_pbuffer='\0';
    }
    else 
	{
        int x = errno;
	    fprintf(stderr, "Failed to write to logfile. errno:%s    message:%s", strerror(x), _pbuffer);
	    return false;
	}
	return true;
}


bool logclose()
{
	Log_Writer *lw = &Logger;
	if(lw->fp == NULL)
		return false;
	fflush(lw->fp);
	fclose(lw->fp);
	lw->fp = NULL;
	return true;
}

int loggetlevel( char *level )
{
	int ok = LL_INFO;
	if (!strncasecmp("DEBUG", level, 5))
	{
		ok = LL_DEBUG;
	}else if (!strncasecmp("TRACE", level, 5))
	{
		ok = LL_TRACE;
	}else if (!strncasecmp("INFO", level, 4))
	{
		ok = LL_INFO;
	}
	else if (!strncasecmp("WARN", level, 4))
	{
		ok = LL_WARNING;
	}
	else if (!strncasecmp("ERROR", level, 5))
	{
		ok = LL_ERROR;
	}
	else
	{
		//got LL_OFF;
	}
	return ok;
}



