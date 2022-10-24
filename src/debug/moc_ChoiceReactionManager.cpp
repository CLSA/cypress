/****************************************************************************
** Meta object code from reading C++ file 'ChoiceReactionManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../managers/ChoiceReactionManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ChoiceReactionManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ChoiceReactionManager_t {
    QByteArrayData data[9];
    char stringdata0[105];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ChoiceReactionManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ChoiceReactionManager_t qt_meta_stringdata_ChoiceReactionManager = {
    {
QT_MOC_LITERAL(0, 0, 21), // "ChoiceReactionManager"
QT_MOC_LITERAL(1, 22, 16), // "runnableSelected"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 17), // "canSelectRunnable"
QT_MOC_LITERAL(4, 58, 5), // "start"
QT_MOC_LITERAL(5, 64, 7), // "measure"
QT_MOC_LITERAL(6, 72, 6), // "finish"
QT_MOC_LITERAL(7, 79, 14), // "selectRunnable"
QT_MOC_LITERAL(8, 94, 10) // "readOutput"

    },
    "ChoiceReactionManager\0runnableSelected\0"
    "\0canSelectRunnable\0start\0measure\0"
    "finish\0selectRunnable\0readOutput"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChoiceReactionManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    0,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   51,    2, 0x0a /* Public */,
       5,    0,   52,    2, 0x0a /* Public */,
       6,    0,   53,    2, 0x0a /* Public */,
       7,    1,   54,    2, 0x0a /* Public */,
       8,    0,   57,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,

       0        // eod
};

void ChoiceReactionManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ChoiceReactionManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->runnableSelected(); break;
        case 1: _t->canSelectRunnable(); break;
        case 2: _t->start(); break;
        case 3: _t->measure(); break;
        case 4: _t->finish(); break;
        case 5: _t->selectRunnable((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->readOutput(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ChoiceReactionManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChoiceReactionManager::runnableSelected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ChoiceReactionManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChoiceReactionManager::canSelectRunnable)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ChoiceReactionManager::staticMetaObject = { {
    QMetaObject::SuperData::link<ManagerBase::staticMetaObject>(),
    qt_meta_stringdata_ChoiceReactionManager.data,
    qt_meta_data_ChoiceReactionManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ChoiceReactionManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChoiceReactionManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ChoiceReactionManager.stringdata0))
        return static_cast<void*>(this);
    return ManagerBase::qt_metacast(_clname);
}

int ChoiceReactionManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ManagerBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void ChoiceReactionManager::runnableSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ChoiceReactionManager::canSelectRunnable()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
