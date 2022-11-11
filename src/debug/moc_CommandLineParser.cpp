/****************************************************************************
** Meta object code from reading C++ file 'CommandLineParser.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../auxiliary/CommandLineParser.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CommandLineParser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CommandLineParser_t {
    QByteArrayData data[11];
    char stringdata0[187];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CommandLineParser_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CommandLineParser_t qt_meta_stringdata_CommandLineParser = {
    {
QT_MOC_LITERAL(0, 0, 17), // "CommandLineParser"
QT_MOC_LITERAL(1, 18, 11), // "ParseResult"
QT_MOC_LITERAL(2, 30, 7), // "parseOk"
QT_MOC_LITERAL(3, 38, 10), // "parseError"
QT_MOC_LITERAL(4, 49, 15), // "parseMissingArg"
QT_MOC_LITERAL(5, 65, 17), // "parseRunModeError"
QT_MOC_LITERAL(6, 83, 19), // "parseInputFileError"
QT_MOC_LITERAL(7, 103, 20), // "parseOutputPathError"
QT_MOC_LITERAL(8, 124, 21), // "parseMeasureTypeError"
QT_MOC_LITERAL(9, 146, 21), // "parseVersionRequested"
QT_MOC_LITERAL(10, 168, 18) // "parseHelpRequested"

    },
    "CommandLineParser\0ParseResult\0parseOk\0"
    "parseError\0parseMissingArg\0parseRunModeError\0"
    "parseInputFileError\0parseOutputPathError\0"
    "parseMeasureTypeError\0parseVersionRequested\0"
    "parseHelpRequested"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CommandLineParser[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, alias, flags, count, data
       1,    1, 0x0,    9,   19,

 // enum data: key, value
       2, uint(CommandLineParser::parseOk),
       3, uint(CommandLineParser::parseError),
       4, uint(CommandLineParser::parseMissingArg),
       5, uint(CommandLineParser::parseRunModeError),
       6, uint(CommandLineParser::parseInputFileError),
       7, uint(CommandLineParser::parseOutputPathError),
       8, uint(CommandLineParser::parseMeasureTypeError),
       9, uint(CommandLineParser::parseVersionRequested),
      10, uint(CommandLineParser::parseHelpRequested),

       0        // eod
};

void CommandLineParser::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject CommandLineParser::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CommandLineParser.data,
    qt_meta_data_CommandLineParser,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CommandLineParser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CommandLineParser::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CommandLineParser.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CommandLineParser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
