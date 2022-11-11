QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        DefaultRequestHandler.cpp \
        main.cpp \
        server/GripStrengthRequestHandler.cpp \
        server/InstrumentRequestHandlerFactory.cpp \
        server/server.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


unix: INCLUDEPATH += /usr/local/include
unix: LIBS += -L/usr/local/lib -lPocoFoundation -lPocoNet -lPocoUtil

win32: INCLUDEPATH += $$PWD/poco/Foundation/include
win32: INCLUDEPATH += $$PWD/poco/Net/include
win32: INCLUDEPATH += $$PWD/poco/Util/include
win32: LIBS += -L$$PWD/poco/lib64 -lPocoFoundation -lPocoNet -lPocoUtil

DISTFILES += \
    server/pages/index.html

HEADERS += \
    DefaultRequestHandler.h \
    server/GripStrengthRequestHandler.h \
    server/InstrumentRequestHandlerFactory.h \
    server/server.h
