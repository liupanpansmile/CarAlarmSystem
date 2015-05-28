#define _cplusplus

#include <QTime>
#include <QMessageBox>
#include <QFont>

#include "CASys.h"
#include "ui_CASys.h"
#include "rgb2bmp.h"
#include "gps.h"
#include "v4l.h"
#include "send_data.h"


#define 	SAFE  1
#define 	DANGER 0
#define     CALL_POLICE_RET     3

static void DEBUG_PRINT_ALLINFO(AllInfo *allInfo)
{
	log_debug("pictureName = %s , DriverName =%s ,Telephone = %s ,plateNumber =%s \n",allInfo->filename,allInfo->car_info.driverName,allInfo->car_info.driverTelNumber,allInfo->car_info.plateNumber) ;
}


void CASys::Init()
{
	if(config != NULL)
	{
		config->load() ;
		strcpy(allInfo->filename,config->PictureName()) ;						//图片名称
		strcpy(allInfo->car_info.driverName,config->DriverName()) ;				//司机姓名
		strcpy(allInfo->car_info.driverTelNumber,config->DriverTelNumber()) ;	//电话号
		strcpy(allInfo->car_info.plateNumber,config->PlateNumber()) ;			//车牌号
		strcpy(serverIP,config->Host()) ;										
		port = config->Port() ;
		DEBUG_PRINT_ALLINFO(allInfo) ;
	}
}

CASys::CASys(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::CASys)
{
	ui->setupUi(this);
	allInfo = new AllInfo() ;          
	config = new Config() ;

	Init() ;						/*初始化车相关信息*/

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
	connect(timer, SIGNAL(timeout()), this, SLOT(GetVideo()));
	timer->start(1);

	//   datetime_timer = new QTimer(this) ;
	//   connect(datetime_timer,SIGNAL(timeout()),this,SLOT(showDateTime())) ;
	//    datetime_timer->start(1000) ;

}

CASys::~CASys()
{
	if(timer != NULL)
	{
		delete timer ;
		timer = NULL ;
	}
	if(allInfo != NULL)
	{
		delete allInfo ;
		allInfo = NULL ;
	}
	if(ui != NULL)
	{
		delete ui ;
		ui = NULL ;
	}
	if(config != NULL)
	{
		delete config ;
		config = NULL ;
	}

}

void CASys::closeEvent(QCloseEvent *event)
{
	if(timer->timerId())
	{
		killTimer(timer->timerId());
	}
	v4l_munmap(&v4l_dev0) ;
	v4l_close(&v4l_dev0);
	event->accept();
	//log_debug("close event") ;
}

void CASys::showDateTime()
{
	QDateTime currentTime = QDateTime::currentDateTime() ;      //获取当前时间
	QString timeStr = currentTime.toString("yyyy-MM-dd hh:mm:ss dddd") ;    //设置系统时间格式
	ui->LblDateTime->setText(timeStr) ;
}

void CASys::GetVideo()
{
	v4l_grab_movie(&v4l_dev0);

	qApp->processEvents();
	//  write_data(filename,v4l_dev0.buffer,MAX_WIDTH,MAX_HEIGHT) ;
	QImage image0((const uchar*)v4l_dev0.buffer,MAX_WIDTH,MAX_HEIGHT,QImage::Format_RGB888);

	//  QImage image0;
	//   image0.loadFromData((const uchar*)v4l_dev0.buffer,MAX_WIDTH*MAX_HEIGHT*4) ;
	//  image0.load(filename) ;
	ui->LblShowPic->setPixmap(QPixmap::fromImage(image0));
}



void CASys::on_BtnTakePicture_clicked()
{
	allInfo->is_call_police = 0 ;                       //没有报警
	deal_request();
}

void CASys::on_BtnCallPolice_clicked()
{
	allInfo->is_call_police = 1 ;                       //报警
	deal_request();
}


void CASys::deal_request()
{
	v4l_grab_movie(&v4l_dev0);
	write_data(allInfo->filename,v4l_dev0.buffer,MAX_WIDTH,MAX_HEIGHT) ;

	get_gps( &(allInfo->gps_info) ) ;             //获取GPS信息

	//发送信息至服务器端
	int is_safe_now =  send_to_server(allInfo,serverIP,port);		//发送数据至服务器端

	if(is_safe_now == DANGER )
	{
		//log_info("you are in danger now ! ") ;

		this->ui->LblSafeOrNot->setText("<font color=red>危险</font>");
		this->ui->LblSafeOrNot->setFont(QFont("simsun",36));
	}
	else if(is_safe_now == SAFE )
	{
		//log_info("you are safe now !\n ") ;
		this->ui->LblSafeOrNot->setText("安全");
		this->ui->LblSafeOrNot->setFont(QFont("simsun",36));
	}
	else if(is_safe_now == CALL_POLICE_RET )
	{
		//log_info("You have call police,And the rescue is coming  !") ;
		this->ui->LblSafeOrNot->setText("正在救援");
		this->ui->LblSafeOrNot->setFont(QFont("simsun",36));

	}
	//log_debug("deal request  end...\n") ;
}