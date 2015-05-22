#ifndef MPDC_H
#define MPDC_H

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
#include "ui_mpdc.h"
#include "v4l.h"
#include "pub.h"

namespace Ui {
class MPdc;
}

class MPdc : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MPdc(QWidget *parent = 0);
    ~MPdc();
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
    Ui::MPdc *ui;
    AllInfo *allInfo ;      //所有信息
    char carInfoFile[16] ;     //车的信息文件名
    char pictureName[16] ; //保存图片的文件名
};

#endif // MPDC_H
