QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport bluetooth sql xml network testlib

CONFIG += c++11
CONFIG += testcase
CONFIG += no_testcase_installs

SUBDIRS = tests
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = favicon.ico
SOURCES += \
    auxiliary/CommandLineParser.cpp \
    auxiliary/Constants.cpp \
    auxiliary/FIleUtils.cpp \
    auxiliary/JsonSettings.cpp \
    auxiliary/Utilities.cpp \
    auxiliary/tracker5util.cpp \
    data/AccessQueryHelper.cpp \
    data/ExcelQueryHelper.cpp \
    data/body_composition/commands.cpp \
    data/dxa/PatScanQueryHelper.cpp \
    data/dxa/ReferenceQueryHelper.cpp \
    data/dxa/measurements/ApSpineMeasurement.cpp \
    data/dxa/measurements/DXAMeasurement.cpp \
    data/dxa/measurements/ForearmMeasurement.cpp \
    data/dxa/measurements/HipMeasurement.cpp \
    data/dxa/measurements/IVAImagingMeasurement.cpp \
    data/dxa/measurements/WholeBodyScanMeasurement.cpp \
    data/dxa/smbfilecopier.cpp \
    data/dxa/tests/ApSpineTest.cpp \
    data/dxa/tests/DXATest.cpp \
    data/dxa/tests/ForearmTest.cpp \
    data/dxa/tests/HipTest.cpp \
    data/dxa/tests/IVAImagingTest.cpp \
    data/dxa/tests/WholeBodyScanTest.cpp \
    data/Measurement.cpp \
#    data/TemperatureMeasurement.cpp \
#    data/TemperatureTest.cpp \
    data/grip_strength/measurements/GripStrengthMeasurement.cpp \
    data/grip_strength/tests/GripStrengthTest.cpp \
    data/retinal_camera/databasemanager.cpp \
    dialogs/DXADialog.cpp \
    dialogs/GripStrengthDialog.cpp \
