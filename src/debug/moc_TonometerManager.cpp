/****************************************************************************
** Meta object code from reading C++ file 'TonometerManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../managers/TonometerManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TonometerManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TonometerManager_t {
    QByteArrayData data[13];
    char stringdata0[157];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TonometerManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TonometerManager_t qt_meta_stringdata_TonometerManager = {
    {
QT_MOC_LITERAL(0, 0, 16), // "TonometerManager"
QT_MOC_LITERAL(1, 17, 16), // "runnableSelected"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 17), // "canSelectRunnable"
QT_MOC_LITERAL(4, 53, 16), // "databaseSelected"
QT_MOC_LITERAL(5, 70, 17), // "canSelectDatabase"
QT_MOC_LITERAL(6, 88, 5), // "start"
QT_MOC_LITERAL(7, 94, 7), // "measure"
QT_MOC_LITERAL(8, 102, 6), // "finish"
QT_MOC_LITERAL(9, 109, 14), // "selectRunnable"
QT_MOC_LITERAL(10, 124, 14), // "selectDatabase"
QT_MOC_LITERAL(11, 139, 6), // "select"
QT_MOC_LITERAL(12, 146, 10) // "readOutput"

    },
    "TonometerManager\0runnableSelected\0\0"
    "canSelectRunnable\0databaseSelected\0"
    "canSelectDatabase\0start\0measure\0finish\0"
    "selectRunnable\0selectDatabase\0select\0"
    "readOutput"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TonometerManager[] = {

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
       9,    1,   76,    2, 0x0a /* Public */,
      10,    1,   79,    2, 0x0a /* Public */,
      11,    0,   82,    2, 0x0a /* Public */,
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
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TonometerManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TonometerManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->runnableSelected(); break;
        case 1: _t->canSelectRunnable(); break;
        case 2: _t->databaseSelected(); break;
        case 3: _t->canSelectDatabase(); break;
        case 4: _t->start(); break;
        case 5: _t->measure(); break;
        case 6: _t->finish(); break;
        case 7: _t->selectRunnable((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->selectDatabase((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->select(); break;
        case 10: _t->readOutput(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TonometerManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TonometerManager::runnableSelected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TonometerManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TonometerManager::canSelectRunnable)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TonometerManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TonometerManager::databaseSelected)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TonometerManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TonometerManager::canSelectDatabase)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TonometerManager::staticMetaObject = { {
    QMetaObject::SuperData::link<ManagerBase::staticMetaObject>(),
    qt_meta_stringdata_TonometerManager.data,
    qt_meta_data_TonometerManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TonometerManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TonometerManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TonometerManager.stringdata0))
        return static_cast<void*>(this);
    return ManagerBase::qt_metacast(_clname);
}

int TonometerManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void TonometerManager::runnableSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TonometerManager::canSelectRunnable()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TonometerManager::databaseSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TonometerManager::canSelectDatabase()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE