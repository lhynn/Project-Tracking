/****************************************************************************
** Meta object code from reading C++ file 'ctlwindows.h'
**
** Created: Tue Sep 26 02:34:48 2017
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ctlwindows.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ctlwindows.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Ctlwindows[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x09,
      25,   11,   11,   11, 0x09,
      38,   11,   11,   11, 0x09,
      51,   11,   11,   11, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_Ctlwindows[] = {
    "Ctlwindows\0\0timerDone1()\0timerDone2()\0"
    "timerDone3()\0timerDone4()\0"
};

const QMetaObject Ctlwindows::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Ctlwindows,
      qt_meta_data_Ctlwindows, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Ctlwindows::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Ctlwindows::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Ctlwindows::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Ctlwindows))
        return static_cast<void*>(const_cast< Ctlwindows*>(this));
    return QWidget::qt_metacast(_clname);
}

int Ctlwindows::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: timerDone1(); break;
        case 1: timerDone2(); break;
        case 2: timerDone3(); break;
        case 3: timerDone4(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