#    managers/BluetoothLEManager.cpp \
    data/Measurement.cpp \
    data/MeasurementBase.cpp \
    data/blood_pressure/measurements/BloodPressureMeasurement.cpp \
    data/blood_pressure/tests/BloodPressureTest.cpp \
    data/body_composition/measurements/BodyCompositionMeasurement.cpp \
    data/body_composition/tests/BodyCompositionTest.cpp \
    data/cdtt/measurements/CDTTMeasurement.cpp \
    data/cdtt/tests/CDTTTest.cpp \
    data/choice_reaction/measurements/ChoiceReactionMeasurement.cpp \
    data/choice_reaction/tests/ChoiceReactionTest.cpp \
    data/dxa/PatScanQueryHelper.cpp \
    data/dxa/ReferenceQueryHelper.cpp \
    data/dxa/tests/WholeBodyScanTest.cpp \
    data/ecg/measurements/ECGMeasurement.cpp \
    data/ecg/tests/ECGTest.cpp \
    data/frax/measurements/FraxMeasurement.cpp \
    data/frax/tests/FraxTest.cpp \
    data/grip_strength/measurements/GripStrengthMeasurement.cpp \
    data/grip_strength/tests/GripStrengthTest.cpp \
    data/hearing/measurements/HearingMeasurement.cpp \
    data/hearing/tests/HearingTest.cpp \
    data/spirometer/measurements/SpirometerMeasurement.cpp \
    data/spirometer/tests/SpirometerTest.cpp \
    data/temperature/measurements/TemperatureMeasurement.cpp \
    data/temperature/tests/TemperatureTest.cpp \
    data/tonometer/measurements/TonometerMeasurement.cpp \
    data/tonometer/tests/TonometerTest.cpp \
    data/weigh_scale/measurements/WeightMeasurement.cpp \
    data/weigh_scale/tests/WeighScaleTest.cpp \
    dialogs/DXADialog.cpp \
    dialogs/GripStrengthDialog.cpp \
    dialogs/RetinalCameraDialog.cpp \
    dialogs/ecgdialog.cpp \
    dialogs/signaturepaddialog.cpp \
    managers/ManagerBase.cpp \
    managers/SettingsManager.cpp \
    managers/audiometer/AudiometerManager.cpp \
    managers/blood_pressure/BPMCommunication.cpp \
    managers/blood_pressure/BPMMessage.cpp \
    managers/blood_pressure/BloodPressureManager.cpp \
    managers/bluetooth/BluetoothLEManager.cpp \
    managers/body_composition/BodyCompositionManager.cpp \
    managers/cdtt/CDTTManager.cpp \
    managers/choice_reaction/ChoiceReactionManager.cpp \
    managers/dxa/DXAManager.cpp \
    managers/dxa/dicom/DicomSCP.cpp \
    managers/dxa/dicom/LogWorker.cpp \
    managers/grip_strength/GripStrengthManager.cpp \
    managers/grip_strength/ParadoxReader.cpp \
    managers/retinal_camera/RetinalCameraManager.cpp \
    managers/ecg/ECGManager.cpp \
    managers/emr/EMRPluginWriter.cpp \
    managers/frax/FraxManager.cpp \
    managers/grip_strength/GripStrengthManager.cpp \
    managers/grip_strength/ParadoxReader.cpp \
    managers/retinal_camera/RetinalCameraManager.cpp \
    managers/serial_port/SerialPortManager.cpp \
    managers/signature_pad/signaturepadcommunication.cpp \
    managers/signature_pad/signaturepadmanager.cpp \
    managers/spirometer/SpirometerManager.cpp \
    managers/tonometer/TonometerManager.cpp \
    managers/ultrasound/CarotidIntimaManager.cpp \
    managers/weigh_scale/WeighScaleManager.cpp \
    server/DefaultRequestHandler.cpp \
    server/InstrumentRequestHandlerFactory.cpp \
    server/Server.cpp \
    server/handlers/BodyCompositionRequestHandler.cpp \
    server/handlers/CDTTRequestHandler.cpp \
    server/handlers/ChoiceReactionRequestHandler.cpp \
    server/handlers/DxaDualHipBoneDensityHandler.cpp \
    server/handlers/DxaForearmBoneDensityHandler.cpp \
    server/handlers/DxaLateralBoneDensityHandler.cpp \
    server/handlers/DxaRequestHandler.cpp \
    server/handlers/DxaSpineBoneDensityHandler.cpp \
    server/handlers/DxaWholeBodyBoneDensityHandler.cpp \
    server/handlers/ECGRequestHandler.cpp \
    server/handlers/EMRRequestHandler.cpp \
    server/handlers/FraxRequestHandler.cpp \
    server/handlers/GripStrengthRequestHandler.cpp \
    server/handlers/RetinalCameraRequestHandler.cpp \
    server/handlers/SpirometerRequestHandler.cpp \
    server/handlers/TonometerRequestHandler.cpp \
    server/handlers/UltrasoundRequestHandler.cpp \
    server/handlers/WeighScaleRequestHandler.cpp \
    server/handlers/audiometerrequesthandler.cpp \
    server/handlers/audiometerstatusrequesthandler.cpp \
    server/handlers/bloodpressurerequesthandler.cpp \
    server/handlers/bloodpressurestatusrequesthandler.cpp \
    server/handlers/bodycompositionstatusrequesthandler.cpp \
    server/handlers/cdttstatusrequesthandler.cpp \
    server/handlers/choicereactionstatusrequesthandler.cpp \
    server/handlers/cypressstatusrequesthandler.cpp \
    server/handlers/dxaforearmstatusrequesthandler.cpp \
    server/handlers/dxahipstatusrequesthandler.cpp \
    server/handlers/dxaivastatusrequesthandler.cpp \
    server/handlers/dxaspinestatusrequesthandler.cpp \
    server/handlers/dxawholebodystatusrequesthandler.cpp \
    server/handlers/ecgstatusrequesthandler.cpp \
    server/handlers/fraxstatusrequesthandler.cpp \
    server/handlers/gripstrengthstatusrequesthandler.cpp \
    server/handlers/retinalcamerastatusrequesthandler.cpp \
    server/handlers/signaturepadrequesthandler.cpp \
    server/handlers/signaturepadstatusrequesthandler.cpp \
    server/handlers/spirometerstatusrequesthandler.cpp \
    server/handlers/tonometerstatusrequesthandler.cpp \
    server/handlers/ultrasoundstatusrequesthandler.cpp \
    server/handlers/weighscalestatusrequesthandler.cpp \
    server/utils.cpp \
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
    dialogs/SpirometerDialog.cpp \
    dialogs/TonometerDialog.cpp \
    dialogs/WeighScaleDialog.cpp \
    CypressApplication.cpp \
    main.cpp

