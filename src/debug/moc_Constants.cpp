/****************************************************************************
** Meta object code from reading C++ file 'Constants.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../auxiliary/Constants.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Constants.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Constants_t {
    QByteArrayData data[28];
    char stringdata0[378];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Constants_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Constants_t qt_meta_stringdata_Constants = {
    {
QT_MOC_LITERAL(0, 0, 9), // "Constants"
QT_MOC_LITERAL(1, 10, 7), // "RunMode"
QT_MOC_LITERAL(2, 18, 11), // "modeUnknown"
QT_MOC_LITERAL(3, 30, 11), // "modeDefault"
QT_MOC_LITERAL(4, 42, 8), // "modeLive"
QT_MOC_LITERAL(5, 51, 12), // "modeSimulate"
QT_MOC_LITERAL(6, 64, 12), // "modeHeadless"
QT_MOC_LITERAL(7, 77, 11), // "MeasureType"
QT_MOC_LITERAL(8, 89, 11), // "typeUnknown"
QT_MOC_LITERAL(9, 101, 14), // "typeAudiometer"
QT_MOC_LITERAL(10, 116, 18), // "typeBlood_Pressure"
QT_MOC_LITERAL(11, 135, 20), // "typeBody_Composition"
QT_MOC_LITERAL(12, 156, 8), // "typeCDTT"
QT_MOC_LITERAL(13, 165, 19), // "typeChoice_Reaction"
QT_MOC_LITERAL(14, 185, 7), // "typeECG"
QT_MOC_LITERAL(15, 193, 8), // "typeFrax"
QT_MOC_LITERAL(16, 202, 17), // "typeGrip_Strength"
QT_MOC_LITERAL(17, 220, 18), // "typeRetinal_Camera"
QT_MOC_LITERAL(18, 239, 14), // "typeSpirometer"
QT_MOC_LITERAL(19, 254, 15), // "typeThermometer"
QT_MOC_LITERAL(20, 270, 13), // "typeTonometer"
QT_MOC_LITERAL(21, 284, 15), // "typeWeigh_Scale"
QT_MOC_LITERAL(22, 300, 14), // "typeUltrasound"
QT_MOC_LITERAL(23, 315, 8), // "typeDexa"
QT_MOC_LITERAL(24, 324, 11), // "UnitsSystem"
QT_MOC_LITERAL(25, 336, 13), // "systemUnknown"
QT_MOC_LITERAL(26, 350, 12), // "systemMetric"
QT_MOC_LITERAL(27, 363, 14) // "systemImperial"

    },
    "Constants\0RunMode\0modeUnknown\0modeDefault\0"
    "modeLive\0modeSimulate\0modeHeadless\0"
    "MeasureType\0typeUnknown\0typeAudiometer\0"
    "typeBlood_Pressure\0typeBody_Composition\0"
    "typeCDTT\0typeChoice_Reaction\0typeECG\0"
    "typeFrax\0typeGrip_Strength\0"
    "typeRetinal_Camera\0typeSpirometer\0"
    "typeThermometer\0typeTonometer\0"
    "typeWeigh_Scale\0typeUltrasound\0typeDexa\0"
    "UnitsSystem\0systemUnknown\0systemMetric\0"
    "systemImperial"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Constants[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       3,   14, // enums/sets
       0,    0, // constructors
       4,       // flags
       0,       // signalCount

 // enums: name, alias, flags, count, data
       1,    1, 0x0,    5,   29,
       7,    7, 0x0,   16,   39,
      24,   24, 0x0,    3,   71,

 // enum data: key, value
       2, uint(Constants::modeUnknown),
       3, uint(Constants::modeDefault),
       4, uint(Constants::modeLive),
       5, uint(Constants::modeSimulate),
       6, uint(Constants::modeHeadless),
       8, uint(Constants::typeUnknown),
       9, uint(Constants::typeAudiometer),
      10, uint(Constants::typeBlood_Pressure),
      11, uint(Constants::typeBody_Composition),
      12, uint(Constants::typeCDTT),
      13, uint(Constants::typeChoice_Reaction),
      14, uint(Constants::typeECG),
      15, uint(Constants::typeFrax),
      16, uint(Constants::typeGrip_Strength),
      17, uint(Constants::typeRetinal_Camera),
      18, uint(Constants::typeSpirometer),
      19, uint(Constants::typeThermometer),
      20, uint(Constants::typeTonometer),
      21, uint(Constants::typeWeigh_Scale),
      22, uint(Constants::typeUltrasound),
      23, uint(Constants::typeDexa),
      25, uint(Constants::systemUnknown),
      26, uint(Constants::systemMetric),
      27, uint(Constants::systemImperial),

       0        // eod
};

QT_INIT_METAOBJECT const QMetaObject Constants::staticMetaObject = { {
    nullptr,
    qt_meta_stringdata_Constants.data,
    qt_meta_data_Constants,
    nullptr,
    nullptr,
    nullptr
} };

QT_WARNING_POP
QT_END_MOC_NAMESPACE
