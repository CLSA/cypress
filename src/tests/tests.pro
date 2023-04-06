QT += core gui sql widgets testlib network xml bluetooth
CONFIG += qt warn_on depend_includepath testcase
QMAKE_LFLAGS_WINDOWS += "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\""

TEMPLATE = app

SOURCES +=  \
    main.cpp \
    ../managers/grip_strength/GripStrengthManager.cpp \
    ../managers/retinal_camera/RetinalCameraManager.cpp \
    ../data/retinal_camera/databasemanager.cpp \
    ../auxiliary/tracker5util.cpp \
    tst_retinal_camera.cpp \
    tst_grip_strength.cpp \

HEADERS += \
    main.cpp \
    ../managers/grip_strength/GripStrengthManager.h \
    ../managers/retinal_camera/RetinalCameraManager.h \
    ../data/retinal_camera/databasemanager.h \
    ../auxiliary/tracker5util.h \
    tst_retinal_camera.h \
    tst_grip_strength.h \