HEADERS += \
    auxiliary/CommandLineParser.h \
    auxiliary/Constants.h \
    auxiliary/FileUtils.h \
    auxiliary/JsonSettings.h \
    auxiliary/Utilities.h \
    auxiliary/tracker5util.h \
    data/AccessQueryHelper.h \
    data/ExcelQueryHelper.h \
    data/MeasurementBase.h \
    data/blood_pressure/measurements/BloodPressureMeasurement.h \
    data/blood_pressure/tests/BloodPressureTest.h \
    data/body_composition/commands.h \
    data/body_composition/measurements/BodyCompositionMeasurement.h \
    data/body_composition/tests/BodyCompositionTest.h \
    data/cdtt/measurements/CDTTMeasurement.h \
    data/cdtt/tests/CDTTTest.h \
    data/choice_reaction/measurements/ChoiceReactionMeasurement.h \
    data/choice_reaction/tests/ChoiceReactionTest.h \
    data/dxa/PatScanQueryHelper.h \
    data/dxa/ReferenceQueryHelper.h \
    data/dxa/measurements/ApSpineMeasurement.h \
    data/dxa/measurements/DXAMeasurement.h \
    data/dxa/measurements/ForearmMeasurement.h \
    data/dxa/measurements/HipMeasurement.h \
    data/dxa/measurements/IVAImagingMeasurement.h \
    data/dxa/measurements/WholeBodyScanMeasurement.h \
    data/dxa/smbfilecopier.h \
    data/dxa/tests/ApSpineTest.h \
    data/dxa/tests/ForearmTest.h \
    data/dxa/tests/HipTest.h \
    data/dxa/tests/IVAImagingTest.h \
    data/dxa/tests/WholeBodyScanTest.h \
    data/Measurement.h \
    data/TestBase.h \
