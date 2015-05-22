/****************************************************************************
** Meta object code from reading C++ file 'mpdc.h'
**
** Created: Tue Jul 1 08:02:46 2014
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mpdc.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mpdc.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MPdc[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
       6,    5,    5,    5, 0x0a,
      17,    5,    5,    5, 0x0a,
      45,    5,    5,    5, 0x0a,
      72,    5,    5,    5, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MPdc[] = {
    "MPdc\0\0GetVideo()\0on_BtnTakePicture_clicked()\0"
    "on_BtnCallPolice_clicked()\0showDateTime()\0"
};

const QMetaObject MPdc::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MPdc,
      qt_meta_data_MPdc, 0 }
};

const QMetaObject *MPdc::metaObject() const
{
    return &staticMetaObject;
}

void *MPdc::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MPdc))
	return static_cast<void*>(const_cast< MPdc*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MPdc::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: GetVideo(); break;
        case 1: on_BtnTakePicture_clicked(); break;
        case 2: on_BtnCallPolice_clicked(); break;
        case 3: showDateTime(); break;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
