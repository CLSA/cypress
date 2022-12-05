QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport bluetooth sql usb xml network testlib

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
    auxiliary/JsonSettings.cpp \
    auxiliary/Utilities.cpp \
    auxiliary/tracker5util.cpp \
    data/AccessQueryHelper.cpp \
    data/ExcelQueryHelper.cpp \
    data/dxa/PatScanQueryHelper.cpp \
    data/dxa/ReferenceQueryHelper.cpp \
    data/dxa/measurements/ApSpineMeasurement.cpp \
    data/dxa/measurements/DXAMeasurement.cpp \
    data/dxa/measurements/ForearmMeasurement.cpp \
    data/dxa/measurements/HipMeasurement.cpp \
    data/dxa/measurements/IVAImagingMeasurement.cpp \
    data/dxa/measurements/WholeBodyScanMeasurement.cpp \
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
    managers/dxa/ApSpineScanManager.cpp \
    managers/dxa/DXAManager.cpp \
    managers/dxa/ForearmScanManager.cpp \
    managers/dxa/HipScanManager.cpp \
    managers/dxa/IVAImagingScanManager.cpp \
    managers/dxa/WholeBodyScanManager.cpp \
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
    managers/spirometer/SpirometerManager.cpp \
    managers/tonometer/TonometerManager.cpp \
    managers/weigh_scale/WeighScaleManager.cpp \
    server/DefaultRequestHandler.cpp \
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
    server/handlers/InstrumentRequestHandlerFactory.cpp \
    server/handlers/RetinalCameraRequestHandler.cpp \
    server/handlers/SpirometerRequestHandler.cpp \
    server/handlers/TonometerRequestHandler.cpp \
    server/handlers/UltrasoundRequestHandler.cpp \
    server/handlers/WeighScaleRequestHandler.cpp \
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
    CypressApplication.cpp \
    main.cpp

HEADERS += \
    auxiliary/CommandLineParser.h \
    auxiliary/Constants.h \
    auxiliary/JsonSettings.h \
    auxiliary/Utilities.h \
    auxiliary/tracker5util.h \
    data/AccessQueryHelper.h \
    data/ExcelQueryHelper.h \
    data/MeasurementBase.h \
    data/blood_pressure/measurements/BloodPressureMeasurement.h \
    data/blood_pressure/tests/BloodPressureTest.h \
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
    data/dxa/tests/ApSpineTest.h \
    data/dxa/tests/ForearmTest.h \
    data/dxa/tests/HipTest.h \
    data/dxa/tests/IVAImagingTest.h \
    data/dxa/tests/WholeBodyScanTest.h \
    data/Measurement.h \
    data/TestBase.h \
#    data/TemperatureMeasurement.h \
#    data/TemperatureTest.h \
    data/grip_strength/measurements/GripStrengthMeasurement.h \
    data/grip_strength/tests/GripStrengthTest.h \
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
    managers/SettingsManager.h \
    managers/audiometer/AudiometerManager.h \
    managers/blood_pressure/BPMCommunication.h \
    managers/blood_pressure/BPMMessage.h \
    managers/blood_pressure/BloodPressureManager.h \
    managers/bluetooth/BluetoothLEManager.h \
    managers/body_composition/BodyCompositionManager.h \
    managers/cdtt/CDTTManager.h \
    managers/choice_reaction/ChoiceReactionManager.h \
    managers/dxa/ApSpineScanManager.h \
    managers/dxa/DXAManager.h \
    managers/dxa/ForearmScanManager.h \
    managers/dxa/HipScanManager.h \
    managers/dxa/IVAImagingScanManager.h \
    managers/dxa/WholeBodyScanManager.h \
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
    managers/spirometer/SpirometerManager.h \
    managers/tonometer/TonometerManager.h \
    managers/weigh_scale/WeighScaleManager.h \
    server/DefaultRequestHandler.h \
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
    server/handlers/InstrumentRequestHandlerFactory.h \
    server/handlers/RetinalCameraRequestHandler.h \
    server/handlers/SpirometerRequestHandler.h \
    server/handlers/TonometerRequestHandler.h \
    server/handlers/UltrasoundRequestHandler.h \
    server/handlers/WeighScaleRequestHandler.h \
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



!contains(QMAKE_TARGET.arch, x86_64): {
    message("32-bit")

    INCLUDEPATH += C:/Users/Anthony/vcpkg/packages/poco_x86-windows/include

    INCLUDEPATH += "C:/Program Files (x86)/DCMTK/include"
    INCLUDEPATH += "C:/Program Files (x86)/DCMTK/zlib-1.2.12/include"

    LIBS += -L"C:/Users/Anthony/vcpkg/packages/poco_x86-windows/lib" -lPocoNet -lPocoFoundation -lPocoUtil
    LIBS += -L"C:/Program Files (x86)/DCMTK/lib" -ldcmdata -loflog -lofstd -lws2_32 -lnetapi32 -lwsock32 -ladvapi32 -liphlpapi
    LIBS += -L"C:/Program Files (x86)/DCMTK/zlib-1.2.12/lib" -lzlib_o

} else {
    message("64-bit")
    INCLUDEPATH += C:/Users/Anthony/vcpkg/packages/poco_x64-windows/include
    LIBS += -L"C:/Users/Anthony/vcpkg/packages/poco_x64-windows/lib" -lPocoNet -lPocoFoundation -lPocoUtil

    INCLUDEPATH += "C:/Program Files/DCMTK/include"
    INCLUDEPATH += "C:/Program Files/DCMTK/zlib-1.2.12/include"

    LIBS += -L"C:/Users/Anthony/vcpkg/packages/poco_x86-windows/lib" -lPocoNet -lPocoFoundation -lPocoUtil
    LIBS += -L"C:/Program Files/DCMTK/lib" -ldcmdata -loflog -lofstd -lws2_32 -lnetapi32 -lwsock32 -ladvapi32 -liphlpapi
    LIBS += -L"C:/Program Files/DCMTK/zlib-1.2.12/lib" -lzlib_o
}


FORMS += \
  dialogs/RetinalCameraDialog.ui \
  dialogs/dxadialog.ui \
  dialogs/gripstrengthdialog.ui \
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
    favicon.ico \
    server/pages/index.html
