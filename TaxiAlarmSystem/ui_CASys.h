/********************************************************************************
** Form generated from reading ui file 'casys.ui'
**
** Created: Wed May 20 12:31:30 2015
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CASYS_H
#define UI_CASYS_H

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

class Ui_CASys
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

    void setupUi(QMainWindow *CASys)
    {
    if (CASys->objectName().isEmpty())
        CASys->setObjectName(QString::fromUtf8("CASys"));
    CASys->resize(800, 480);
    CASys->setStyleSheet(QString::fromUtf8(""));
    centralWidget = new QWidget(CASys);
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
    CASys->setCentralWidget(centralWidget);
    menuBar = new QMenuBar(CASys);
    menuBar->setObjectName(QString::fromUtf8("menuBar"));
    menuBar->setGeometry(QRect(0, 0, 800, 25));
    CASys->setMenuBar(menuBar);
    mainToolBar = new QToolBar(CASys);
    mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
    CASys->addToolBar(Qt::TopToolBarArea, mainToolBar);
    statusBar = new QStatusBar(CASys);
    statusBar->setObjectName(QString::fromUtf8("statusBar"));
    CASys->setStatusBar(statusBar);

    retranslateUi(CASys);

    QMetaObject::connectSlotsByName(CASys);
    } // setupUi

    void retranslateUi(QMainWindow *CASys)
    {
    CASys->setWindowTitle(QApplication::translate("CASys", "CASys", 0, QApplication::UnicodeUTF8));
    LblShowPic->setText(QApplication::translate("CASys", "cam1", 0, QApplication::UnicodeUTF8));
    BtnTakePicture->setText(QApplication::translate("CASys", "\346\213\215\347\205\247", 0, QApplication::UnicodeUTF8));
    BtnCallPolice->setText(QApplication::translate("CASys", "\346\212\245\350\255\246", 0, QApplication::UnicodeUTF8));
    LblDateTime->setText(QString());
    LblSafeOrNot->setText(QApplication::translate("CASys", "\345\256\211\345\205\250", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(CASys);
    } // retranslateUi

};

namespace Ui {
    class CASys: public Ui_CASys {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CASYS_H
