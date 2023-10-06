QT += testlib
QT += gui
QT += xml
QT += network

CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  tst_pdf_filler.cpp \
    ../../auxiliary/Constants.cpp \
    ../../auxiliary/pdf_form_filler.cpp \
    ../../data/Measurement.cpp \
    ../../data/blood_pressure/measurements/blood_pressure_measurement.cpp \
    ../../data/blood_pressure/tests/blood_pressure_test.cpp \
    ../../data/ecg/measurements/ecg_measurement.cpp \
    ../../data/ecg/tests/ecg_test.cpp

HEADERS += \
    ../../auxiliary/Constants.h \
    ../../auxiliary/pdf_form_filler.h \
    ../../data/Measurement.h \
    ../../data/TestBase.h \
    ../../data/blood_pressure/measurements/blood_pressure_measurement.h \
    ../../data/blood_pressure/tests/blood_pressure_test.h \
    ../../data/ecg/measurements/ecg_measurement.h \
    ../../data/ecg/tests/ecg_test.h
