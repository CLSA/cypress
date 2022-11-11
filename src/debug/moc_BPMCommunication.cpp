/****************************************************************************
** Meta object code from reading C++ file 'BPMCommunication.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../managers/BPMCommunication.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BPMCommunication.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BPMCommunication_t {
    QByteArrayData data[24];
    char stringdata0[240];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BPMCommunication_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BPMCommunication_t qt_meta_stringdata_BPMCommunication = {
    {
QT_MOC_LITERAL(0, 0, 16), // "BPMCommunication"
QT_MOC_LITERAL(1, 17, 16), // "connectionStatus"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 15), // "deviceInfoReady"
QT_MOC_LITERAL(4, 51, 16), // "measurementReady"
QT_MOC_LITERAL(5, 68, 10), // "readingNum"
QT_MOC_LITERAL(6, 79, 3), // "sbp"
QT_MOC_LITERAL(7, 83, 3), // "dbp"
QT_MOC_LITERAL(8, 87, 5), // "pulse"
QT_MOC_LITERAL(9, 93, 5), // "start"
QT_MOC_LITERAL(10, 99, 3), // "end"
QT_MOC_LITERAL(11, 103, 12), // "averageReady"
QT_MOC_LITERAL(12, 116, 16), // "finalReviewReady"
QT_MOC_LITERAL(13, 133, 16), // "measurementError"
QT_MOC_LITERAL(14, 150, 5), // "error"
QT_MOC_LITERAL(15, 156, 7), // "timeout"
QT_MOC_LITERAL(16, 164, 13), // "abortFinished"
QT_MOC_LITERAL(17, 178, 10), // "successful"
QT_MOC_LITERAL(18, 189, 7), // "connect"
QT_MOC_LITERAL(19, 197, 8), // "QUsb::Id"
QT_MOC_LITERAL(20, 206, 10), // "disconnect"
QT_MOC_LITERAL(21, 217, 7), // "measure"
QT_MOC_LITERAL(22, 225, 5), // "abort"
QT_MOC_LITERAL(23, 231, 8) // "QThread*"

    },
    "BPMCommunication\0connectionStatus\0\0"
    "deviceInfoReady\0measurementReady\0"
    "readingNum\0sbp\0dbp\0pulse\0start\0end\0"
    "averageReady\0finalReviewReady\0"
    "measurementError\0error\0timeout\0"
    "abortFinished\0successful\0connect\0"
    "QUsb::Id\0disconnect\0measure\0abort\0"
    "QThread*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BPMCommunication[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       3,    0,   77,    2, 0x06 /* Public */,
       4,    6,   78,    2, 0x06 /* Public */,
      11,    3,   91,    2, 0x06 /* Public */,
      12,    3,   98,    2, 0x06 /* Public */,
      13,    2,  105,    2, 0x06 /* Public */,
      13,    1,  110,    2, 0x26 /* Public | MethodCloned */,
      16,    1,  113,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      18,    1,  116,    2, 0x0a /* Public */,
      20,    0,  119,    2, 0x0a /* Public */,
      21,    0,  120,    2, 0x0a /* Public */,
      22,    1,  121,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::QDateTime, QMetaType::QDateTime,    5,    6,    7,    8,    9,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    6,    7,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    6,    7,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   14,   15,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::Bool,   17,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 19,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 23,    2,

       0        // eod
};

void BPMCommunication::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BPMCommunication *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connectionStatus((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 1: _t->deviceInfoReady(); break;
        case 2: _t->measurementReady((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4])),(*reinterpret_cast< const QDateTime(*)>(_a[5])),(*reinterpret_cast< const QDateTime(*)>(_a[6]))); break;
        case 3: _t->averageReady((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 4: _t->finalReviewReady((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 5: _t->measurementError((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 6: _t->measurementError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->abortFinished((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 8: _t->connect((*reinterpret_cast< const QUsb::Id(*)>(_a[1]))); break;
        case 9: _t->disconnect(); break;
        case 10: _t->measure(); break;
        case 11: _t->abort((*reinterpret_cast< QThread*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QUsb::Id >(); break;
            }
            break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QThread* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BPMCommunication::*)(const bool & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BPMCommunication::connectionStatus)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BPMCommunication::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BPMCommunication::deviceInfoReady)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (BPMCommunication::*)(const int & , const int & , const int & , const int & , const QDateTime & , const QDateTime & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BPMCommunication::measurementReady)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (BPMCommunication::*)(const int & , const int & , const int & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BPMCommunication::averageReady)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (BPMCommunication::*)(const int & , const int & , const int & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BPMCommunication::finalReviewReady)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (BPMCommunication::*)(const QString & , const int & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BPMCommunication::measurementError)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (BPMCommunication::*)(const bool & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BPMCommunication::abortFinished)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BPMCommunication::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_BPMCommunication.data,
    qt_meta_data_BPMCommunication,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BPMCommunication::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BPMCommunication::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BPMCommunication.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int BPMCommunication::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void BPMCommunication::connectionStatus(const bool & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BPMCommunication::deviceInfoReady()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void BPMCommunication::measurementReady(const int & _t1, const int & _t2, const int & _t3, const int & _t4, const QDateTime & _t5, const QDateTime & _t6)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t6))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void BPMCommunication::averageReady(const int & _t1, const int & _t2, const int & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void BPMCommunication::finalReviewReady(const int & _t1, const int & _t2, const int & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void BPMCommunication::measurementError(const QString & _t1, const int & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 7
void BPMCommunication::abortFinished(const bool & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
