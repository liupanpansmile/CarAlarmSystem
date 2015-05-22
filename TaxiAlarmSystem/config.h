#pragma once
#include "comm.h"

class Config
{
public:
	Config();
	~Config();

public:
	void load();

	inline char* Host(){return this->host;}
	inline char* Loglevel() {return this->loglevel;}
	inline int	  Port() {return this->port;}
	inline char* Logfile() {return this->logfile; }
	inline char* DriverName() {return this->dirverName ;}
	inline char* DriverTelNumber() {return this->driverTelNumber ;}
	inline char* PictureName(){return this->pictureName ;}
	inline char* PlateNumber() {return this->plateNumber ;}

private:
	void load_default(int collector , int logfile, int loglevel);

private:
	char host[512];
	char loglevel[64];
	int  port;
	char logfile[512];

	char dirverName[128];
	char driverTelNumber[32] ;
	char plateNumber[128] ;
	char pictureName[128] ;

	static const char *CONFIGFILE;
	static const char *HOST;
	static const char *LOGLEVEL;
	static const char *LOGFILE;
	static const int  PORT;
	

};

