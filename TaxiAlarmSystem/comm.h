//common.h
//common defination
#ifndef _COMM_H_
#define _COMM_H_

//#include "version.h"


// #ifdef  _X64
// typedef unsigned __int64    size_t;
// #else
// typedef unsigned int   size_t;
// #endif

#define _T

/***************************************************
 * agent settings
***************************************************/

#define APP_MAX_NUM	10
/***************************************************
 * structures, defination for  communication between
 * agent and daemonwang
***************************************************/
#define CMCT_SUCCESS	0
#define CMCT_FAILED		-1
#define CMCT_UNSUPPORT	2


enum 
{
	CMD_APPINFO = 2,
	CMD_METRICS,
	CMD_ERROR,
	CMD_TRACES,
	CMD_SLOW_SQL,
	/* flowing not used*/

	CMD_CONTROL_RX,
	CMD_ERROR_RECEIVE =13
};

typedef	struct _repbuf
{
	int		version;
	int		status;
	int		buflen;
}repbuf, *prepbuf;
typedef struct _cmctbuf
{
	int		version;
	int		run_id; //identifier
	int		cmd;
	int		buflen;
}cmctbuf;


/***************************************************
 * version & copyright
***************************************************/
#define LINUX
#define	ONEAPM_NAME				"OneAPM Server Monitor"
// define in config.h
//#define	PHP_AGENT_VERSION		"1.0.0.0" 
#define PHP_VERSION_NUM			1000

/***************************************************
 * Deafult Config string
***************************************************/
#define CONFIG_CONLLECTOR_URL	"tpm.oneapm.com"
#define CONFIG_LOG_LEVEL		"info"
#define CONFIG_UNIX_DOMAIN_PATH	"/tmp/.oneapm-daemon.domain"
#define CONFIG_OA_PID_FILE		"/var/log/oneapm/oneapm-daemon.pid"
#define CONFIG_OA_LOG_FILE		"/var/log/oneapm/oneapm-daemon.log"
#define CONFIG_OA_CFG_FILE		"/etc/oneapm/oneapm.cfg"
#define OCNFIG_SSL_ENABLE		TRUE
#define CONFIG_SPECIAL			0
#define CONFIG_PORT				0

#define PROTOCOL_VERSION		"12"
#define RPM_POST_URL			"http%s://%s/tpm/agent.do?method=?marshal_format=json&license_key=%s&protocol_version=%s&method=%s"

/***************************************************
 * rpm
***************************************************/
#define COLLECT_HOST_NAME		"tpm.oneapm.com"


/***************************************************
 * funciton
***************************************************/

#define TRACE	printf
#define SAFE_FREE(p)	if(p){free(p); p = NULL;}
#if !defined sizearray
#define sizearray(a)    (sizeof(a) / sizeof((a)[0]))
#endif

/***************************************************
 * command:
***************************************************/

#define CMD_CONNECT				"connect"
#define CMD_SHUTDOWN			"shutdown"
#define CMD_METRIC_DATA			"metric_data"
#define CMD_SQL_TRACE_DATA		"sql_trace_data"
#define CMD_TRACES_DATA	        "transaction_sample_data"
#define CMD_EVENT_DATA			"analytic_event_data"
#define CMD_ERROR_DATA			"error_data"

/***************************************************
 * constant value
***************************************************/
#define CONN_BUF_DEF_SIZE 4096
#define CONN_BUF_EXT_SIZE CONN_BUF_DEF_SIZE
#endif
