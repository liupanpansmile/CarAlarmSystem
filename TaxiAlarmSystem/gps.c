#include <string.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>

#include "gps.h"
#include "log.h"

#define FALSE 0
#define TRUE 1

volatile int gps_fd; // 打开串口终端的文件标识符
char GPS_BUF[1024]; // 存放GPS信息的缓冲区
int baud = BAUDRATE; // 波特率

struct termios oldtio,newtio,oldstdtio,newstdtio;

// 得到第几个逗号在缓存区的位置 -- 内部接口
static int GetComma(int num,char* str);
// 将时间转换成北京时间 -- 内部接口
static void UTC2BTC(struct tagDate *GPS);
static void get_char_array(char *s,char *buf);

/*********************************************************************************************************
** Function name: gps_init
** Descriptions: 打开GPS对应的串口，初始化串口相应的信息
** Input: 			无
** Output : 返回TRUE表示正常完成返回。FALSE打开串口失败
** Created by: lpp
** Created Date: 2014/5/18
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified Date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
int gps_init()
{
    //struct termios oldtio,newtio,oldstdtio,newstdtio;
    gps_fd = open(GPS_FD, O_RDWR );
    if (gps_fd <0)
    {
        perror(GPS_FD);
		log_debug("open gps moderm failed") ;
        return FALSE;
    }
	log_debug("open gps moderm success ! initializing serial modem...") ;

    tcgetattr(0,&oldstdtio);
    tcgetattr(gps_fd,&oldtio); /* save current modem settings */
    tcgetattr(gps_fd,&newstdtio); /* get working stdtio */

    newtio.c_cflag = baud | CRTSCTS | CS8 | CLOCAL | CREAD;/*ctrol flag*/
    newtio.c_iflag = IGNPAR; /*input flag*/
    newtio.c_oflag = 0;         /*output flag*/
    newtio.c_lflag = 0;         /* local modes */ 
    newtio.c_cc[VMIN]=1;    /* control chars */
    newtio.c_cc[VTIME]=0;

 /* now clean the modem line and activate the settings for modem */
    tcflush(gps_fd, TCIFLUSH);
    tcsetattr(gps_fd,TCSANOW,&newtio);/*set attrib	  */
    return TRUE;
}

/*********************************************************************************************************
** Function name: gps_end
** Descriptions: 保留串口的一些设置，关闭串口
** Input: 			无
** Output : 返回TRUE表示正常完成返回。
** Created by: lpp
** Created Date: 2014/5/18
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified Date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
int gps_end()
{

    tcsetattr(gps_fd,TCSANOW,&oldtio); /* restore old modem setings */
    tcsetattr(0,TCSANOW,&oldstdtio); /* restore old tty setings */
    close(gps_fd);
	log_debug("gps end , and close gps fd ...") ;
    return TRUE;
}

/*********************************************************************************************************
** Function name: gps_read
** Descriptions: 读取GPS设备的信息
** Input: 			无
** Output :
** Created by: lpp
** Created Date: 2014/5/18
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified Date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
int gps_read()
{
    int i = 0;
    char c;
    char buf[GPS_SIZE];
	log_debug("read gps moderm ...") ;
    while ( 1 )
    {

        read(gps_fd, &c, 1); /* com port */
        buf[i++] = c;

        if(c == '\n')
        {
            strncpy(GPS_BUF,buf,i);
            break;
        }
        if(i > 1024)
        {
            log_debug("GPS_ERROR");
            break;
        }
    }
    return 0;
}

/****************************************************************
** Function name: show_gps
** Descriptions: 显示解析后的GPS信息
** Input: *GPS  GPS信息结构体指针
**
** Output : 无
** Created by: lpp
** Created Date: 2014-5-16
*****************************************************************/
void show_gps(struct tagLocation *GPS)
{
	log_debug("---------------GPS Info as follow------------") ;
	log_debug("DATE     : %d-%02d-%02d \n",GPS->m_date.m_year,GPS->m_date.m_month,GPS->m_date.m_day);
	log_debug("TIME     :  %02d:%02d:%02d \n",GPS->m_date.m_hour,GPS->m_date.m_minute,GPS->m_date.m_second);
	log_debug("Latitude : %s %c\n",GPS->m_latitude,GPS->m_NS);
	log_debug("Longitude: %s %c\n",GPS->m_longitude,GPS->m_EW);
	log_debug("high     : %s \n",GPS->m_high);
	log_debug("STATUS   : %c\n",GPS->m_status);
}

