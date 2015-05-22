#ifndef _SEND_DATA_H
#define _SEND_DATA_H

#ifdef  _cplusplus
extern "C"{
#endif

#include "pub.h"
#include "gps.h"

//发送数据至服务器端
int send_to_server(AllInfo* send_data,char *server_ip_address,int port) ;

#ifdef  _cplusplus
}
#endif

#endif

