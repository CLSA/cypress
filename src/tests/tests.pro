QT += core gui sql widgets testlib network xml bluetooth
CONFIG += qt warn_on depend_includepath testcase
QMAKE_LFLAGS_WINDOWS += "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\""

TEMPLATE = app

SOURCES +=  \
    main.cpp \

HEADERS += \
    main.cpp \
