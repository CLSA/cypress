QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport bluetooth sql xml network testlib

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TRANSLATIONS += \
    Cypress_en_CA.ts

CONFIG += c++11 testcase no_testcase_installs lrelease embed_translations
SUBDIRS = tests

QMAKE_LFLAGS += /NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:libcmtd.lib /NODEFAULTLIB:msvcrtd.lib
QMAKE_LFLAGS_WINDOWS += "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\""

win32 {
    # QtUsb
    INCLUDEPATH += $$PWD/../dep/QtUsb_x86/include
    LIBS += -L$$PWD/../dep/QtUsb_x86/lib -lQt5Usb

    # OpenSSL
    INCLUDEPATH += "C:/Program Files (x86)/OpenSSL-Win32/include"
    LIBS += -L"C:/Program Files (x86)/OpenSSL-Win32/lib" -llibssl -llibcrypto

    # POCO
    INCLUDEPATH += $$PWD/../dep/poco-1.12.4-all/Foundation/include
    INCLUDEPATH += $$PWD/../dep/poco-1.12.4-all/Util/include
    INCLUDEPATH += $$PWD/../dep/poco-1.12.4-all/Net/include
    INCLUDEPATH += $$PWD/../dep/poco-1.12.4-all/Crypto/include
    INCLUDEPATH += $$PWD/../dep/poco-1.12.4-all/NetSSL_OpenSSL/include
    LIBS += -L$$PWD/../dep/poco-1.12.4-all/lib -lPocoFoundation -lPocoUtil -lPocoCrypto -lPocoNet -lPocoNetSSL

    # DCMTK
    INCLUDEPATH += $$PWD/../dep/dcmtk-3.6.7-win32-install/include/
    INCLUDEPATH += $$PWD/../dep/dcmtk-3.6.7-win32-install/lib/
    LIBS += -L$$PWD/../dep/dcmtk-3.6.7-win32-install/lib/ -ldcmdata -loflog -lofstd -lws2_32 -lnetapi32 -lwsock32 -ladvapi32 -liphlpapi

    # Topaz Signature Pad
    INCLUDEPATH += $$PWD/../dep/SigLib/Include/
    INCLUDEPATH += $$PWD/../dep/SigLib/SigTablt/Win32/
    DEPENDPATH += $$PWD/../dep/SigLib/SigTablt/Win32/
    LIBS += -L$$PWD/../dep/SigLib/SigTablt/Win32/ -lhid -lLibJpeg -llibtiff -lSigLib -lzlib -lsetupapi -lmsvcrt -luser32 -lwinmm -llegacy_stdio_definitions
}


