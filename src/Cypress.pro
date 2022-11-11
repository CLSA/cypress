QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport bluetooth sql usb xml network testlib

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    RetinalCameraDialog.cpp \
    auxiliary/CommandLineParser.cpp \
    auxiliary/Constants.cpp \
    auxiliary/JsonSettings.cpp \
    auxiliary/Utilities.cpp \
<<<<<<< HEAD
    auxiliary/tracker5util.cpp \
=======
    #auxiliary/tracker5util.cpp \
>>>>>>> refs/rewritten/Integrated-POCO-library-to-use-their-HTTP-server-implementation-Added-Grip-Strength-request-handler-Added-index-html-to-possibly-be-used-as-workstation-configuration-page
    data/AccessQueryHelper.cpp \
    data/ApSpineTest.cpp \
    data/BloodPressureMeasurement.cpp \
    data/BloodPressureTest.cpp \
    data/BodyCompositionMeasurement.cpp \
    data/BodyCompositionTest.cpp \
    data/CDTTMeasurement.cpp \
    data/CDTTTest.cpp \
    data/ChoiceReactionMeasurement.cpp \
    data/ChoiceReactionTest.cpp \
    data/DXATest.cpp \
    data/ECGMeasurement.cpp \
    data/ECGTest.cpp \
    data/ExcelQueryHelper.cpp \
    data/ForearmTest.cpp \
    data/FraxMeasurement.cpp \
    data/FraxTest.cpp \
    data/HipTest.cpp \
    data/WholeBodyScanTest.cpp \
    data/grip_strength/GripStrengthMeasurement.cpp \
    data/grip_strength/GripStrengthTest.cpp \
    data/HearingMeasurement.cpp \
    data/HearingTest.cpp \
    data/Measurement.cpp \
#    data/TemperatureMeasurement.cpp \
#    data/TemperatureTest.cpp \
    data/SpirometerMeasurement.cpp \
    data/SpirometerTest.cpp \
    data/TonometerMeasurement.cpp \
    data/TonometerTest.cpp \
    data/WeightMeasurement.cpp \
    data/WeighScaleTest.cpp \
    dialogs/GripStrengthDialog.cpp \
    managers/AudiometerManager.cpp \
    managers/CDTTManager.cpp \
    managers/ChoiceReactionManager.cpp \
    managers/dxa/ApSpineScanManager.cpp \
    managers/dxa/DXAManager.cpp \
    managers/EMRPluginWriter.cpp \
    managers/BodyCompositionManager.cpp \
#    managers/BluetoothLEManager.cpp \
    managers/BloodPressureManager.cpp \
    managers/BPMCommunication.cpp \
    managers/BPMMessage.cpp \
    managers/ECGManager.cpp \
    managers/FraxManager.cpp \
    managers/ManagerBase.cpp \
    managers/SerialPortManager.cpp \
    managers/SpirometerManager.cpp \
    managers/TonometerManager.cpp \
    managers/WeighScaleManager.cpp \
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    server/server.cpp \
=======
    #server/GripStrengthRequestHandler.cpp \
    #server/InstrumentRequestHandlerFactory.cpp \
    #server/server.cpp \
>>>>>>> aeed5d7 (moved server code to its own subproject to be run independently to prevent blocking main loop, applications now launch on network request, added default request (404) handler, regex for matching request URIs to the appropriate handler)
    #tests/server/testserver.cpp \
    #tests/server/testserver.cpp \
=======
>>>>>>> 60bc3a9 (retinal camera intregration, configuring MS SQL server)
=======
=======
    managers/dxa/DXAManager.cpp \
    managers/dxa/ForearmScanManager.cpp \
    managers/dxa/HipScanManager.cpp \
    managers/dxa/IVAImagingScanManager.cpp \
    managers/dxa/WholeBodyScanManager.cpp \
    managers/grip_strength/GripStrengthManager.cpp \
    managers/grip_strength/ParadoxReader.cpp \
    managers/retinal_camera/RetinalCameraManager.cpp \
>>>>>>> d8613bb (WIP: migrating hologic data extractors)
    rest/DefaultRequestHandler.cpp \
    rest/RetinalCameraRequestHandler.cpp \
    rest/server/GripStrengthRequestHandler.cpp \
    rest/server/InstrumentRequestHandlerFactory.cpp \
    rest/server/server.cpp \
