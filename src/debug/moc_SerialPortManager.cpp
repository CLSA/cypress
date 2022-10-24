/****************************************************************************
** Meta object code from reading C++ file 'SerialPortManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../managers/SerialPortManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SerialPortManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SerialPortManager_t {
    QByteArrayData data[17];
    char stringdata0[228];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialPortManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialPortManager_t qt_meta_stringdata_SerialPortManager = {
    {
QT_MOC_LITERAL(0, 0, 17), // "SerialPortManager"
QT_MOC_LITERAL(1, 18, 15), // "scanningDevices"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 16), // "deviceDiscovered"
QT_MOC_LITERAL(4, 52, 15), // "canSelectDevice"
QT_MOC_LITERAL(5, 68, 14), // "deviceSelected"
QT_MOC_LITERAL(6, 83, 16), // "canConnectDevice"
QT_MOC_LITERAL(7, 100, 17), // "deviceNameChanged"
QT_MOC_LITERAL(8, 118, 5), // "start"
QT_MOC_LITERAL(9, 124, 13), // "connectDevice"
QT_MOC_LITERAL(10, 138, 16), // "disconnectDevice"
QT_MOC_LITERAL(11, 155, 12), // "selectDevice"
QT_MOC_LITERAL(12, 168, 11), // "writeDevice"
QT_MOC_LITERAL(13, 180, 10), // "readDevice"
QT_MOC_LITERAL(14, 191, 9), // "setDevice"
QT_MOC_LITERAL(15, 201, 15), // "QSerialPortInfo"
QT_MOC_LITERAL(16, 217, 10) // "deviceName"

    },
    "SerialPortManager\0scanningDevices\0\0"
    "deviceDiscovered\0canSelectDevice\0"
    "deviceSelected\0canConnectDevice\0"
    "deviceNameChanged\0start\0connectDevice\0"
    "disconnectDevice\0selectDevice\0writeDevice\0"
    "readDevice\0setDevice\0QSerialPortInfo\0"
    "deviceName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialPortManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       1,  102, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    1,   80,    2, 0x06 /* Public */,
       4,    0,   83,    2, 0x06 /* Public */,
       5,    1,   84,    2, 0x06 /* Public */,
       6,    0,   87,    2, 0x06 /* Public */,
       7,    1,   88,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   91,    2, 0x0a /* Public */,
       9,    0,   92,    2, 0x0a /* Public */,
      10,    0,   93,    2, 0x0a /* Public */,
      11,    1,   94,    2, 0x0a /* Public */,
      12,    0,   97,    2, 0x08 /* Private */,
      13,    0,   98,    2, 0x08 /* Private */,
      14,    1,   99,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 15,    2,

 // properties: name, type, flags
      16, QMetaType::QString, 0x00495003,

 // properties: notify_signal_id
       5,

       0        // eod
};

void SerialPortManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SerialPortManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->scanningDevices(); break;
        case 1: _t->deviceDiscovered((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->canSelectDevice(); break;
        case 3: _t->deviceSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->canConnectDevice(); break;
        case 5: _t->deviceNameChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->start(); break;
        case 7: _t->connectDevice(); break;
        case 8: _t->disconnectDevice(); break;
        case 9: _t->selectDevice((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->writeDevice(); break;
        case 11: _t->readDevice(); break;
        case 12: _t->setDevice((*reinterpret_cast< const QSerialPortInfo(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SerialPortManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortManager::scanningDevices)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SerialPortManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortManager::deviceDiscovered)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SerialPortManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortManager::canSelectDevice)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SerialPortManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortManager::deviceSelected)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SerialPortManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortManager::canConnectDevice)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (SerialPortManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortManager::deviceNameChanged)) {
                *result = 5;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<SerialPortManager *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->m_deviceName; break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<SerialPortManager *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0:
            if (_t->m_deviceName != *reinterpret_cast< QString*>(_v)) {
                _t->m_deviceName = *reinterpret_cast< QString*>(_v);
                Q_EMIT _t->deviceNameChanged(_t->m_deviceName);
            }
            break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject SerialPortManager::staticMetaObject = { {
    QMetaObject::SuperData::link<ManagerBase::staticMetaObject>(),
    qt_meta_stringdata_SerialPortManager.data,
    qt_meta_data_SerialPortManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SerialPortManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SerialPortManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SerialPortManager.stringdata0))
        return static_cast<void*>(this);
    return ManagerBase::qt_metacast(_clname);
}

int SerialPortManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ManagerBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void SerialPortManager::scanningDevices()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SerialPortManager::deviceDiscovered(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SerialPortManager::canSelectDevice()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void SerialPortManager::deviceSelected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SerialPortManager::canConnectDevice()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void SerialPortManager::deviceNameChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