RC_ICONS = favicon.ico
SOURCES += \
    auxiliary/CommandLineParser.cpp \
    auxiliary/Constants.cpp \
    auxiliary/FIleUtils.cpp \
    auxiliary/JsonSettings.cpp \
    auxiliary/Utilities.cpp \
    auxiliary/filecompressor.cpp \
    auxiliary/tracker5util.cpp \
    data/AccessQueryHelper.cpp \
    data/ExcelQueryHelper.cpp \
    data/body_composition/commands.cpp \
    data/dxa/measurements/ApSpineMeasurement.cpp \
    data/dxa/measurements/DXAMeasurement.cpp \
    data/dxa/measurements/ForearmMeasurement.cpp \
    data/dxa/measurements/HipMeasurement.cpp \
    data/dxa/measurements/IVAImagingMeasurement.cpp \
    data/dxa/measurements/WholeBodyScanMeasurement.cpp \
    #data/dxa/smbfilecopier.cpp \
    data/dxa/tests/ApSpineTest.cpp \
    #data/dxa/tests/DXATest.cpp \
    data/dxa/tests/ForearmTest.cpp \
    data/dxa/tests/HipTest.cpp \
    data/dxa/tests/IVAImagingTest.cpp \
    data/dxa/tests/WholeBodyScanTest.cpp \
    data/grip_strength/tests/GripStrengthTest.cpp \
    data/retinal_camera/databasemanager.cpp \
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
    data/ecg/measurements/ECGMeasurement.cpp \
    data/ecg/tests/ECGTest.cpp \
    data/frax/measurements/FraxMeasurement.cpp \
    data/frax/tests/FraxTest.cpp \
    data/grip_strength/measurements/GripStrengthMeasurement.cpp \
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
    dicom/dcmrecv.cpp \
    dicom/dicomdirectorywatcher.cpp \
    logging/debugdialog.cpp \
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
    #managers/dxa/dicom/LogWorker.cpp \
    managers/grip_strength/GripStrengthManager.cpp \
    managers/grip_strength/ParadoxReader.cpp \
    managers/ecg/ECGManager.cpp \
    managers/emr/EMRPluginWriter.cpp \
    managers/frax/FraxManager.cpp \
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
    server/defaultdeletesessionrequesthandler.cpp \
    server/handlers/body_composition_request_handler.cpp \
    server/handlers/audiometer_request_handler.cpp \
    server/handlers/audiometer_status_request_handler.cpp \
    server/handlers/blood_pressure_request_handler.cpp \
    server/handlers/blood_pressure_status_request_handler.cpp \
    server/handlers/body_composition_status_request_handler.cpp \
    server/handlers/cdtt_request_handler.cpp \
    server/handlers/cdtt_status_request_handler.cpp \
    server/handlers/choice_reaction_request_handler.cpp \
    server/handlers/choice_reaction_status_request_handler.cpp \
    server/handlers/cypress_status_request_handler.cpp \
    server/handlers/dxa_request_handler.cpp \
    server/handlers/dxa_status_request_handler.cpp \
    server/handlers/ecg_request_handler.cpp \
    server/handlers/ecg_status_request_handler.cpp \
    server/handlers/frax_request_handler.cpp \
    server/handlers/frax_status_request_handler.cpp \
    server/handlers/grip_strength_request_handler.cpp \
    server/handlers/grip_strength_status_request_handler.cpp \
    server/handlers/retinal_camera_request_handler.cpp \
    server/handlers/retinal_camera_status_request_handler.cpp \
    server/handlers/signature_pad_request_handler.cpp \
    server/handlers/signature_pad_status_request_handler.cpp \
    server/handlers/spirometer_request_handler.cpp \
    server/handlers/spirometer_status_request_handler.cpp \
    server/handlers/tonometer_request_handler.cpp \
    server/handlers/tonometer_status_request_handler.cpp \
    server/handlers/ultrasound_request_handler.cpp \
    server/handlers/ultrasound_status_request_handler.cpp \
    server/handlers/weigh_scale_request_handler.cpp \
    server/handlers/weigh_scale_status_request_handler.cpp \
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
    main.cpp \
    widgets/testinfowidget.cpp

HEADERS += \
    auxiliary/CommandLineParser.h \
    auxiliary/Constants.h \
    auxiliary/FileUtils.h \
    auxiliary/JsonSettings.h \
    auxiliary/Utilities.h \
    auxiliary/filecompressor.h \
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
    #data/dxa/smbfilecopier.h \
    data/dxa/tests/ApSpineTest.h \
    data/dxa/tests/ForearmTest.h \
    data/dxa/tests/HipTest.h \
    data/dxa/tests/IVAImagingTest.h \
    data/dxa/tests/WholeBodyScanTest.h \
    data/Measurement.h \
    data/TestBase.h \
    #data/TemperatureMeasurement.h \
    #data/TemperatureTest.h \
    data/dxa/tests/dxatest.h \
    data/grip_strength/tests/GripStrengthTest.h \
    data/retinal_camera/databasemanager.h \
    dialogs/DXADialog.h \
    data/ecg/measurements/ECGMeasurement.h \
    data/ecg/tests/ECGTest.h \
    data/frax/measurements/FraxMeasurement.h \
    data/frax/tests/FraxTest.h \
    data/grip_strength/measurements/GripStrengthMeasurement.h \
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
    dialogs/GripStrengthDialog.h \
    dialogs/RetinalCameraDialog.h \
    dialogs/ecgdialog.h \
    dialogs/signaturepaddialog.h \
    dicom/dcmrecv.h \
    dicom/dicomdirectorywatcher.h \
    logging/debugdialog.h \
    managers/SettingsManager.h \
    managers/audiometer/AudiometerManager.h \
    managers/blood_pressure/BPMCommunication.h \
    managers/blood_pressure/BPMMessage.h \
    managers/blood_pressure/BloodPressureManager.h \
    managers/bluetooth/BluetoothLEManager.h \
    managers/body_composition/BodyCompositionManager.h \
    managers/cdtt/CDTTManager.h \
    managers/choice_reaction/ChoiceReactionManager.h \
    managers/dxa/dicom/DicomSCP.h \
    #managers/dxa/dicom/LogWorker.h \
    managers/grip_strength/GripStrengthManager.h \
    managers/ManagerBase.h \
    managers/ecg/ECGManager.h \
    managers/emr/EMRPluginWriter.h \
    managers/frax/FraxManager.h \
    managers/ManagerBase.h \
    managers/dxa/DXAManager.h \
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
    server/defaultdeletesessionrequesthandler.h \
    server/handlers/BodyCompositionRequestHandler.h \
    server/handlers/audiometer_request_handler.h \
    server/handlers/audiometer_status_requesthandler.h \
    server/handlers/blood_pressure_request_handler.h \
    server/handlers/blood_pressure_status_request_handler.h \
    server/handlers/body_composition_status_request_handler.h \
    server/handlers/cdtt_request_handler.h \
    server/handlers/cdtt_status_request_handler.h \
    server/handlers/choice_reaction_request_handler.h \
    server/handlers/choice_reaction_status_request_handler.h \
    server/handlers/cypress_status_request_handler.h \
    server/handlers/dxa_request_handler.h \
    server/handlers/dxa_status_request_handler.h \
    server/handlers/ecg_request_handler.h \
    server/handlers/ecg_status_request_handler.h \
    server/handlers/frax_request_handler.h \
    server/handlers/frax_status_request_handler.h \
    server/handlers/grip_strength_request_handler.h \
    server/handlers/grip_strength_status_request_handler.h \
    server/handlers/retinal_camera_request_handler.h \
    server/handlers/retinal_camera_status_request_handler.h \
    server/handlers/signature_pad_request_handler.h \
    server/handlers/signature_pad_status_request_handler.h \
    server/handlers/spirometer_request_handler.h \
    server/handlers/spirometer_status_request_handler.h \
    server/handlers/tonometer_request_handler.h \
    server/handlers/tonometer_status_request_handler.h \
    server/handlers/ultrasound_request_handler.h \
    server/handlers/ultrasound_status_request_handler.h \
    server/handlers/weigh_scale_request_handler.h \
    server/handlers/weigh_scale_status_request_handler.h \
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
    CypressApplication.h \
    widgets/testinfowidget.h

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
  dialogs/weighscaledialog.ui \
  widgets/testinfowidget.ui

