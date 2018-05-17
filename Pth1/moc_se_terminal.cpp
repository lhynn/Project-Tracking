/****************************************************************************
** Meta object code from reading C++ file 'se_terminal.h'
**
** Created: Wed May 2 18:37:51 2018
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "se_terminal.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'se_terminal.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_se_terminal[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   13,   12,   12, 0x08,
      45,   36,   12,   12, 0x08,
      78,   68,   12,   12, 0x08,
     111,   12,   12,   12, 0x08,
     130,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_se_terminal[] = {
    "se_terminal\0\0x\0TouchButtonDeal(int)\0"
    "Num,Type\0PhyButtonDeal(int,int)\0"
    "x,y,press\0TouchRockerDeal(float,float,int)\0"
    "OsdUpdateFunc(int)\0timerDone()\0"
};

const QMetaObject se_terminal::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_se_terminal,
      qt_meta_data_se_terminal, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &se_terminal::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *se_terminal::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *se_terminal::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_se_terminal))
        return static_cast<void*>(const_cast< se_terminal*>(this));
    return QWidget::qt_metacast(_clname);
}

int se_terminal::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: TouchButtonDeal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: PhyButtonDeal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: TouchRockerDeal((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: OsdUpdateFunc((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: timerDone(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
