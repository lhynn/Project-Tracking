/****************************************************************************
** Meta object code from reading C++ file 'sec_terminal.h'
**
** Created: Mon Jul 2 19:32:02 2018
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "sec_terminal.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sec_terminal.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Sec_Terminal[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      26,   13,   13,   13, 0x08,
      48,   39,   13,   13, 0x08,
      71,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Sec_Terminal[] = {
    "Sec_Terminal\0\0timerDone()\0LoadingIrq()\0"
    "Num,Type\0PhyButtonDeal(int,int)\0"
    "OsdUpdateFunc(int)\0"
};

const QMetaObject Sec_Terminal::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Sec_Terminal,
      qt_meta_data_Sec_Terminal, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Sec_Terminal::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Sec_Terminal::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Sec_Terminal::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Sec_Terminal))
        return static_cast<void*>(const_cast< Sec_Terminal*>(this));
    return QWidget::qt_metacast(_clname);
}

int Sec_Terminal::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: timerDone(); break;
        case 1: LoadingIrq(); break;
        case 2: PhyButtonDeal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: OsdUpdateFunc((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