RESOURCES += \

DISTFILES += \
    ../README.md \
    RunNetworkRequests.py \
    clear.tif \
    favicon.ico \
    ok.tif \
    please.tif \
    server/pages/index.html \
    sign.tif \
    tests/fixtures/audiometer/input.json \
    tests/fixtures/audiometer/output.json \
    tests/fixtures/audiometer/settings.json \
    tests/fixtures/blood_pressure/input.json \
    tests/fixtures/blood_pressure/output.json \
    tests/fixtures/blood_pressure/settings.json \
    tests/fixtures/body_composition/input.json \
    tests/fixtures/body_composition/output.json \
    tests/fixtures/body_composition/settings.json \
    tests/fixtures/cdtt/input.json \
    tests/fixtures/cdtt/output.json \
    tests/fixtures/cdtt/settings.json \
    tests/fixtures/choice_reaction/input.json \
    tests/fixtures/choice_reaction/output.json \
    tests/fixtures/choice_reaction/settings.json \
    tests/fixtures/dxa/hip/hip.json \
    tests/fixtures/dxa/input.json \
    tests/fixtures/dxa/iva/iva.json \
    tests/fixtures/dxa/output.json \
    tests/fixtures/dxa/spine/spine.json \
    tests/fixtures/dxa/whole_body/whole_body.json \
    tests/fixtures/frax/input.json \
    tests/fixtures/frax/output.json \
    tests/fixtures/frax/settings.json \
    tests/fixtures/grip_strength/input.json \
    tests/fixtures/grip_strength/output.json \
    tests/fixtures/input.json \
    tests/fixtures/output.json \
    tests/fixtures/retinal_camera/input.json \
    tests/fixtures/retinal_camera/output.json \
    tests/fixtures/thermometer/input.json \
    tests/fixtures/thermometer/output.json \
    tests/fixtures/thermometer/settings.json \
    tests/fixtures/tonometer/input.json \
    tests/fixtures/tonometer/output.json \
    tests/fixtures/tonometer/settings.json \
    tests/fixtures/ultrasound/input.json \
    tests/fixtures/ultrasound/output.json \
    tests/fixtures/ultrasound/settings.json \
    tests/fixtures/weigh_scale/input.json \
    tests/fixtures/weigh_scale/output.json \
    tests/fixtures/weigh_scale/settings.json \
    thankyou.tif
