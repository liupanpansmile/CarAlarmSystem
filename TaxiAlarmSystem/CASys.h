#ifndef CASys_H
#define CASys_H

#include <QMainWindow>
#include <QtGui/QWidget>
#include <QtGui/QCloseEvent>
#include <QtGui/QLabel>
#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QtGui/QPainter>
#include<QtGui/QDateEdit>
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtGui/QPushButton>
#include <QtGui/QApplication>
#include "ui_CASys.h"
#include "v4l.h"
#include "pub.h"
#include "config.h"

namespace Ui {
class CASys;
}

class CASys : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit CASys(QWidget *parent = 0);
    ~CASys();
    QTimer *timer;
    QTimer *datetime_timer ;
    v4l_device  v4l_dev0;
    void closeEvent(QCloseEvent *);
    void deal_request() ;

public slots:

void GetVideo();
    
void on_BtnTakePicture_clicked();

void on_BtnCallPolice_clicked();

void showDateTime() ;
private:
    Ui::CASys *ui;
    AllInfo *allInfo ;			//所有信息

	char serverIP[128] ;
	int port ;
	Config *config ;


private:
	void Init() ;
};

#endif // MPDC_H
