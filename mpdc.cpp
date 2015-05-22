#define _cplusplus
#include "mpdc.h"
#include "ui_mpdc.h"
#include "rgb2bmp.h"
#include "gps.h"
#include "v4l.h"
#include "send_data.h"
#include <QTime>
#include <QMessageBox>
#include <QFont>

#define 	SAFE  1
#define 	DANGER 0
#define     CALL_POLICE_RET     3

extern char server_ip_address[20] ;
/*********************************************************************************************************
** Function name: get_carInfo
** Descriptions: 打开车信息的文件，并写进相应的结构体中
** Input: carInfoFile			要打开的文件名称
**        carInfo				保存车信息的结构体指针
** Output : 返回1表示正常完成返回。
** Created by: lpp
** Created Date: 2014/5/18
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified Date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
int static get_carInfo( char* carInfoFile,CarInfo *carInfo )
    {

    int car_fd ;
    int ret = 0 ;
/*
    car_fd = open(carInfoFile,O_WRONLY) ;
    if(car_fd < 0)
        {
        perror(carInfoFile) ;
        exit(0) ;
        }
    CarInfo testInfo = {"张三","18304635731","黑A11100"} ;
    write(car_fd,&testInfo,sizeof(CarInfo)) ;
    close(car_fd) ;
*/

    car_fd = open(carInfoFile,O_RDONLY) ;
    if(car_fd < 0)
        {
        perror(carInfoFile) ;
        exit(0) ;
        }
    //读取文件中的信息，保存到结构提=体中
    ret = read(car_fd,carInfo,sizeof(CarInfo)) ;
    if(ret  < sizeof(CarInfo))
        {
        printf("read %s failed \n",carInfoFile) ;
        exit(0) ;
        }
   // printf("name = %s,tel = %s,no = %s\n",carInfo->carOwnerName,carInfo->carOwnerTel,carInfo->carNumber) ;
    close(car_fd) ;
    return 1 ;
    }



MPdc::MPdc(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MPdc)
{
    ui->setupUi(this);

    allInfo = new AllInfo() ;          //保存所有的发送信息
   strcpy(carInfoFile,"carInfo.txt") ;      //保存车信息的文件名
    strcpy(pictureName,"pic.bmp") ;     //保存图片的文件名

    /*设置设备信息*/
   v4l_open(DEFAULT_DEVICE, &v4l_dev0) ;/*打开设备*/
    v4l_get_capability(&v4l_dev0);
    v4l_get_picture(&v4l_dev0);
    v4l_init_mbuf(&v4l_dev0);/*初始化设备*/
    v4l_get_mbuf(&v4l_dev0);/*内存映射*/
     v4l_get_window(&v4l_dev0);

     /*设置图片的色度*/
     /********************
      * fbuffer : base=0x(nil) size=0x0 depth=0 bpl=0
  picture : brightness=32768 hue=0 colour=0 contrast=32768
  picture : whiteness=0 depth=24 palette=rgb24
  mbuf: size=2457616 frames=2
  v4l: using mapped buffers for capture
      *************/
     v4l_dev0.picture.depth = 24;
     v4l_dev0.picture.brightness=32768;
     v4l_dev0.picture.contrast=32768;
     v4l_dev0.picture.hue=0;
     v4l_dev0.picture.colour=0;
     v4l_dev0.picture.whiteness = 0;
     v4l_dev0.picture.palette=QImage::Format_RGB888;


     timer = new QTimer(this);
  //   datetime_timer = new QTimer(this) ;


     connect(timer, SIGNAL(timeout()), this, SLOT(GetVideo()));
  //   connect(datetime_timer,SIGNAL(timeout()),this,SLOT(showDateTime())) ;
     timer->start(1);
 //    datetime_timer->start(1000) ;
}

MPdc::~MPdc()
{
    delete timer ;
    delete allInfo ;
    delete ui;
}
void MPdc::closeEvent(QCloseEvent *event)
{
    if(timer->timerId())
    {
        killTimer(timer->timerId());
    }
    v4l_munmap(&v4l_dev0) ;
    v4l_close(&v4l_dev0);
    event->accept();
 }

void MPdc::showDateTime()
{
    QDateTime currentTime = QDateTime::currentDateTime() ;      //获取当前时间
    QString timeStr = currentTime.toString("yyyy-MM-dd hh:mm:ss dddd") ;    //设置系统时间格式
   // printf("time= %s\n",timeStr) ;
    ui->LblDateTime->setText(timeStr) ;

}

void MPdc::GetVideo()
{
    char* filename  = "a.bmp" ;
       v4l_grab_movie(&v4l_dev0);

        qApp->processEvents();
       //  write_data(filename,v4l_dev0.buffer,MAX_WIDTH,MAX_HEIGHT) ;
       QImage image0((const uchar*)v4l_dev0.buffer,MAX_WIDTH,MAX_HEIGHT,QImage::Format_RGB888);

       //  QImage image0;
      //   image0.loadFromData((const uchar*)v4l_dev0.buffer,MAX_WIDTH*MAX_HEIGHT*4) ;
     //  image0.load(filename) ;
        ui->LblShowPic->setPixmap(QPixmap::fromImage(image0));

}



void MPdc::on_BtnTakePicture_clicked()
{
        allInfo->is_call_police = 0 ;                       //没有报警
       deal_request();
}

void MPdc::on_BtnCallPolice_clicked()
{
        allInfo->is_call_police = 1 ;                       //报警
        deal_request();
}


 void MPdc::deal_request()
{
    v4l_grab_movie(&v4l_dev0);
   write_data(pictureName,v4l_dev0.buffer,MAX_WIDTH,MAX_HEIGHT) ;

     //获取车相关信息
    get_carInfo( carInfoFile,&allInfo->car_info )  ; //获取车的信息
    strcpy(allInfo->filename,pictureName);			//保存照片的名称


 //   printf("get GPS Info ...\n") ;
    //获取GPS 信息

      get_gps( &(allInfo->gps_info) ) ;             //获取GPS信息
   /* while(1)
               {
                   get_gps( &(allInfo->gps_info) ) ;             //获取GPS信息
                   if(allInfo->gps_info.status == 'A')
                       break ;
               }
   */
     show_gps(&(allInfo->gps_info)) ;				//显示GPS 信息

     //发送信息至服务器端
     int is_safe_now =  send_to_server(allInfo,server_ip_address);		//发送数据至服务器端

     if(is_safe_now == DANGER )
        {
              printf("you are in danger now !\n ") ;

              this->ui->LblSafeOrNot->setText("<font color=red>危险</font>");
              this->ui->LblSafeOrNot->setFont(QFont("simsun",36));
        }
         else if(is_safe_now == SAFE )
          {
              printf("you are safe now !\n ") ;
              this->ui->LblSafeOrNot->setText("安全");
              this->ui->LblSafeOrNot->setFont(QFont("simsun",36));
           }
     else if(is_safe_now == CALL_POLICE_RET )
     {
             printf("The rescue is coming  !\n ") ;
             this->ui->LblSafeOrNot->setText("正在救援");
            this->ui->LblSafeOrNot->setFont(QFont("simsun",36));

       }
           printf("send infomation end...\n") ;
}

