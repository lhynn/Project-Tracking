/****************************************************************************
** Meta object code from reading C++ file 'messageserver.h'
**
** Created: Wed May 2 18:37:47 2018
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "messageserver.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'messageserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MessageServer[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: signature, parameters, type, tag, flags
      36,   15,   14,   14, 0x05,
      67,   14,   14,   14, 0x05,
      77,   14,   14,   14, 0x05,
      88,   14,   14,   14, 0x05,
      98,   14,   14,   14, 0x05,
     107,   14,   14,   14, 0x05,
     116,   14,   14,   14, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_MessageServer[] = {
    "MessageServer\0\0position,label,value\0"
    "statusMsg(int,QString,QString)\0playMsg()\0"
    "pauseMsg()\0stopMsg()\0incMsg()\0decMsg()\0"
    "hideMsg()\0"
};

const QMetaObject MessageServer::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_MessageServer,
      qt_meta_data_MessageServer, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MessageServer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MessageServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MessageServer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MessageServer))
        return static_cast<void*>(const_cast< MessageServer*>(this));
    return QThread::qt_metacast(_clname);
}

int MessageServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: statusMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 1: playMsg(); break;
        case 2: pauseMsg(); break;
        case 3: stopMsg(); break;
        case 4: incMsg(); break;
        case 5: decMsg(); break;
        case 6: hideMsg(); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void MessageServer::statusMsg(int _t1, QString _t2, QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MessageServer::playMsg()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void MessageServer::pauseMsg()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void MessageServer::stopMsg()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void MessageServer::incMsg()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void MessageServer::decMsg()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void MessageServer::hideMsg()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}
QT_END_MOC_NAMESPACE
