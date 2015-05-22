#include <QApplication>
#include <QTextCodec>
#include "mpdc.h"

char server_ip_address[20] ;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("utf8"); //GBK gbk
      QTextCodec::setCodecForTr(codec);
      QTextCodec::setCodecForLocale(codec);
      QTextCodec::setCodecForCStrings(codec);
    MPdc w;
    if(argc <2)
    {
        printf("please enter server IP \n") ;
        return 0 ;
    }
    strcpy(server_ip_address,argv[1]) ;
    w.show();
    
    return a.exec();
}
