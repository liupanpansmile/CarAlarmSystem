#include <QApplication>
#include <QTextCodec>
#include "CASys.h"

//char server_ip_address[20] ;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("utf8"); //GBK gbk
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    CASys w;
   
    w.show();
    
    return a.exec();
}
