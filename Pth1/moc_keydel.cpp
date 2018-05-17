/****************************************************************************
** Meta object code from reading C++ file 'keydel.h'
**
** Created: Wed May 2 18:37:46 2018
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "keydel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'keydel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_KeyDel[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,    8,    7,    7, 0x05,
      32,    7,    7,    7, 0x05,
      55,   52,    7,    7, 0x05,
      82,    7,    7,    7, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_KeyDel[] = {
    "KeyDel\0\0,\0PhyButtonMsg(int,int)\0"
    "TouchButtonMsg(int)\0,,\0"
    "RockerSgl(float,float,int)\0OsdUpdate(int)\0"
};

const QMetaObject KeyDel::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_KeyDel,
      qt_meta_data_KeyDel, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KeyDel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KeyDel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KeyDel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KeyDel))
        return static_cast<void*>(const_cast< KeyDel*>(this));
    return QThread::qt_metacast(_clname);
}

int KeyDel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: PhyButtonMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: TouchButtonMsg((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: RockerSgl((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: OsdUpdate((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void KeyDel::PhyButtonMsg(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void KeyDel::TouchButtonMsg(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void KeyDel::RockerSgl(float _t1, float _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void KeyDel::OsdUpdate(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