#    data/TemperatureMeasurement.h \
#    data/TemperatureTest.h \
    data/dxa/tests/dxatest.h \
    data/grip_strength/measurements/GripStrengthMeasurement.h \
    data/grip_strength/tests/GripStrengthTest.h \
    data/retinal_camera/databasemanager.h \
    dialogs/DXADialog.h \
    dialogs/GripStrengthDialog.h \
    data/ecg/measurements/ECGMeasurement.h \
    data/ecg/tests/ECGTest.h \
    data/frax/measurements/FraxMeasurement.h \
    data/frax/tests/FraxTest.h \
    data/grip_strength/measurements/GripStrengthMeasurement.h \
    data/grip_strength/tests/GripStrengthTest.h \
    data/hearing/measurements/HearingMeasurement.h \
    data/hearing/tests/HearingTest.h \
    data/spirometer/measurements/SpirometerMeasurement.h \
    data/spirometer/tests/SpirometerTest.h \
    data/temperature/measurements/TemperatureMeasurement.h \
    data/temperature/tests/TemperatureTest.h \
    data/tonometer/measurements/TonometerMeasurement.h \
    data/tonometer/tests/TonometerTest.h \
    data/weigh_scale/measurements/WeightMeasurement.h \
    data/weigh_scale/tests/WeighScaleTest.h \
    dialogs/DXADialog.h \
    dialogs/GripStrengthDialog.h \
    dialogs/RetinalCameraDialog.h \
    dialogs/ecgdialog.h \
    dialogs/signaturepaddialog.h \
    managers/SettingsManager.h \
    managers/audiometer/AudiometerManager.h \
    managers/blood_pressure/BPMCommunication.h \
    managers/blood_pressure/BPMMessage.h \
    managers/blood_pressure/BloodPressureManager.h \
    managers/bluetooth/BluetoothLEManager.h \
    managers/body_composition/BodyCompositionManager.h \
    managers/cdtt/CDTTManager.h \
    managers/choice_reaction/ChoiceReactionManager.h \
    managers/dxa/DXAManager.h \
    managers/dxa/dicom/DicomSCP.h \
    managers/dxa/dicom/LogWorker.h \
    managers/grip_strength/GripStrengthManager.h \
    managers/ManagerBase.h \
    managers/ecg/ECGManager.h \
    managers/emr/EMRPluginWriter.h \
    managers/frax/FraxManager.h \
    managers/grip_strength/GripStrengthManager.h \
    managers/ManagerBase.h \
    managers/dxa/DXAManager.h \
    managers/grip_strength/GripStrengthManager.h \
    managers/grip_strength/ParadoxReader.h \
    managers/retinal_camera/RetinalCameraManager.h \
    managers/serial_port/SerialPortManager.h \
    managers/signature_pad/signaturepadcommunication.h \
    managers/signature_pad/signaturepadmanager.h \
    managers/spirometer/SpirometerManager.h \
    managers/tonometer/TonometerManager.h \
    managers/ultrasound/CarotidIntimaManager.h \
    managers/weigh_scale/WeighScaleManager.h \
    server/DefaultRequestHandler.h \
    server/InstrumentRequestHandlerFactory.h \
    server/Server.h \
    server/handlers/BodyCompositionRequestHandler.h \
    server/handlers/CDTTRequestHandler.h \
    server/handlers/ChoiceReactionRequestHandler.h \
    server/handlers/DxaDualHipBoneDensityHandler.h \
    server/handlers/DxaForearmBoneDensityHandler.h \
    server/handlers/DxaLateralBoneDensityHandler.h \
    server/handlers/DxaRequestHandler.h \
    server/handlers/DxaSpineBoneDensityHandler.h \
    server/handlers/DxaWholeBodyBoneDensityHandler.h \
    server/handlers/ECGRequestHandler.h \
    server/handlers/EMRRequestHandler.h \
    server/handlers/FraxRequestHandler.h \
    server/handlers/GripStrengthRequestHandler.h \
    server/handlers/RetinalCameraRequestHandler.h \
    server/handlers/SpirometerRequestHandler.h \
    server/handlers/TonometerRequestHandler.h \
    server/handlers/UltrasoundRequestHandler.h \
    server/handlers/WeighScaleRequestHandler.h \
    server/handlers/audiometerrequesthandler.h \
    server/handlers/audiometerstatusrequesthandler.h \
    server/handlers/bloodpressurerequesthandler.h \
    server/handlers/bloodpressurestatusrequesthandler.h \
    server/handlers/bodycompositionstatusrequesthandler.h \
    server/handlers/cdttstatusrequesthandler.h \
    server/handlers/choicereactionstatusrequesthandler.h \
    server/handlers/cypressstatusrequesthandler.h \
    server/handlers/dxaforearmstatusrequesthandler.h \
    server/handlers/dxahipstatusrequesthandler.h \
    server/handlers/dxaivastatusrequesthandler.h \
    server/handlers/dxaspinestatusrequesthandler.h \
    server/handlers/dxawholebodystatusrequesthandler.h \
    server/handlers/ecgstatusrequesthandler.h \
    server/handlers/fraxstatusrequesthandler.h \
    server/handlers/gripstrengthstatusrequesthandler.h \
    server/handlers/retinalcamerastatusrequesthandler.h \
    server/handlers/signaturepadrequesthandler.h \
    server/handlers/signaturepadstatusrequesthandler.h \
    server/handlers/spirometerstatusrequesthandler.h \
    server/handlers/tonometerstatusrequesthandler.h \
    server/handlers/ultrasoundstatusrequesthandler.h \
    server/handlers/weighscalestatusrequesthandler.h \
    server/utils.h \
    widgets/BarcodeWidget.h \
    widgets/MeasureWidget.h \
    dialogs/AudiometerDialog.h \
    dialogs/BloodPressureDialog.h \
    dialogs/BodyCompositionDialog.h \
    dialogs/CDTTDialog.h \
    dialogs/ChoiceReactionDialog.h \
    dialogs/DialogBase.h \
    dialogs/DialogFactory.h \
    dialogs/FraxDialog.h \
    dialogs/SpirometerDialog.h \
    dialogs/TonometerDialog.h \
    dialogs/WeighScaleDialog.h \
    CypressApplication.h

#TRANSLATIONS += \
#    Cypress_en_CA.ts
CONFIG += lrelease
CONFIG += embed_translations

QMAKE_LFLAGS_WINDOWS += "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\""

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