/*********************************************************************************************************
** Function name: gps_parse
** Descriptions: 将gps信息字符串解析成结构体信息
** Input:  *GPS	GPS结构体
** Output : 无
** Created by: lpp
** Created Date: 2014-5-16
********************************************************************************************************/
/*
例：$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50
字段0：$GPRMC，语句ID，表明该语句为Recommended Minimum Specific GPS/TRANSIT Data（RMC）推荐最小定位信息
字段1：UTC时间，hhmmss.sss格式
 字段2：状态，A=定位，V=未定位
字段3：纬度ddmm.mmmm，度分格式（前导位数不足则补0）
 字段4：纬度N（北纬）或S（南纬）
字段5：经度dddmm.mmmm，度分格式（前导位数不足则补0）
 字段6：经度E（东经）或W（西经）
 字段7：速度，节，Knots
字段8：方位角，度
字段9：UTC日期，DDMMYY格式
*/
void gps_parse(struct tagLocation *GPS)
{
    int tmp;
    char c;
    char* buf=GPS_BUF;
    c=buf[5];
	log_debug("gps_parse begin ...") ;
    if(c=='C'){//"GPRMC"
        GPS->m_date.m_hour   =(buf[ 7]-'0')*10+(buf[ 8]-'0');
        GPS->m_date.m_minute =(buf[ 9]-'0')*10+(buf[10]-'0');
        GPS->m_date.m_second =(buf[11]-'0')*10+(buf[12]-'0');
        tmp = GetComma(9,buf);
        GPS->m_date.m_day    =(buf[tmp+0]-'0')*10+(buf[tmp+1]-'0');
        GPS->m_date.m_month  =(buf[tmp+2]-'0')*10+(buf[tmp+3]-'0');
        GPS->m_date.m_year   =(buf[tmp+4]-'0')*10+(buf[tmp+5]-'0')+2000;
        //------------------------------
        GPS->m_status	  =buf[GetComma(2,buf)];
        get_char_array(&buf[GetComma(3,buf)],GPS->m_latitude) ;
        GPS->m_NS       =buf[GetComma(4,buf)];
        get_char_array(&buf[GetComma(5,buf)],GPS->m_longitude) ;
        GPS->m_EW       =buf[GetComma(6,buf)];
#ifdef USE_BEIJING_TIMEZONE
        UTC2BTC(&GPS->m_date);
#endif
    }
    if(c=='A'){ //"$GPGGA"
        get_char_array( &buf[GetComma(9,buf)],GPS->m_high);
    }
	log_debug("read gps modem end ...") ;
}

/******************************************************************
** Function name:  get_char_array
** Descriptions: 截取一个字符串从第一个字符到首次遇到逗号其间的字符串，保存在buf中
** Input: *s	源数据
**		  *buf 保存截取数据的指针
** Output : 无
** Created by: lpp
** Created Date: 2014-5-16
*****************************************************************/
static void get_char_array(char *s,char *buf)
{
    int i;

    i=GetComma(1,s);   //获取首次出现逗号的下一个字符的下标，即要获取字符的长度
    strncpy(buf,s,i);
    buf[i]=0;
}


/************************************************************************
** Function name:  GetComma
** Descriptions: 得到逗号序列对应的下一个字符的下标
** Input: num	逗号的序列号
**		  *str 源数据
** Output : 指定逗号的下一个字符的下标
** Created by: lpp
** Created Date: 2014-5-16
**************************************************************************/
static int GetComma(int num,char *str)
{
    int i,j=0;
    int len=strlen(str);
    for(i=0;i<len;i++)
    {
        if(str[i]==',')j++;
        if(j==num)return i+1;
    }
    return 0;
}

/************************************************************
** Function name:  UTC2BTC
** Descriptions: 将世界时转换为北京时间
** Input: 	*GPS
** Output : 无
** Created by: lpp
** Created Date: 2014-5-16
************************************************************/
static void UTC2BTC(struct tagDate *GPS)
{
//***************************************************
//如果秒号先出,再出时间数据,则将时间数据+1秒
        GPS->m_second++; //加一秒
        if(GPS->m_second>59){
            GPS->m_second=0;
            GPS->m_minute++;
            if(GPS->m_minute>59){
                GPS->m_minute=0;
                GPS->m_hour++;
            }
        }

//***************************************************
        GPS->m_hour+=8;
        if(GPS->m_hour>23)
        {
            GPS->m_hour-=24;
            GPS->m_day+=1;
            if(GPS->m_month==2 ||
                    GPS->m_month==4 ||
                    GPS->m_month==6 ||
                    GPS->m_month==9 ||
                    GPS->m_month==11 ){
                if(GPS->m_day>30){
                    GPS->m_day=1;
                    GPS->m_month++;
                }
            }
            else{
                if(GPS->m_day>31){
                    GPS->m_day=1;
                    GPS->m_month++;
                }
            }
            if(GPS->m_year % 4 == 0 ){//
                if(GPS->m_day > 29 && GPS->m_month ==2){
                    GPS->m_day=1;
                    GPS->m_month++;
                }
            }
            else{
                if(GPS->m_day>28 &&GPS->m_month ==2){
                    GPS->m_day=1;
                    GPS->m_month++;
                }
            }
            if(GPS->m_month>12){
                GPS->m_month-=12;
                GPS->m_year++;
            }
        }
}



/************************************************************
** Function name:  GetLocation
** Descriptions: 读取原始的GPS 信息。并转换
** Input: 	*location
** Output : 无
** Created by: lpp
** Created Date: 2014-5-16
************************************************************/
int GetLocation(struct tagLocation *location)
{
    gps_read();
    gps_parse(location);
    return 0 ;
}

/************************************************************
** Function name:  get_gps
** Descriptions:直到读取到争取的GPS信息为止
** Input: 	*location
** Output : 无
** Created by: lpp
** Created Date: 2014-5-16
************************************************************/
int get_gps(GPS_INFO* location )
{
    gps_init( );
    while( 1 )
    {
        GetLocation(location);
        show_gps(location) ;
        if(location->m_status == 'A')
        {
            break ;
        }
    }
    gps_end( ) ;
    return 0 ;
}
