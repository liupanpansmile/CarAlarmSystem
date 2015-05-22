#ifndef _PUB_H
#define _PUB_H
#ifdef  _cplusplus
extern "C"{
#endif

#include "gps.h"

#define PICTURE_SIZE 320*240*3+54
typedef struct CarInfo
{
    char carOwnerName[20] ;   //��������
    char carOwnerTel[12] ;       //������ϵ��ʽ
    char carNumber[20] ;        //���ƺ���
}CarInfo;

typedef struct AllInfo
{
    char filename[16];        //����ͼƬ�ļ�������
    int  is_call_police ;       //�Ƿ񱨾�
    CarInfo car_info ;         //������Ϣ
    GPS_INFO gps_info ;         //gps ��Ϣ
}AllInfo;

#ifdef  _cplusplus
}
#endif

#endif

