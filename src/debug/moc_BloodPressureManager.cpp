/****************************************************************************
** Meta object code from reading C++ file 'BloodPressureManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../managers/BloodPressureManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BloodPressureManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BloodPressureManager_t {
    QByteArrayData data[32];
    char stringdata0[448];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BloodPressureManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BloodPressureManager_t qt_meta_stringdata_BloodPressureManager = {
    {
QT_MOC_LITERAL(0, 0, 20), // "BloodPressureManager"
QT_MOC_LITERAL(1, 21, 17), // "attemptConnection"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 8), // "QUsb::Id"
QT_MOC_LITERAL(4, 49, 16), // "startMeasurement"
QT_MOC_LITERAL(5, 66, 16), // "abortMeasurement"
QT_MOC_LITERAL(6, 83, 8), // "QThread*"
QT_MOC_LITERAL(7, 92, 16), // "canConnectDevice"
QT_MOC_LITERAL(8, 109, 15), // "scanningDevices"
QT_MOC_LITERAL(9, 125, 16), // "deviceDiscovered"
QT_MOC_LITERAL(10, 142, 15), // "canSelectDevice"
QT_MOC_LITERAL(11, 158, 14), // "deviceSelected"
QT_MOC_LITERAL(12, 173, 17), // "deviceNameChanged"
QT_MOC_LITERAL(13, 191, 11), // "sideChanged"
QT_MOC_LITERAL(14, 203, 15), // "cuffSizeChanged"
QT_MOC_LITERAL(15, 219, 5), // "start"
QT_MOC_LITERAL(16, 225, 7), // "measure"
QT_MOC_LITERAL(17, 233, 6), // "finish"
QT_MOC_LITERAL(18, 240, 13), // "connectDevice"
QT_MOC_LITERAL(19, 254, 16), // "disconnectDevice"
QT_MOC_LITERAL(20, 271, 12), // "selectDevice"
QT_MOC_LITERAL(21, 284, 11), // "scanDevices"
QT_MOC_LITERAL(22, 296, 20), // "measurementAvailable"
QT_MOC_LITERAL(23, 317, 16), // "averageAvailable"
QT_MOC_LITERAL(24, 334, 20), // "finalReviewAvailable"
QT_MOC_LITERAL(25, 355, 23), // "connectionStatusChanged"
QT_MOC_LITERAL(26, 379, 13), // "abortComplete"
QT_MOC_LITERAL(27, 393, 19), // "deviceInfoAvailable"
QT_MOC_LITERAL(28, 413, 9), // "setDevice"
QT_MOC_LITERAL(29, 423, 10), // "deviceName"
QT_MOC_LITERAL(30, 434, 8), // "cuffSize"
QT_MOC_LITERAL(31, 443, 4) // "side"

    },
    "BloodPressureManager\0attemptConnection\0"
    "\0QUsb::Id\0startMeasurement\0abortMeasurement\0"
    "QThread*\0canConnectDevice\0scanningDevices\0"
    "deviceDiscovered\0canSelectDevice\0"
    "deviceSelected\0deviceNameChanged\0"
    "sideChanged\0cuffSizeChanged\0start\0"
    "measure\0finish\0connectDevice\0"
    "disconnectDevice\0selectDevice\0scanDevices\0"
    "measurementAvailable\0averageAvailable\0"
    "finalReviewAvailable\0connectionStatusChanged\0"
    "abortComplete\0deviceInfoAvailable\0"
    "setDevice\0deviceName\0cuffSize\0side"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BloodPressureManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   14, // methods
       3,  210, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  139,    2, 0x06 /* Public */,
       4,    0,  142,    2, 0x06 /* Public */,
       5,    1,  143,    2, 0x06 /* Public */,
       7,    0,  146,    2, 0x06 /* Public */,
       8,    0,  147,    2, 0x06 /* Public */,
       9,    1,  148,    2, 0x06 /* Public */,
      10,    0,  151,    2, 0x06 /* Public */,
      11,    1,  152,    2, 0x06 /* Public */,
      12,    1,  155,    2, 0x06 /* Public */,
      13,    1,  158,    2, 0x06 /* Public */,
      14,    1,  161,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    0,  164,    2, 0x0a /* Public */,
      16,    0,  165,    2, 0x0a /* Public */,
      17,    0,  166,    2, 0x0a /* Public */,
      18,    0,  167,    2, 0x0a /* Public */,
      19,    0,  168,    2, 0x0a /* Public */,
      20,    1,  169,    2, 0x0a /* Public */,
      21,    0,  172,    2, 0x0a /* Public */,
      22,    6,  173,    2, 0x08 /* Private */,
      23,    3,  186,    2, 0x08 /* Private */,
      24,    3,  193,    2, 0x08 /* Private */,
      25,    1,  200,    2, 0x08 /* Private */,
      26,    1,  203,    2, 0x08 /* Private */,
      27,    0,  206,    2, 0x08 /* Private */,
      28,    1,  207,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::QDateTime, QMetaType::QDateTime,    2,    2,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    2,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    2,    2,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    2,

 // properties: name, type, flags
      29, QMetaType::QString, 0x00495003,
      30, QMetaType::QString, 0x00495003,
      31, QMetaType::QString, 0x00495003,

 // properties: notify_signal_id
       8,
      10,
       9,

       0        // eod
};

void BloodPressureManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BloodPressureManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->attemptConnection((*reinterpret_cast< const QUsb::Id(*)>(_a[1]))); break;
        case 1: _t->startMeasurement(); break;
        case 2: _t->abortMeasurement((*reinterpret_cast< QThread*(*)>(_a[1]))); break;
        case 3: _t->canConnectDevice(); break;
        case 4: _t->scanningDevices(); break;
        case 5: _t->deviceDiscovered((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->canSelectDevice(); break;
        case 7: _t->deviceSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->deviceNameChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->sideChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->cuffSizeChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->start(); break;
        case 12: _t->measure(); break;
        case 13: _t->finish(); break;
        case 14: _t->connectDevice(); break;
        case 15: _t->disconnectDevice(); break;
        case 16: _t->selectDevice((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: _t->scanDevices(); break;
        case 18: _t->measurementAvailable((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4])),(*reinterpret_cast< const QDateTime(*)>(_a[5])),(*reinterpret_cast< const QDateTime(*)>(_a[6]))); break;
        case 19: _t->averageAvailable((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 20: _t->finalReviewAvailable((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 21: _t->connectionStatusChanged((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 22: _t->abortComplete((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 23: _t->deviceInfoAvailable(); break;
        case 24: _t->setDevice((*reinterpret_cast< const QUsb::Id(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QUsb::Id >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QThread* >(); break;
            }
            break;
        case 24:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QUsb::Id >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BloodPressureManager::*)(const QUsb::Id & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BloodPressureManager::attemptConnection)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BloodPressureManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BloodPressureManager::startMeasurement)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (BloodPressureManager::*)(QThread * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BloodPressureManager::abortMeasurement)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (BloodPressureManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BloodPressureManager::canConnectDevice)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (BloodPressureManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BloodPressureManager::scanningDevices)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (BloodPressureManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BloodPressureManager::deviceDiscovered)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (BloodPressureManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BloodPressureManager::canSelectDevice)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (BloodPressureManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BloodPressureManager::deviceSelected)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (BloodPressureManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BloodPressureManager::deviceNameChanged)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (BloodPressureManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BloodPressureManager::sideChanged)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (BloodPressureManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BloodPressureManager::cuffSizeChanged)) {
                *result = 10;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<BloodPressureManager *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->m_deviceName; break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->m_cuffSize; break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->m_side; break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<BloodPressureManager *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0:
            if (_t->m_deviceName != *reinterpret_cast< QString*>(_v)) {
                _t->m_deviceName = *reinterpret_cast< QString*>(_v);
                Q_EMIT _t->deviceNameChanged(_t->m_deviceName);
            }
            break;
        case 1:
            if (_t->m_cuffSize != *reinterpret_cast< QString*>(_v)) {
                _t->m_cuffSize = *reinterpret_cast< QString*>(_v);
                Q_EMIT _t->cuffSizeChanged(_t->m_cuffSize);
            }
            break;
        case 2:
            if (_t->m_side != *reinterpret_cast< QString*>(_v)) {
                _t->m_side = *reinterpret_cast< QString*>(_v);
                Q_EMIT _t->sideChanged(_t->m_side);
            }
            break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject BloodPressureManager::staticMetaObject = { {
    QMetaObject::SuperData::link<ManagerBase::staticMetaObject>(),
    qt_meta_stringdata_BloodPressureManager.data,
    qt_meta_data_BloodPressureManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BloodPressureManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BloodPressureManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BloodPressureManager.stringdata0))
        return static_cast<void*>(this);
    return ManagerBase::qt_metacast(_clname);
}

int BloodPressureManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ManagerBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void BloodPressureManager::attemptConnection(const QUsb::Id & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BloodPressureManager::startMeasurement()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void BloodPressureManager::abortMeasurement(QThread * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void BloodPressureManager::canConnectDevice()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void BloodPressureManager::scanningDevices()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void BloodPressureManager::deviceDiscovered(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void BloodPressureManager::canSelectDevice()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void BloodPressureManager::deviceSelected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void BloodPressureManager::deviceNameChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void BloodPressureManager::sideChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void BloodPressureManager::cuffSizeChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
