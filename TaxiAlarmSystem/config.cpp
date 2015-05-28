#include "comm.h"

#ifdef __cplusplus
extern "C" {
#include "log.h"
}
#endif

#include "minIni.h"
#include "config.h"
#include <string.h>
#include <stdlib.h>

const char *Config::CONFIGFILE = "./oneMe.cfg";
const char *Config::HOST = "tpm.oneapm.com";
const char *Config::LOGLEVEL = "info";
const char *Config::LOGFILE = "/var/log/oneapm/oneMe.log";
const int  Config::PORT = 80;

Config::Config()
{
	host[0] = '\0';
	loglevel[0] = '\0';
	port = 0;
	logfile[0] = '\0';
	dirverName[0] = '\0' ;
	driverTelNumber[0] = '\0' ;
	plateNumber[0] = '\0' ;
	pictureName[0] = '\0' ;

}

Config::~Config()
{
}

void Config::load()
{
	FILE *cfgfile = fopen(CONFIGFILE, "r");
	if (!cfgfile)
	{
		log_debug("Failed to open config file %d|%s", errno, strerror(errno));
		log_warn("Failed to open config file, default setting will be used\n", CONFIGFILE);
		load_default(0, 0, 0);
		return;
	}
	fclose(cfgfile);

	int logfileLen = ini_getstr("logfile", logfile, sizearray(logfile), CONFIGFILE);
	int loglevelLen = ini_getstr("loglevel", loglevel, sizearray(loglevel), CONFIGFILE);
	int hostLen = ini_getstr("host", host, sizearray(host), CONFIGFILE);
	
	int dirverNameLen = ini_getstr("driverName", dirverName, sizearray(dirverName), CONFIGFILE);
	int dirverNumLen = ini_getstr("driverTelNumber", driverTelNumber, sizearray(driverTelNumber), CONFIGFILE);
	int plateNumberLen = ini_getstr("plateNumber", plateNumber, sizearray(plateNumber), CONFIGFILE);

	int pictureNameLen = ini_getstr("pictureName", pictureName, sizearray(pictureName), CONFIGFILE);
	this->port = ini_getlong("port", PORT, CONFIGFILE);
	
	load_default(logfileLen, loglevelLen, hostLen);

}

void  Config::load_default(int collectorlen , int logfilelen, int loglevellen)
{
	int result = true; //
	if (collectorlen <= 0)
	{
		strcpy(host, HOST);
	}

	if(logfilelen <= 0)
	{
		strcpy(logfile, LOGFILE);
	}

	LogLevel llevel = LL_INFO;
	if(loglevel <= 0)
	{
		strcpy(loglevel, LOGLEVEL);
		llevel = LL_INFO;
	}else
	{
		llevel = (LogLevel)loggetlevel(loglevel);
	}
	if (this->port == 0)
	{
		this->port = PORT;
	}
	if (false ==  loginit(llevel, logfile))
	 	return ;
	
}

