/********************************************************************************
** Form generated from reading ui file 'mpdc.ui'
**
** Created: Tue Jul 1 08:02:33 2014
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CASys_H
#define UI_CASys_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class UI_CASys
{
public:
    QWidget *centralWidget;
    QLabel *LblShowPic;
    QPushButton *BtnTakePicture;
    QPushButton *BtnCallPolice;
    QLabel *LblDateTime;
    QLabel *LblSafeOrNot;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MPdc)
    {
    if (MPdc->objectName().isEmpty())
        MPdc->setObjectName(QString::fromUtf8("MPdc"));
    MPdc->resize(800, 480);
    MPdc->setStyleSheet(QString::fromUtf8(""));
    centralWidget = new QWidget(MPdc);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
    centralWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(190, 255, 200);"));
    LblShowPic = new QLabel(centralWidget);
    LblShowPic->setObjectName(QString::fromUtf8("LblShowPic"));
    LblShowPic->setGeometry(QRect(40, 30, 320, 240));
    BtnTakePicture = new QPushButton(centralWidget);
    BtnTakePicture->setObjectName(QString::fromUtf8("BtnTakePicture"));
    BtnTakePicture->setGeometry(QRect(30, 330, 141, 61));
    QFont font;
    font.setFamily(QString::fromUtf8("Abyssinica SIL"));
    font.setPointSize(48);
    BtnTakePicture->setFont(font);
    BtnCallPolice = new QPushButton(centralWidget);
    BtnCallPolice->setObjectName(QString::fromUtf8("BtnCallPolice"));
    BtnCallPolice->setGeometry(QRect(200, 330, 141, 61));
    BtnCallPolice->setFont(font);
    BtnCallPolice->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
    LblDateTime = new QLabel(centralWidget);
    LblDateTime->setObjectName(QString::fromUtf8("LblDateTime"));
    LblDateTime->setGeometry(QRect(380, 40, 421, 20));
    QFont font1;
    font1.setFamily(QString::fromUtf8("Abyssinica SIL"));
    font1.setPointSize(36);
    LblDateTime->setFont(font1);
    LblSafeOrNot = new QLabel(centralWidget);
    LblSafeOrNot->setObjectName(QString::fromUtf8("LblSafeOrNot"));
    LblSafeOrNot->setGeometry(QRect(385, 336, 101, 41));
    LblSafeOrNot->setFont(font);
    MPdc->setCentralWidget(centralWidget);
    menuBar = new QMenuBar(MPdc);
    menuBar->setObjectName(QString::fromUtf8("menuBar"));
    menuBar->setGeometry(QRect(0, 0, 800, 25));
    MPdc->setMenuBar(menuBar);
    mainToolBar = new QToolBar(MPdc);
    mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
    MPdc->addToolBar(Qt::TopToolBarArea, mainToolBar);
    statusBar = new QStatusBar(MPdc);
    statusBar->setObjectName(QString::fromUtf8("statusBar"));
    MPdc->setStatusBar(statusBar);

    retranslateUi(MPdc);

    QMetaObject::connectSlotsByName(MPdc);
    } // setupUi

    void retranslateUi(QMainWindow *MPdc)
    {
    MPdc->setWindowTitle(QApplication::translate("MPdc", "MPdc", 0, QApplication::UnicodeUTF8));
    LblShowPic->setText(QApplication::translate("MPdc", "cam1", 0, QApplication::UnicodeUTF8));
    BtnTakePicture->setText(QApplication::translate("MPdc", "\346\213\215\347\205\247", 0, QApplication::UnicodeUTF8));
    BtnCallPolice->setText(QApplication::translate("MPdc", "\346\212\245\350\255\246", 0, QApplication::UnicodeUTF8));
    LblDateTime->setText(QString());
    LblSafeOrNot->setText(QApplication::translate("MPdc", "\345\256\211\345\205\250", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(MPdc);
    } // retranslateUi

};

namespace Ui {
    class CASys: public UI_CASys {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CASys_H
