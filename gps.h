#ifndef _GPS_H
#define _GPS_H

#ifdef  _cplusplus
extern "C"{
#endif

#define GPS_SIZE 1024
// 串口设备号
#define  GPS_DEBUG 1

#define USE_BEIJING_TIMEZONE
#define BAUDRATE B9600

#define COM1 "/dev/ttySAC1"              //
#define GPS_FD  COM1

#define ENDMINITERM 27 /* ESC to quit miniterm */

typedef struct tagDate{
    int m_year;
    int m_month;
    int m_day;
    int m_hour;
    int m_minute;
    int m_second;
}GoToDate;

typedef struct tagLocation
{
    GoToDate m_date;              //时间
    char m_NS;                        //南北极
    char m_EW;                       //东西
    char m_status;  		            //接收状态
    char pack[5] ;                    //填充字符
    char	m_latitude[16];          //纬度
    char m_longitude[16];        //经度
    char m_speed[16];            //速度
    char m_high[16];              //高度
    }GPS_INFO;

int gps_init();                                     //初始化GPS模块
int gps_end();                                  //关闭串口
int gps_read();                                 //读取GPS模块（串口）
void gps_parse(struct tagLocation *GPS);//解析GPS信息
void show_gps(struct tagLocation *GPS);// 显示GPS 信息
int get_gps(GPS_INFO* location ) ;//获取GPS 信息
#ifdef  _cplusplus
}
#endif



#endif
