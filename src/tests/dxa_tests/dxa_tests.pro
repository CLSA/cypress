QT += testlib gui widgets sql xml network serialport
CONFIG += qt warn_on depend_includepath testcase

# QtUsb
INCLUDEPATH += $$PWD/../../../dep/QtUsb_x86/include
LIBS += -L$$PWD/../../../dep/QtUsb_x86/lib -lQt5Usb

# OpenSSL
INCLUDEPATH += "C:/Program Files (x86)/OpenSSL-Win32/include"
LIBS += -L"C:/Program Files (x86)/OpenSSL-Win32/lib/VC/x86/MD" -llibssl -llibcrypto

# POCO
INCLUDEPATH += "C:/Program Files (x86)/Poco/include"
LIBS += -L"C:/Program Files (x86)/Poco/lib" -lPocoFoundation -lPocoUtil -lPocoCrypto -lPocoNet -lPocoNetSSL

# DCMTK
INCLUDEPATH += "C:/Program Files (x86)/DCMTK/include"
LIBS += -L"C:/Program Files (x86)/DCMTK/lib" -ldcmdata -loflog -lofstd -lws2_32 -lnetapi32 -lwsock32 -ladvapi32 -liphlpapi

# Topaz Signature Pad
# INCLUDEPATH += $$PWD/../dep/SigLib/Include/
# INCLUDEPATH += $$PWD/../dep/SigLib/SigTablt/Win32/
# DEPENDPATH += $$PWD/../dep/SigLib/SigTablt/Win32/
LIBS += -L$$PWD/../../../dep/SigLib/SigTablt/Win32/ -lzlib

TEMPLATE = app

INCLUDEPATH += C:/work/clsa/cypress/src


SOURCES +=  tst_dxa.cpp ../../managers/dxa/apex_database_helper.cpp
HEADERS +=  ../../managers/dxa/apex_database_helper.h
