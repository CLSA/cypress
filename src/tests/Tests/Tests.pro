QT += testlib
QT += gui
QT += xml
QT += network
QT += sql
QT += widgets

CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

# QtUsb
INCLUDEPATH += $$PWD/../../../dep/QtUsb-win32/include
LIBS += -L$$PWD/../../../dep/QtUsb-win32/lib -lQt5Usb

# OpenSSL
INCLUDEPATH += "C:/OpenSSL-Win32/include"
LIBS += -L"C:/OpenSSL-Win32/lib" -llibssl -llibcrypto

# POCO
INCLUDEPATH += $$PWD/../../../dep/poco-1.12.4-all/Foundation/include
INCLUDEPATH += $$PWD/../../../dep/poco-1.12.4-all/Util/include
INCLUDEPATH += $$PWD/../../../dep/poco-1.12.4-all/Net/include
INCLUDEPATH += $$PWD/../../../dep/poco-1.12.4-all/Crypto/include
INCLUDEPATH += $$PWD/../../../dep/poco-1.12.4-all/NetSSL_OpenSSL/include

INCLUDEPATH += $$PWD/../../../dep/poco-1.12.4-all/bin

LIBS += -L$$PWD/../../../dep/poco-1.12.4-all/lib -lPocoFoundation -lPocoUtil -lPocoCrypto -lPocoNet -lPocoNetSSL

# DCMTK
INCLUDEPATH += $$PWD/../../../dep/dcmtk-3.6.7-win32/include/
INCLUDEPATH += $$PWD/../../../dep/dcmtk-3.6.7-win32/lib/
LIBS += -L$$PWD/../../../dep/dcmtk-3.6.7-win32/lib/ -ldcmdata -loflog -lofstd -lws2_32 -lnetapi32 -lwsock32 -ladvapi32 -liphlpapi

# Topaz Signature Pad
#INCLUDEPATH += $$PWD/../../../dep/SigLib/Include/
#INCLUDEPATH += $$PWD/../../../dep/SigLib/SigTablt/Win32/
#DEPENDPATH += $$PWD/../../../dep/SigLib/SigTablt/Win32/
#LIBS += -L$$PWD/../../../dep/SigLib/SigTablt/Win32/ -lhid -lLibJpeg -llibtiff -lSigLib -lzlib -lsetupapi -lmsvcrt -luser32 -lwinmm -llegacy_stdio_definitions

SOURCES +=  tst_pdf_filler.cpp \
    ../../auxiliary/CRC8.cpp \
    ../../auxiliary/Constants.cpp \
    ../../auxiliary/Utilities.cpp \
    ../../auxiliary/file_compressor.cpp \
    ../../auxiliary/json_settings.cpp \
    ../../auxiliary/pdf_form_filler.cpp \
    ../../auxiliary/tracker5_util.cpp \
    ../../data/access_query_helper.cpp \
    ../../data/cdtt/measurements/cdtt_measurement.cpp \
    ../../data/cdtt/tests/cdtt_test.cpp \
    ../../data/choice_reaction/measurements/choice_reaction_measurement.cpp \
    ../../data/choice_reaction/tests/choice_reaction_test.cpp \
    ../../data/cimt_vivid_i_measurement.cpp \
    ../../data/cimt_vivid_i_test.cpp \
    ../../data/excel_query_helper.cpp \
    ../../data/frax/measurements/frax_measurement.cpp \
    ../../data/frax/tests/frax_test.cpp \
    ../../data/grip_strength/measurements/grip_strength_measurement.cpp \
    ../../data/grip_strength/tests/grip_strength_test.cpp \
    ../../data/hearing/measurements/hearing_measurement.cpp \
    ../../data/hearing/tests/hearing_test.cpp \
    ../../data/retinal_camera/database_manager.cpp \
    ../../data/retinal_camera/retinal_camera_measurement.cpp \
    ../../data/retinal_camera/retinal_camera_test.cpp \
    ../../data/test_base.cpp \
    ../../data/measurement.cpp \
    ../../data/blood_pressure/measurements/blood_pressure_measurement.cpp \
    ../../data/blood_pressure/tests/blood_pressure_test.cpp \
    ../../data/ecg/measurements/ecg_measurement.cpp \
    ../../data/ecg/tests/ecg_test.cpp \
    ../../data/spirometer/measurements/spirometer_measurement.cpp \
    ../../data/spirometer/tests/spirometer_test.cpp \
    ../../data/tonometer/measurements/tonometer_measurement.cpp \
    ../../data/tonometer/tests/tonometer_test.cpp \
    ../../data/weigh_scale/measurements/weight_measurement.cpp \
    ../../data/weigh_scale/tests/weigh_scale_test.cpp

HEADERS += \
    ../../auxiliary/CRC8.h \
    ../../auxiliary/Constants.h \
    ../../auxiliary/Utilities.h \
    ../../auxiliary/file_compressor.h \
    ../../auxiliary/json_settings.h \
    ../../auxiliary/pdf_form_filler.h \
    ../../auxiliary/tracker5_util.h \
    ../../data/access_query_helper.h \
    ../../data/cdtt/measurements/cdtt_measurement.h \
    ../../data/cdtt/tests/cdtt_test.h \
    ../../data/choice_reaction/measurements/choice_reaction_measurement.h \
    ../../data/choice_reaction/tests/choice_reaction_test.h \
    ../../data/cimt_vivid_i_measurement.h \
    ../../data/cimt_vivid_i_test.h \
    ../../data/excel_query_helper.h \
    ../../data/frax/measurements/frax_measurement.h \
    ../../data/frax/tests/frax_test.h \
    ../../data/grip_strength/measurements/grip_strength_measurement.h \
    ../../data/grip_strength/tests/grip_strength_test.h \
    ../../data/hearing/measurements/hearing_measurement.h \
    ../../data/hearing/tests/hearing_test.h \
    ../../data/measurement.h \
    ../../data/retinal_camera/database_manager.h \
    ../../data/retinal_camera/retinal_camera_measurement.h \
    ../../data/retinal_camera/retinal_camera_test.h \
    ../../data/test_base.h \
    ../../data/blood_pressure/measurements/blood_pressure_measurement.h \
    ../../data/blood_pressure/tests/blood_pressure_test.h \
    ../../data/ecg/measurements/ecg_measurement.h \
    ../../data/ecg/tests/ecg_test.h \
    ../../data/spirometer/measurements/spirometer_measurement.h \
    ../../data/spirometer/tests/spirometer_test.h \
    ../../data/tonometer/measurements/tonometer_measurement.h \
    ../../data/tonometer/tests/tonometer_test.h \
    ../../data/weigh_scale/measurements/weight_measurement.h \
    ../../data/weigh_scale/tests/weigh_scale_test.h
