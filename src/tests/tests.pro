QT += core gui sql widgets testlib network xml bluetooth
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  \
    ../managers/grip_strength/GripStrengthManager.cpp \
    main.cpp \
    ../auxiliary/tracker5util.cpp \
    tst_dicomdirectorywatcher.cpp \
    tst_grip_strength.cpp \

HEADERS += \
    ../managers/grip_strength/GripStrengthManager.h \
    ../auxiliary/tracker5util.h \
    tst_dicomdirectorywatcher.h \
    tst_grip_strength.h \
