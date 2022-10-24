/****************************************************************************
** Meta object code from reading C++ file 'AudiometerManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../managers/AudiometerManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AudiometerManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AudiometerManager_t {
    QByteArrayData data[6];
    char stringdata0[57];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AudiometerManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AudiometerManager_t qt_meta_stringdata_AudiometerManager = {
    {
QT_MOC_LITERAL(0, 0, 17), // "AudiometerManager"
QT_MOC_LITERAL(1, 18, 7), // "measure"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 6), // "finish"
QT_MOC_LITERAL(4, 34, 10), // "readDevice"
QT_MOC_LITERAL(5, 45, 11) // "writeDevice"

    },
    "AudiometerManager\0measure\0\0finish\0"
    "readDevice\0writeDevice"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AudiometerManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x0a /* Public */,
       3,    0,   35,    2, 0x0a /* Public */,
       4,    0,   36,    2, 0x08 /* Private */,
       5,    0,   37,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AudiometerManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AudiometerManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->measure(); break;
        case 1: _t->finish(); break;
        case 2: _t->readDevice(); break;
        case 3: _t->writeDevice(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject AudiometerManager::staticMetaObject = { {
    QMetaObject::SuperData::link<SerialPortManager::staticMetaObject>(),
    qt_meta_stringdata_AudiometerManager.data,
    qt_meta_data_AudiometerManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AudiometerManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AudiometerManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AudiometerManager.stringdata0))
        return static_cast<void*>(this);
    return SerialPortManager::qt_metacast(_clname);
}

int AudiometerManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SerialPortManager::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
