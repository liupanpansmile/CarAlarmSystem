#ifndef _PUB_H
#define _PUB_H
#ifdef  _cplusplus
extern "C"{
#endif

#include "gps.h"

#define PICTURE_SIZE 320*240*3+54
typedef struct CarInfo
{
    char carOwnerName[20] ;   //车主姓名
    char carOwnerTel[12] ;       //车主联系方式
    char carNumber[20] ;        //车牌号码
}CarInfo;

typedef struct AllInfo
{
    char filename[16];        //发送图片文件的名称
    int  is_call_police ;       //是否报警
    CarInfo car_info ;         //车的信息
    GPS_INFO gps_info ;         //gps 信息
}AllInfo;

#ifdef  _cplusplus
}
#endif

#endif

