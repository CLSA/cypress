QT += core gui sql widgets testlib network xml bluetooth
CONFIG += qt warn_on depend_includepath testcase
CONGIG += no_testcase_installs

TEMPLATE = app

SOURCES +=  \
    ../auxiliary/BluetoothUtil.cpp \
    ../auxiliary/CRC8.cpp \
    ../auxiliary/CommandLineParser.cpp \
    ../auxiliary/Constants.cpp \
    ../auxiliary/JsonSettings.cpp \
    ../auxiliary/Utilities.cpp \
    ../auxiliary/tracker5util.cpp \
    ../data/AccessQueryHelper.cpp \
    ../data/ExcelQueryHelper.cpp \
    ../data/Measurement.cpp \
    ../data/MeasurementBase.cpp \
    ../data/blood_pressure/measurements/BloodPressureMeasurement.cpp \
    ../data/blood_pressure/tests/BloodPressureTest.cpp \
    ../data/body_composition/measurements/BodyCompositionMeasurement.cpp \
    ../data/body_composition/tests/BodyCompositionTest.cpp \
    ../data/cdtt/measurements/CDTTMeasurement.cpp \
    ../data/cdtt/tests/CDTTTest.cpp \
    ../data/choice_reaction/measurements/ChoiceReactionMeasurement.cpp \
    ../data/choice_reaction/tests/ChoiceReactionTest.cpp \
    ../data/dxa/PatScanQueryHelper.cpp \
    ../data/dxa/ReferenceQueryHelper.cpp \
    ../data/dxa/measurements/ApSpineMeasurement.cpp \
    ../data/dxa/measurements/DXAMeasurement.cpp \
    ../data/dxa/measurements/ForearmMeasurement.cpp \
    ../data/dxa/measurements/HipMeasurement.cpp \
    ../data/dxa/measurements/IVAImagingMeasurement.cpp \
    ../data/dxa/measurements/WholeBodyScanMeasurement.cpp \
    ../data/dxa/tests/ApSpineTest.cpp \
    ../data/dxa/tests/DXATest.cpp \
    ../data/dxa/tests/ForearmTest.cpp \
    ../data/dxa/tests/HipTest.cpp \
    ../data/dxa/tests/IVAImagingTest.cpp \
    ../data/dxa/tests/WholeBodyScanTest.cpp \
    ../data/ecg/measurements/ECGMeasurement.cpp \
    ../data/ecg/tests/ECGTest.cpp \
    ../data/frax/measurements/FraxMeasurement.cpp \
    ../data/frax/tests/FraxTest.cpp \
    ../data/grip_strength/measurements/GripStrengthMeasurement.cpp \
    ../data/grip_strength/tests/GripStrengthTest.cpp \
    ../data/hearing/measurements/HearingMeasurement.cpp \
    ../data/hearing/tests/HearingTest.cpp \
    ../data/spirometer/measurements/SpirometerMeasurement.cpp \
    ../data/spirometer/tests/SpirometerTest.cpp \
    ../data/temperature/measurements/TemperatureMeasurement.cpp \
    ../data/temperature/tests/TemperatureTest.cpp \
    ../data/tonometer/measurements/TonometerMeasurement.cpp \
    ../data/tonometer/tests/TonometerTest.cpp \
    ../data/weigh_scale/measurements/WeightMeasurement.cpp \
    ../data/weigh_scale/tests/WeighScaleTest.cpp \
    main.cpp \
    tst_audiometer.cpp \
    tst_blood_pressure.cpp \
    tst_body_composition.cpp \
    tst_cdtt.cpp \
    tst_choice_reaction.cpp \
    tst_dxa.cpp \
    tst_ecg.cpp \
    tst_emr.cpp \
    tst_frax.cpp \
    tst_grip_strength.cpp \
    tst_retinal_camera.cpp \
    tst_tonometer.cpp \
    tst_ultrasound.cpp \
    tst_weigh_scale.cpp

HEADERS += ../data/grip_strength/measurements/GripStrengthMeasurement.h \
    ../auxiliary/BluetoothUtil.h \
    ../auxiliary/CRC8.h \
    ../auxiliary/CommandLineParser.h \
    ../auxiliary/Constants.h \
    ../auxiliary/JsonSettings.h \
    ../auxiliary/Utilities.h \
    ../auxiliary/tracker5util.h \
    ../data/AccessQueryHelper.h \
    ../data/ExcelQueryHelper.h \
    ../data/Measurement.h \
    ../data/MeasurementBase.h \
    ../data/TestBase.h \
    ../data/blood_pressure/measurements/BloodPressureMeasurement.h \
    ../data/blood_pressure/tests/BloodPressureTest.h \
    ../data/body_composition/measurements/BodyCompositionMeasurement.h \
    ../data/body_composition/tests/BodyCompositionTest.h \
    ../data/cdtt/measurements/CDTTMeasurement.h \
    ../data/cdtt/tests/CDTTTest.h \
    ../data/choice_reaction/measurements/ChoiceReactionMeasurement.h \
    ../data/choice_reaction/tests/ChoiceReactionTest.h \
    ../data/dxa/PatScanQueryHelper.h \
    ../data/dxa/ReferenceQueryHelper.h \
    ../data/dxa/measurements/ApSpineMeasurement.h \
    ../data/dxa/measurements/DXAMeasurement.h \
    ../data/dxa/measurements/ForearmMeasurement.h \
    ../data/dxa/measurements/HipMeasurement.h \
    ../data/dxa/measurements/IVAImagingMeasurement.h \
    ../data/dxa/measurements/WholeBodyScanMeasurement.h \
    ../data/dxa/tests/ApSpineTest.h \
    ../data/dxa/tests/ForearmTest.h \
    ../data/dxa/tests/HipTest.h \
    ../data/dxa/tests/IVAImagingTest.h \
    ../data/dxa/tests/WholeBodyScanTest.h \
    ../data/ecg/measurements/ECGMeasurement.h \
    ../data/ecg/tests/ECGTest.h \
    ../data/frax/measurements/FraxMeasurement.h \
    ../data/frax/tests/FraxTest.h \
    ../data/grip_strength/tests/GripStrengthTest.h \
    ../data/hearing/measurements/HearingMeasurement.h \
    ../data/hearing/tests/HearingTest.h \
    ../data/spirometer/measurements/SpirometerMeasurement.h \
    ../data/spirometer/tests/SpirometerTest.h \
    ../data/temperature/measurements/TemperatureMeasurement.h \
    ../data/temperature/tests/TemperatureTest.h \
    ../data/tonometer/measurements/TonometerMeasurement.h \
    ../data/tonometer/tests/TonometerTest.h \
    ../data/weigh_scale/measurements/WeightMeasurement.h \
    ../data/weigh_scale/tests/WeighScaleTest.h \
    tst_audiometer.h \
    tst_blood_pressure.h \
    tst_body_composition.h \
    tst_cdtt.h \
    tst_choice_reaction.h \
    tst_dxa.h \
    tst_ecg.h \
    tst_emr.h \
    tst_frax.h \
    tst_grip_strength.h \
    tst_retinal_camera.h \
    tst_tonometer.h \
    tst_ultrasound.h \
    tst_weigh_scale.h

!contains(QMAKE_TARGET.arch, x86_64): {
    message("32-bit")

    INCLUDEPATH += "C:/Users/Anthony/vcpkg/packages/poco_x86-windows/include"

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
