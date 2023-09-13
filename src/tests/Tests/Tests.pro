QT += testlib
QT += gui
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  tst_pdf_filler.cpp \
    ../../auxiliary/pdf_form_filler.cpp

HEADERS += \
    ../../auxiliary/pdf_form_filler.h
