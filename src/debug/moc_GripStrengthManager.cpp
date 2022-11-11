/****************************************************************************
** Meta object code from reading C++ file 'GripStrengthManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../managers/GripStrengthManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GripStrengthManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GripStrengthManager_t {
    QByteArrayData data[7];
    char stringdata0[74];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GripStrengthManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GripStrengthManager_t qt_meta_stringdata_GripStrengthManager = {
    {
QT_MOC_LITERAL(0, 0, 19), // "GripStrengthManager"
QT_MOC_LITERAL(1, 20, 18), // "processInitialized"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 12), // "measureStart"
QT_MOC_LITERAL(4, 53, 5), // "start"
QT_MOC_LITERAL(5, 59, 7), // "measure"
QT_MOC_LITERAL(6, 67, 6) // "finish"

    },
    "GripStrengthManager\0processInitialized\0"
    "\0measureStart\0start\0measure\0finish"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GripStrengthManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    0,   40,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   41,    2, 0x0a /* Public */,
       5,    0,   42,    2, 0x0a /* Public */,
       6,    0,   43,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void GripStrengthManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GripStrengthManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->processInitialized(); break;
        case 1: _t->measureStart(); break;
        case 2: _t->start(); break;
        case 3: _t->measure(); break;
        case 4: _t->finish(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GripStrengthManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GripStrengthManager::processInitialized)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (GripStrengthManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GripStrengthManager::measureStart)) {
                *result = 1;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject GripStrengthManager::staticMetaObject = { {
    QMetaObject::SuperData::link<ManagerBase::staticMetaObject>(),
    qt_meta_stringdata_GripStrengthManager.data,
    qt_meta_data_GripStrengthManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GripStrengthManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GripStrengthManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GripStrengthManager.stringdata0))
        return static_cast<void*>(this);
    return ManagerBase::qt_metacast(_clname);
}

int GripStrengthManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ManagerBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void GripStrengthManager::processInitialized()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void GripStrengthManager::measureStart()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