# POCO
INCLUDEPATH += $$PWD/../dep/poco-1.12.4-all/Foundation/include
INCLUDEPATH += $$PWD/../dep/poco-1.12.4-all/Util/include
INCLUDEPATH += $$PWD/../dep/poco-1.12.4-all/Net/include
INCLUDEPATH += $$PWD/../dep/poco-1.12.4-all/Crypto/include
INCLUDEPATH += $$PWD/../dep/poco-1.12.4-all/NetSSL_OpenSSL/include
LIBS += -L$$PWD/../dep/poco-1.12.4-all/lib64 -lPocoFoundation -lPocoUtil -lPocoCrypto -lPocoNet -lPocoNetSSL

# OpenSSL
INCLUDEPATH += "C:/Program Files/OpenSSL-Win64/include"
LIBS += -L"C:/Program Files/OpenSSL-Win64/lib" -lopenssl -llibcrypto

# DCMTK
INCLUDEPATH += $$PWD/../dep/dcmtk-3.6.7-win64-release/include/
INCLUDEPATH += $$PWD/../dep/dcmtk-3.6.7-win64-release/lib/
INCLUDEPATH += $$PWD/../dep/dcmtk-3.6.7-win64-support-MD-iconv-msvc-16.9/zlib-1.2.12/include

DEPENDPATH += $$PWD/../dep/dcmtk-3.6.7-win64-release/lib/
LIBS += -L$$PWD/../dep/dcmtk-3.6.7-win64-release/lib/ -ldcmdata -loflog -lofstd -lws2_32 -lnetapi32 -lwsock32 -ladvapi32 -liphlpapi

# Topaz Signature Pad
INCLUDEPATH += $$PWD/../dep/SigLib/Include/
INCLUDEPATH += $$PWD/../dep/SigLib/SigTablt/x64/
DEPENDPATH += $$PWD/../dep/SigLib/SigTablt/x64/
LIBS += -L$$PWD/../dep/SigLib/SigTablt/x64/ -lhid -lLibJpeg -llibtiff -lSigLib -lzlib -lsetupapi -lmsvcrt -luser32 -lwinmm -llegacy_stdio_definitions

#win32: INCLUDEPATH += $$PWD/../deps/poco_x86-windows/include
#win32: INCLUDEPATH += $$PWD/../deps/poco_x86-windows/lib
#win32: DEPENDPATH += $$PWD/../deps/poco_x86-windows/lib
#win32: LIBS += -L$$PWD/../deps/poco_x86-windows/lib -lPocoNet -lPocoFoundation -lPocoUtil
#
#win32: INCLUDEPATH += $$PWD/../deps/dcmtk-3.6.7-win32-release/include
#win32: INCLUDEPATH += $$PWD/../deps/dcmtk-3.6.7-win32-release/lib
#win32: DEPENDPATH += $$PWD/../deps/dcmtk-3.6.7-win32-release/lib
#win32: LIBS += -L$$PWD/../deps/dcmtk-3.6.7-win32-release/lib/ -ldcmdata
#
#win32: INCLUDEPATH += $$PWD/../deps/SigPad/Include
#win32: INCLUDEPATH += $$PWD/../deps/SigPad/SigTablt/Win32
#win32: DEPENDPATH += $$PWD/../deps/SigPad/SigTablt/Win32
#win32: LIBS += -L$$PWD/../deps/SigPad/SigTablt/Win32/ -lhid -lLibJpeg -llibtiff -lSigLib -lzlib


FORMS += \
  dialogs/RetinalCameraDialog.ui \
  dialogs/cdttdialog.ui \
  dialogs/choicereactiondialog.ui \
  dialogs/dxadialog.ui \
  dialogs/ecgdialog.ui \
  dialogs/fraxdialog.ui \
  dialogs/gripstrengthdialog.ui \
  dialogs/signaturepaddialog.ui \
  dialogs/spirometerdialog.ui \
  dialogs/tonometerdialog.ui \
  widgets/barcodewidget.ui \
  widgets/measurewidget.ui \
  dialogs/audiometerdialog.ui \
  dialogs/bloodpressuredialog.ui \
  dialogs/bodycompositiondialog.ui \
  dialogs/runnabledialog.ui \
  dialogs/weighscaledialog.ui

RESOURCES += \

DISTFILES += \
    ../README.md \
    clear.tif \
    favicon.ico \
    ok.tif \
    please.tif \
    server/pages/index.html \
    sign.tif \
    thankyou.tif
