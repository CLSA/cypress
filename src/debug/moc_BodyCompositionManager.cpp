/****************************************************************************
** Meta object code from reading C++ file 'BodyCompositionManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../managers/BodyCompositionManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BodyCompositionManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BodyCompositionManager_t {
    QByteArrayData data[15];
    char stringdata0[181];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BodyCompositionManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BodyCompositionManager_t qt_meta_stringdata_BodyCompositionManager = {
    {
QT_MOC_LITERAL(0, 0, 22), // "BodyCompositionManager"
QT_MOC_LITERAL(1, 23, 8), // "canInput"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 10), // "canConfirm"
QT_MOC_LITERAL(4, 44, 5), // "error"
QT_MOC_LITERAL(5, 50, 17), // "notifyGenderInput"
QT_MOC_LITERAL(6, 68, 14), // "notifyAgeInput"
QT_MOC_LITERAL(7, 83, 17), // "notifyHeightInput"
QT_MOC_LITERAL(8, 101, 13), // "connectDevice"
QT_MOC_LITERAL(9, 115, 11), // "resetDevice"
QT_MOC_LITERAL(10, 127, 15), // "confirmSettings"
QT_MOC_LITERAL(11, 143, 7), // "measure"
QT_MOC_LITERAL(12, 151, 6), // "finish"
QT_MOC_LITERAL(13, 158, 10), // "readDevice"
QT_MOC_LITERAL(14, 169, 11) // "writeDevice"

    },
    "BodyCompositionManager\0canInput\0\0"
    "canConfirm\0error\0notifyGenderInput\0"
    "notifyAgeInput\0notifyHeightInput\0"
    "connectDevice\0resetDevice\0confirmSettings\0"
    "measure\0finish\0readDevice\0writeDevice"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BodyCompositionManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    0,   80,    2, 0x06 /* Public */,
       4,    1,   81,    2, 0x06 /* Public */,
       5,    1,   84,    2, 0x06 /* Public */,
       6,    1,   87,    2, 0x06 /* Public */,
       7,    1,   90,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   93,    2, 0x0a /* Public */,
       9,    0,   94,    2, 0x0a /* Public */,
      10,    0,   95,    2, 0x0a /* Public */,
      11,    0,   96,    2, 0x0a /* Public */,
      12,    0,   97,    2, 0x0a /* Public */,
      13,    0,   98,    2, 0x08 /* Private */,
      14,    0,   99,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
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
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void BodyCompositionManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BodyCompositionManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->canInput(); break;
        case 1: _t->canConfirm(); break;
        case 2: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->notifyGenderInput((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->notifyAgeInput((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->notifyHeightInput((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->connectDevice(); break;
        case 7: _t->resetDevice(); break;
        case 8: _t->confirmSettings(); break;
        case 9: _t->measure(); break;
        case 10: _t->finish(); break;
        case 11: _t->readDevice(); break;
        case 12: _t->writeDevice(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BodyCompositionManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BodyCompositionManager::canInput)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BodyCompositionManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BodyCompositionManager::canConfirm)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (BodyCompositionManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BodyCompositionManager::error)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (BodyCompositionManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BodyCompositionManager::notifyGenderInput)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (BodyCompositionManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BodyCompositionManager::notifyAgeInput)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (BodyCompositionManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BodyCompositionManager::notifyHeightInput)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BodyCompositionManager::staticMetaObject = { {
    QMetaObject::SuperData::link<SerialPortManager::staticMetaObject>(),
    qt_meta_stringdata_BodyCompositionManager.data,
    qt_meta_data_BodyCompositionManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BodyCompositionManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BodyCompositionManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BodyCompositionManager.stringdata0))
        return static_cast<void*>(this);
    return SerialPortManager::qt_metacast(_clname);
}

int BodyCompositionManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SerialPortManager::qt_metacall(_c, _id, _a);
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
    return _id;
}

// SIGNAL 0
void BodyCompositionManager::canInput()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void BodyCompositionManager::canConfirm()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void BodyCompositionManager::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void BodyCompositionManager::notifyGenderInput(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void BodyCompositionManager::notifyAgeInput(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void BodyCompositionManager::notifyHeightInput(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