>>>>>>> cf61d48 (moved rest server into main project, server now opens dialog on request. WIP: closing dialog shouldn't exit event loop)
    widgets/BarcodeWidget.cpp \
    widgets/MeasureWidget.cpp \
    dialogs/AudiometerDialog.cpp \
    dialogs/BloodPressureDialog.cpp \
    dialogs/BodyCompositionDialog.cpp \
    dialogs/CDTTDialog.cpp \
    dialogs/ChoiceReactionDialog.cpp \
    dialogs/DialogBase.cpp \
    dialogs/DialogFactory.cpp \
    dialogs/FraxDialog.cpp \
    dialogs/ECGDialog.cpp \
    dialogs/SpirometerDialog.cpp \
    dialogs/TonometerDialog.cpp \
    dialogs/WeighScaleDialog.cpp \
    RetinalCameraDialog.cpp \
    CypressApplication.cpp \
    main.cpp 

HEADERS += \
    RetinalCameraDialog.h \
    auxiliary/CommandLineParser.h \
    auxiliary/Constants.h \
    auxiliary/JsonSettings.h \
    auxiliary/Utilities.h \
<<<<<<< HEAD
    auxiliary/tracker5util.h \
=======
    #auxiliary/tracker5util.h \
>>>>>>> refs/rewritten/Integrated-POCO-library-to-use-their-HTTP-server-implementation-Added-Grip-Strength-request-handler-Added-index-html-to-possibly-be-used-as-workstation-configuration-page
    data/AccessQueryHelper.h \
    data/ApSpineTest.h \
    data/BloodPressureMeasurement.h \
    data/BloodPressureTest.h \
    data/BodyCompositionMeasurement.h \
    data/BodyCompositionTest.h \
    data/CDTTMeasurement.h \
    data/CDTTTest.h \
    data/ChoiceReactionMeasurement.h \
    data/ChoiceReactionTest.h \
    data/DXATest.h \
    data/ECGMeasurement.h \
    data/ECGTest.h \
    data/ExcelQueryHelper.h \
    data/ForearmTest.h \
    data/FraxMeasurement.h \
    data/FraxTest.h \
    data/HipTest.h \
    data/WholeBodyScanTest.h \
    data/grip_strength/GripStrengthMeasurement.h \
    data/grip_strength/GripStrengthTest.h \
    data/HearingMeasurement.h \
    data/HearingTest.h \
    data/Measurement.h \
    data/TestBase.h \
    data/SpirometerMeasurement.h \
    data/SpirometerTest.h \
    data/TonometerMeasurement.h \
    data/TonometerTest.h \
#    data/TemperatureMeasurement.h \
#    data/TemperatureTest.h \
    data/WeightMeasurement.h \
    data/WeighScaleTest.h \
    dialogs/GripStrengthDialog.h \
    managers/AudiometerManager.h \
    managers/BloodPressureManager.h \
#    managers/BluetoothLEManager.h \
    managers/BodyCompositionManager.h \
    managers/BPMCommunication.h \
    managers/BPMMessage.h \
    managers/CDTTManager.h \
    managers/ChoiceReactionManager.h \
    managers/dxa/ApSpineScanManager.h \
    managers/dxa/DXAManager.h \
    managers/ECGManager.h \
    managers/EMRPluginWriter.h \
    managers/FraxManager.h \
    managers/dxa/ForearmScanManager.h \
    managers/dxa/HipScanManager.h \
    managers/dxa/IVAImagingScanManager.h \
    managers/dxa/WholeBodyScanManager.h \
    managers/grip_strength/GripStrengthManager.h \
    managers/ManagerBase.h \
    managers/SerialPortManager.h \
    managers/SpirometerManager.h \
    managers/TonometerManager.h \
    managers/WeighScaleManager.h \
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    server/server.h \
=======
>>>>>>> aeed5d7 (moved server code to its own subproject to be run independently to prevent blocking main loop, applications now launch on network request, added default request (404) handler, regex for matching request URIs to the appropriate handler)
=======
=======
    managers/dxa/DXAManager.h \
    managers/grip_strength/GripStrengthManager.h \
    managers/grip_strength/ParadoxReader.h \
    managers/retinal_camera/RetinalCameraManager.h \
>>>>>>> d8613bb (WIP: migrating hologic data extractors)
    rest/DefaultRequestHandler.h \
    rest/RetinalCameraRequestHandler.h \
    rest/server/GripStrengthRequestHandler.h \
    rest/server/InstrumentRequestHandlerFactory.h \
    rest/server/server.h \
>>>>>>> cf61d48 (moved rest server into main project, server now opens dialog on request. WIP: closing dialog shouldn't exit event loop)
    widgets/BarcodeWidget.h \
    widgets/MeasureWidget.h \
    dialogs/AudiometerDialog.h \
    dialogs/BloodPressureDialog.h \
    dialogs/BodyCompositionDialog.h \
    dialogs/CDTTDialog.h \
    dialogs/ChoiceReactionDialog.h \
    dialogs/DialogBase.h \
    dialogs/DialogFactory.h \
    dialogs/ECGDialog.h \
    dialogs/FraxDialog.h \
    dialogs/SpirometerDialog.h \
    dialogs/TonometerDialog.h \
    dialogs/WeighScaleDialog.h \
    RetinalCameraDialog.h \
    CypressApplication.h

#TRANSLATIONS += \
#    Cypress_en_CA.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: INCLUDEPATH += $$PWD/../poco/Foundation/include
win32: INCLUDEPATH += $$PWD/../poco/Net/include
win32: INCLUDEPATH += $$PWD/../poco/Util/include
win32: LIBS += -L$$PWD/../poco/lib64 -lPocoFoundation -lPocoNet -lPocoUtil

FORMS += \
    RetinalCameraDialog.ui \
  dialogs/gripstrengthdialog.ui \
  widgets/barcodewidget.ui \
  widgets/measurewidget.ui \
  dialogs/audiometerdialog.ui \
  dialogs/bloodpressuredialog.ui \
  dialogs/bodycompositiondialog.ui \
#  dialogs/thermometerdialog.ui \
  dialogs/runnabledialog.ui \
  dialogs/weighscaledialog.ui

RESOURCES += \

SUBDIRS += \
    rest/rest.pro

DISTFILES += \
    rest/server/pages/index.html
