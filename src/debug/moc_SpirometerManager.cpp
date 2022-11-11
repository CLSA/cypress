/****************************************************************************
** Meta object code from reading C++ file 'SpirometerManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../managers/SpirometerManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SpirometerManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SpirometerManager_t {
    QByteArrayData data[13];
    char stringdata0[158];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SpirometerManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SpirometerManager_t qt_meta_stringdata_SpirometerManager = {
    {
QT_MOC_LITERAL(0, 0, 17), // "SpirometerManager"
QT_MOC_LITERAL(1, 18, 16), // "runnableSelected"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 17), // "canSelectRunnable"
QT_MOC_LITERAL(4, 54, 17), // "canSelectDataPath"
QT_MOC_LITERAL(5, 72, 16), // "dataPathSelected"
QT_MOC_LITERAL(6, 89, 5), // "start"
QT_MOC_LITERAL(7, 95, 7), // "measure"
QT_MOC_LITERAL(8, 103, 6), // "finish"
QT_MOC_LITERAL(9, 110, 6), // "select"
QT_MOC_LITERAL(10, 117, 14), // "selectRunnable"
QT_MOC_LITERAL(11, 132, 14), // "selectDataPath"
QT_MOC_LITERAL(12, 147, 10) // "readOutput"

    },
    "SpirometerManager\0runnableSelected\0\0"
    "canSelectRunnable\0canSelectDataPath\0"
    "dataPathSelected\0start\0measure\0finish\0"
    "select\0selectRunnable\0selectDataPath\0"
    "readOutput"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SpirometerManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,
       3,    0,   70,    2, 0x06 /* Public */,
       4,    0,   71,    2, 0x06 /* Public */,
       5,    0,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   73,    2, 0x0a /* Public */,
       7,    0,   74,    2, 0x0a /* Public */,
       8,    0,   75,    2, 0x0a /* Public */,
       9,    0,   76,    2, 0x0a /* Public */,
      10,    1,   77,    2, 0x0a /* Public */,
      11,    1,   80,    2, 0x0a /* Public */,
      12,    0,   83,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,

       0        // eod
};

void SpirometerManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SpirometerManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->runnableSelected(); break;
        case 1: _t->canSelectRunnable(); break;
        case 2: _t->canSelectDataPath(); break;
        case 3: _t->dataPathSelected(); break;
        case 4: _t->start(); break;
        case 5: _t->measure(); break;
        case 6: _t->finish(); break;
        case 7: _t->select(); break;
        case 8: _t->selectRunnable((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->selectDataPath((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->readOutput(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SpirometerManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SpirometerManager::runnableSelected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SpirometerManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SpirometerManager::canSelectRunnable)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SpirometerManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SpirometerManager::canSelectDataPath)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SpirometerManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SpirometerManager::dataPathSelected)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SpirometerManager::staticMetaObject = { {
    QMetaObject::SuperData::link<ManagerBase::staticMetaObject>(),
    qt_meta_stringdata_SpirometerManager.data,
    qt_meta_data_SpirometerManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SpirometerManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SpirometerManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SpirometerManager.stringdata0))
        return static_cast<void*>(this);
    return ManagerBase::qt_metacast(_clname);
}

int SpirometerManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ManagerBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void SpirometerManager::runnableSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SpirometerManager::canSelectRunnable()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SpirometerManager::canSelectDataPath()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void SpirometerManager::dataPathSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
