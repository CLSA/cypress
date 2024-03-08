QT += testlib gui widgets sql xml network serialport
CONFIG += qt warn_on depend_includepath testcase

# QtUsb
INCLUDEPATH += C:/work/clsa/cypress/dep/QtUsb_x86/include
LIBS += -L"C:/work/clsa/cypress/dep/QtUsb_x86/lib" -lQt5Usb

# OpenSSL
INCLUDEPATH += C:/work/clsa/cypress/dep/OpenSSL-Win32/include
LIBS += -L"C:/work/clsa/cypress/dep/OpenSSL-Win32/lib" -llibssl -llibcrypto

# POCO
INCLUDEPATH += C:/work/clsa/cypress/dep/poco-1.12.4-all/Foundation/include
INCLUDEPATH += C:/work/clsa/cypress/dep/poco-1.12.4-all/Util/include
INCLUDEPATH += C:/work/clsa/cypress/dep/poco-1.12.4-all/Net/include
INCLUDEPATH += C:/work/clsa/cypress/dep/poco-1.12.4-all/Crypto/include
INCLUDEPATH += C:/work/clsa/cypress/dep/poco-1.12.4-all/NetSSL_OpenSSL/include
INCLUDEPATH += C:/work/clsa/cypress/dep/poco-1.12.4-all/bin
LIBS += -L"C:/work/clsa/cypress/dep/poco-1.12.4-all/lib/" -lPocoFoundation -lPocoUtil -lPocoCrypto -lPocoNet -lPocoNetSSL

# DCMTK
INCLUDEPATH += "C:/Program Files (x86)/DCMTK/include"
LIBS += -L"C:/Program Files (x86)/DCMTK/lib" -ldcmdata -loflog -lofstd -lws2_32 -lnetapi32 -lwsock32 -ladvapi32 -liphlpapi

TEMPLATE = app

INCLUDEPATH += C:/work/clsa/cypress/src

SOURCES +=  tst_tonometertests.cpp \
    ../../auxiliary/CRC8.cpp \
    ../../auxiliary/Constants.cpp \
    ../../auxiliary/Utilities.cpp \
    ../../auxiliary/command_line_parser.cpp \
    ../../auxiliary/file_compressor.cpp \
    ../../auxiliary/file_utils.cpp \
    ../../auxiliary/json_settings.cpp \
    ../../auxiliary/network_utils.cpp \
    ../../auxiliary/pdf_form_filler.cpp \
    ../../auxiliary/tracker5_util.cpp \
    ../../auxiliary/windows_util.cpp \
    ../../cypress_application.cpp \
    ../../cypress_main_window.cpp \
    ../../cypress_session.cpp \
    ../../cypress_settings.cpp \
    ../../data/Measurement.cpp \
    ../../data/access_query_helper.cpp \
    ../../data/blood_pressure/measurements/blood_pressure_measurement.cpp \
    ../../data/blood_pressure/tests/blood_pressure_test.cpp \
    ../../data/body_composition/commands.cpp \
    ../../data/body_composition/measurements/body_composition_measurement.cpp \
    ../../data/body_composition/tests/body_composition_test.cpp \
    ../../data/cdtt/measurements/cdtt_measurement.cpp \
    ../../data/cdtt/tests/cdtt_test.cpp \
    ../../data/choice_reaction/measurements/choice_reaction_measurement.cpp \
    ../../data/choice_reaction/tests/choice_reaction_test.cpp \
    ../../data/cimt_vivid_i_measurement.cpp \
    ../../data/cimt_vivid_i_test.cpp \
    ../../data/dxa/measurements/ap_spine_measurement.cpp \
    ../../data/dxa/measurements/dxa_measurement.cpp \
    ../../data/dxa/measurements/forearm_measurement.cpp \
    ../../data/dxa/measurements/hip_measurement.cpp \
    ../../data/dxa/measurements/iva_imaging_measurement.cpp \
    ../../data/dxa/measurements/whole_body_measurement.cpp \
    ../../data/dxa/pat_scan_query_helper.cpp \
    ../../data/dxa/reference_query_helper.cpp \
    ../../data/dxa/smbfilecopier.cpp \
    ../../data/dxa/tests/dxa_hip_test.cpp \
    ../../data/dxa/tests/dxa_test.cpp \
    ../../data/ecg/measurements/ecg_measurement.cpp \
    ../../data/ecg/tests/ecg_test.cpp \
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
    ../../data/spirometer/measurements/spirometer_measurement.cpp \
    ../../data/spirometer/tests/spirometer_test.cpp \
    ../../data/temperature/measurements/temperature_measurement.cpp \
    ../../data/temperature/tests/temperature_test.cpp \
    ../../data/test_base.cpp \
    ../../data/tonometer/measurements/tonometer_measurement.cpp \
    ../../data/tonometer/tests/tonometer_test.cpp \
    ../../data/weigh_scale/measurements/weight_measurement.cpp \
    ../../data/weigh_scale/tests/weigh_scale_test.cpp \
    ../../dialogs/audiometer_dialog.cpp \
    ../../dialogs/blood_pressure_dialog.cpp \
    ../../dialogs/body_composition_dialog.cpp \
    ../../dialogs/cdtt_dialog.cpp \
    ../../dialogs/choice_reaction_dialog.cpp \
    ../../dialogs/cimt_vivid_i_dialog.cpp \
    ../../dialogs/cimt_vivid_iq_dialog.cpp \
    ../../dialogs/dialog_base.cpp \
    ../../dialogs/dialog_factory.cpp \
    ../../dialogs/dxa_dialog.cpp \
    ../../dialogs/dxa_hip_dialog.cpp \
    ../../dialogs/ecg_dialog.cpp \
    ../../dialogs/frax_dialog.cpp \
    ../../dialogs/grip_strength_dialog.cpp \
    ../../dialogs/retinal_camera_dialog.cpp \
    ../../dialogs/spirometer_dialog.cpp \
    ../../dialogs/tonometer_dialog.cpp \
    ../../dialogs/weigh_scale_dialog.cpp \
    ../../dicom/dcm_recv.cpp \
    ../../dicom/dicom_directory_watcher.cpp \
    ../../managers/audiometer/audiometer_manager.cpp \
    ../../managers/blood_pressure/blood_pressure_manager.cpp \
    ../../managers/blood_pressure/bpm_communication.cpp \
    ../../managers/blood_pressure/bpm_message.cpp \
    ../../managers/blood_pressure/bpm_messages.cpp \
    ../../managers/blood_pressure/bptru_200_driver.cpp \
    ../../managers/body_composition/body_composition_manager.cpp \
    ../../managers/cdtt/cdtt_manager.cpp \
    ../../managers/choice_reaction/choice_reaction_manager.cpp \
    ../../managers/dxa/dicom/LogWorker.cpp \
    ../../managers/dxa/dicom/dicom_scp.cpp \
    ../../managers/dxa/dxa_hip_manager.cpp \
    ../../managers/dxa/dxa_manager.cpp \
    ../../managers/ecg/ecg_manager.cpp \
    ../../managers/emr/emr_plugin_writer.cpp \
    ../../managers/frax/frax_manager.cpp \
    ../../managers/general_proxy_form/general_proxy_manager.cpp \
    ../../managers/grip_strength/grip_strength_manager.cpp \
    ../../managers/grip_strength/paradox_reader.cpp \
    ../../managers/manager_base.cpp \
    ../../managers/participant_report/participant_report_manager.cpp \
    ../../managers/retinal_camera/retinal_camera_manager.cpp \
    ../../managers/serial_port/serial_port_manager.cpp \
    ../../managers/spirometer/spirometer_manager.cpp \
    ../../managers/tonometer/tonometer_manager.cpp \
    ../../managers/ultrasound/vividi_manager.cpp \
    ../../managers/weigh_scale/weigh_scale_manager.cpp \
    ../../server/default_delete_request_handler.cpp \
    ../../server/default_request_handler.cpp \
    ../../server/default_status_request_handler.cpp \
    ../../server/handlers/audiometer/audiometer_request_handler.cpp \
    ../../server/handlers/audiometer/audiometer_status_request_handler.cpp \
    ../../server/handlers/blood_pressure/blood_pressure_request_handler.cpp \
    ../../server/handlers/blood_pressure/blood_pressure_status_request_handler.cpp \
    ../../server/handlers/body_composition/body_composition_request_handler.cpp \
    ../../server/handlers/body_composition/body_composition_status_request_handler.cpp \
    ../../server/handlers/cdtt/cdtt_request_handler.cpp \
    ../../server/handlers/cdtt/cdtt_status_request_handler.cpp \
    ../../server/handlers/choice_reaction/choice_reaction_request_handler.cpp \
    ../../server/handlers/choice_reaction/choice_reaction_status_request_handler.cpp \
    ../../server/handlers/cypress_status_request_handler.cpp \
    ../../server/handlers/dxa/dxa_hip_request_handler.cpp \
    ../../server/handlers/dxa/dxa_hip_session_request_handler.cpp \
    ../../server/handlers/dxa/dxa_hip_status_request_handler.cpp \
    ../../server/handlers/dxa/dxa_request_handler.cpp \
    ../../server/handlers/dxa/dxa_session_request_handler.cpp \
    ../../server/handlers/dxa/dxa_status_request_handler.cpp \
    ../../server/handlers/ecg/ecg_request_handler.cpp \
    ../../server/handlers/ecg/ecg_status_request_handler.cpp \
    ../../server/handlers/frax/frax_request_handler.cpp \
    ../../server/handlers/frax/frax_status_request_handler.cpp \
    ../../server/handlers/general_proxy_consent/general_proxy_consent_request_handler.cpp \
    ../../server/handlers/grip_strength/grip_strength_request_handler.cpp \
    ../../server/handlers/grip_strength/grip_strength_status_request_handler.cpp \
    ../../server/handlers/participant_report/participant_report_request_handler.cpp \
    ../../server/handlers/retinal_camera/retinal_camera_left_request_handler.cpp \
    ../../server/handlers/retinal_camera/retinal_camera_request_handler.cpp \
    ../../server/handlers/retinal_camera/retinal_camera_right_request_handler.cpp \
    ../../server/handlers/retinal_camera/retinal_camera_status_request_handler.cpp \
    ../../server/handlers/spirometer/spirometer_request_handler.cpp \
    ../../server/handlers/spirometer/spirometer_status_request_handler.cpp \
    ../../server/handlers/tonometer/tonometer_request_handler.cpp \
    ../../server/handlers/tonometer/tonometer_status_request_handler.cpp \
    ../../server/handlers/ultrasound/ultrasound_request_handler.cpp \
    ../../server/handlers/ultrasound/ultrasound_status_request_handler.cpp \
    ../../server/handlers/weigh_scale/weigh_scale_request_handler.cpp \
    ../../server/handlers/weigh_scale/weigh_scale_status_request_handler.cpp \
    ../../server/instrument_request_handler_factory.cpp \
    ../../server/server.cpp \
    ../../server/sessions/audiometer_session.cpp \
    ../../server/sessions/bpm_session.cpp \
    ../../server/sessions/cdtt_session.cpp \
    ../../server/sessions/choice_reaction_session.cpp \
    ../../server/sessions/dxa/dxa_hip_session.cpp \
    ../../server/sessions/dxa/dxa_session.cpp \
    ../../server/sessions/ecg_session.cpp \
    ../../server/sessions/frax_session.cpp \
    ../../server/sessions/gen_proxy_session.cpp \
    ../../server/sessions/grip_strength_session.cpp \
    ../../server/sessions/participant_report_session.cpp \
    ../../server/sessions/retinal_camera_session.cpp \
    ../../server/sessions/spirometer_session.cpp \
    ../../server/sessions/tonometer_session.cpp \
    ../../server/sessions/ultrasound_session.cpp \
    ../../server/sessions/weigh_scale_session.cpp \
    ../../server/utils.cpp \
    ../../widgets/device_settings/audiometer_settings_widget.cpp \
    ../../widgets/device_settings/blood_pressure_settings_widget.cpp \
    ../../widgets/device_settings/body_composition_settings_widget.cpp \
    ../../widgets/device_settings/cdtt_settings_widget.cpp \
    ../../widgets/device_settings/choice_reaction_settings_widget.cpp \
    ../../widgets/device_settings/dexa_settings_widget.cpp \
    ../../widgets/device_settings/ecg_settings_widget.cpp \
    ../../widgets/device_settings/frax_settings_widget.cpp \
    ../../widgets/device_settings/grip_strength_settings_widget.cpp \
    ../../widgets/device_settings/retinal_camera_settings_widget.cpp \
    ../../widgets/device_settings/spirometer_settings_widget.cpp \
    ../../widgets/device_settings/tonometer_settings_widget.cpp \
    ../../widgets/device_settings/vivid_i_settings_widget.cpp \
    ../../widgets/device_settings/weight_scale_settings_widget.cpp \
    ../../widgets/dicom_widget.cpp \
    ../../widgets/directory_picker_widget.cpp \
    ../../widgets/file_picker_widget.cpp \
    ../../widgets/measurement_table.cpp \
    ../../widgets/serial_port_picker_widget.cpp \
    ../../widgets/serial_port_widget.cpp \
    ../../widgets/settings_dialog.cpp \
    ../../widgets/submission_buttons.cpp \
    ../../widgets/test_info_widget.cpp \
    ../../widgets/usb_port_picker.cpp


HEADERS += \
    ../../auxiliary/CRC8.h \
    ../../auxiliary/Constants.h \
    ../../auxiliary/Utilities.h \
    ../../auxiliary/command_line_parser.h \
    ../../auxiliary/file_compressor.h \
    ../../auxiliary/file_utils.h \
    ../../auxiliary/json_settings.h \
    ../../auxiliary/network_utils.h \
    ../../auxiliary/pdf_form_filler.h \
    ../../auxiliary/tracker5_util.h \
    ../../auxiliary/windows_util.h \
    ../../cypress_application.h \
    ../../cypress_main_window.h \
    ../../cypress_session.h \
    ../../cypress_settings.h \
    ../../data/Measurement.h \
    ../../data/access_query_helper.h \
    ../../data/blood_pressure/measurements/blood_pressure_measurement.h \
    ../../data/blood_pressure/tests/blood_pressure_test.h \
    ../../data/body_composition/commands.h \
    ../../data/body_composition/measurements/body_composition_measurement.h \
    ../../data/body_composition/tests/body_composition_test.h \
    ../../data/cdtt/measurements/cdtt_measurement.h \
    ../../data/cdtt/tests/cdtt_test.h \
    ../../data/choice_reaction/measurements/choice_reaction_measurement.h \
    ../../data/choice_reaction/tests/choice_reaction_test.h \
    ../../data/cimt_vivid_i_measurement.h \
    ../../data/cimt_vivid_i_test.h \
    ../../data/dxa/measurements/ap_spine_measurement.h \
    ../../data/dxa/measurements/dxa_measurement.h \
    ../../data/dxa/measurements/forearm_measurement.h \
    ../../data/dxa/measurements/hip_measurement.h \
    ../../data/dxa/measurements/iva_imaging_measurement.h \
    ../../data/dxa/measurements/whole_body_measurement.h \
    ../../data/dxa/pat_scan_query_helper.h \
    ../../data/dxa/reference_query_helper.h \
    ../../data/dxa/smbfilecopier.h \
    ../../data/dxa/tests/dxa_hip_test.h \
    ../../data/dxa/tests/dxa_test.h \
    ../../data/ecg/measurements/ecg_measurement.h \
    ../../data/ecg/tests/ecg_test.h \
    ../../data/excel_query_helper.h \
    ../../data/frax/measurements/frax_measurement.h \
    ../../data/frax/tests/frax_test.h \
    ../../data/grip_strength/measurements/grip_strength_measurement.h \
    ../../data/grip_strength/tests/grip_strength_test.h \
    ../../data/hearing/measurements/hearing_measurement.h \
    ../../data/hearing/tests/hearing_test.h \
    ../../data/retinal_camera/database_manager.h \
    ../../data/retinal_camera/retinal_camera_measurement.h \
    ../../data/retinal_camera/retinal_camera_test.h \
    ../../data/spirometer/measurements/spirometer_measurement.h \
    ../../data/spirometer/tests/spirometer_test.h \
    ../../data/temperature/measurements/temperature_measurement.h \
    ../../data/temperature/tests/temperature_test.h \
    ../../data/test_base.h \
    ../../data/tonometer/measurements/tonometer_measurement.h \
    ../../data/tonometer/tests/tonometer_test.h \
    ../../data/weigh_scale/measurements/weight_measurement.h \
    ../../data/weigh_scale/tests/weigh_scale_test.h \
    ../../dialogs/audiometer_dialog.h \
    ../../dialogs/blood_pressure_dialog.h \
    ../../dialogs/body_composition_dialog.h \
    ../../dialogs/cdtt_dialog.h \
    ../../dialogs/choice_reaction_dialog.h \
    ../../dialogs/cimt_vivid_i_dialog.h \
    ../../dialogs/cimt_vivid_iq_dialog.h \
    ../../dialogs/dialog_base.h \
    ../../dialogs/dialog_factory.h \
    ../../dialogs/dxa_dialog.h \
    ../../dialogs/dxa_hip_dialog.h \
    ../../dialogs/ecg_dialog.h \
    ../../dialogs/frax_dialog.h \
    ../../dialogs/grip_strength_dialog.h \
    ../../dialogs/retinal_camera_dialog.h \
    ../../dialogs/spirometer_dialog.h \
    ../../dialogs/tonometer_dialog.h \
    ../../dialogs/weigh_scale_dialog.h \
    ../../dicom/dcm_recv.h \
    ../../dicom/dicom_directory_watcher.h \
    ../../managers/audiometer/audiometer_manager.h \
    ../../managers/blood_pressure/blood_pressure_manager.h \
    ../../managers/blood_pressure/bpm_communication.h \
    ../../managers/blood_pressure/bpm_message.h \
    ../../managers/blood_pressure/bpm_messages.h \
    ../../managers/blood_pressure/bptru_200_driver.h \
    ../../managers/body_composition/body_composition_manager.h \
    ../../managers/cdtt/cdtt_manager.h \
    ../../managers/choice_reaction/choice_reaction_manager.h \
    ../../managers/dxa/dicom/LogWorker.h \
    ../../managers/dxa/dicom/dicom_scp.h \
    ../../managers/dxa/dxa_hip_manager.h \
    ../../managers/dxa/dxa_manager.h \
    ../../managers/ecg/ecg_manager.h \
    ../../managers/emr/emr_plugin_writer.h \
    ../../managers/frax/frax_manager.h \
    ../../managers/general_proxy_form/general_proxy_manager.h \
    ../../managers/grip_strength/grip_strength_manager.h \
    ../../managers/grip_strength/paradox_reader.h \
    ../../managers/manager_base.h \
    ../../managers/participant_report/participant_report_manager.h \
    ../../managers/retinal_camera/retinal_camera_manager.h \
    ../../managers/serial_port/serial_port_manager.h \
    ../../managers/spirometer/spirometer_manager.h \
    ../../managers/tonometer/tonometer_manager.h \
    ../../managers/ultrasound/vividi_manager.h \
    ../../managers/weigh_scale/weigh_scale_manager.h \
    ../../server/Server.h \
    ../../server/default_delete_request_handler.h \
    ../../server/default_request_handler.h \
    ../../server/default_status_request_handler.h \
    ../../server/handlers/audiometer/audiometer_request_handler.h \
    ../../server/handlers/audiometer/audiometer_status_request_handler.h \
    ../../server/handlers/blood_pressure/blood_pressure_request_handler.h \
    ../../server/handlers/blood_pressure/blood_pressure_status_request_handler.h \
    ../../server/handlers/body_composition/body_composition_request_handler.h \
    ../../server/handlers/body_composition/body_composition_status_request_handler.h \
    ../../server/handlers/cdtt/cdtt_request_handler.h \
    ../../server/handlers/cdtt/cdtt_status_request_handler.h \
    ../../server/handlers/choice_reaction/choice_reaction_request_handler.h \
    ../../server/handlers/choice_reaction/choice_reaction_status_request_handler.h \
    ../../server/handlers/cypress_status_request_handler.h \
    ../../server/handlers/dxa/dxa_hip_request_handler.h \
    ../../server/handlers/dxa/dxa_hip_session_request_handler.h \
    ../../server/handlers/dxa/dxa_hip_status_request_handler.h \
    ../../server/handlers/dxa/dxa_request_handler.h \
    ../../server/handlers/dxa/dxa_session_request_handler.h \
    ../../server/handlers/dxa/dxa_status_request_handler.h \
    ../../server/handlers/ecg/ecg_request_handler.h \
    ../../server/handlers/ecg/ecg_status_request_handler.h \
    ../../server/handlers/frax/frax_request_handler.h \
    ../../server/handlers/frax/frax_status_request_handler.h \
    ../../server/handlers/general_proxy_consent/general_proxy_consent_request_handler.h \
    ../../server/handlers/grip_strength/grip_strength_request_handler.h \
    ../../server/handlers/grip_strength/grip_strength_status_request_handler.h \
    ../../server/handlers/participant_report/participant_report_request_handler.h \
    ../../server/handlers/retinal_camera/retinal_camera_left_request_handler.h \
    ../../server/handlers/retinal_camera/retinal_camera_request_handler.h \
    ../../server/handlers/retinal_camera/retinal_camera_right_request_handler.h \
    ../../server/handlers/retinal_camera/retinal_camera_status_request_handler.h \
    ../../server/handlers/spirometer/spirometer_request_handler.h \
    ../../server/handlers/spirometer/spirometer_status_request_handler.h \
    ../../server/handlers/tonometer/tonometer_request_handler.h \
    ../../server/handlers/tonometer/tonometer_status_request_handler.h \
    ../../server/handlers/ultrasound/ultrasound_request_handler.h \
    ../../server/handlers/ultrasound/ultrasound_status_request_handler.h \
    ../../server/handlers/weigh_scale/weigh_scale_request_handler.h \
    ../../server/handlers/weigh_scale/weigh_scale_status_request_handler.h \
    ../../server/instrument_request_handler_factory.h \
    ../../server/sessions/audiometer_session.h \
    ../../server/sessions/bpm_session.h \
    ../../server/sessions/cdtt_session.h \
    ../../server/sessions/choice_reaction_session.h \
    ../../server/sessions/dxa/dxa_hip_session.h \
    ../../server/sessions/dxa/dxa_session.h \
    ../../server/sessions/ecg_session.h \
    ../../server/sessions/frax_session.h \
    ../../server/sessions/gen_proxy_session.h \
    ../../server/sessions/grip_strength_session.h \
    ../../server/sessions/participant_report_session.h \
    ../../server/sessions/retinal_camera_session.h \
    ../../server/sessions/spirometer_session.h \
    ../../server/sessions/tonometer_session.h \
    ../../server/sessions/ultrasound_session.h \
    ../../server/sessions/weigh_scale_session.h \
    ../../server/utils.h \
    ../../widgets/device_settings/audiometer_settings_widget.h \
    ../../widgets/device_settings/blood_pressure_settings_widget.h \
    ../../widgets/device_settings/body_composition_settings_widget.h \
    ../../widgets/device_settings/cdtt_settings_widget.h \
    ../../widgets/device_settings/choice_reaction_settings_widget.h \
    ../../widgets/device_settings/dexa_settings_widget.h \
    ../../widgets/device_settings/ecg_settings_widget.h \
    ../../widgets/device_settings/frax_settings_widget.h \
    ../../widgets/device_settings/grip_strength_settings_widget.h \
    ../../widgets/device_settings/retinal_camera_settings_widget.h \
    ../../widgets/device_settings/spirometer_settings_widget.h \
    ../../widgets/device_settings/tonometer_settings_widget.h \
    ../../widgets/device_settings/vivid_i_settings_widget.h \
    ../../widgets/device_settings/weight_scale_settings_widget.h \
    ../../widgets/dicom_widget.h \
    ../../widgets/directory_picker_widget.h \
    ../../widgets/file_picker_widget.h \
    ../../widgets/measurement_table.h \
    ../../widgets/serial_port_picker_widget.h \
    ../../widgets/serial_port_widget.h \
    ../../widgets/settings_dialog.h \
    ../../widgets/submission_buttons.h \
    ../../widgets/test_info_widget.h \
    ../../widgets/usb_port_picker.h


DISTFILES += \
    ../../server/pages/index.html

SUBDIRS += \
    ../../examples/qtusb/qtusb_test.pro

RESOURCES += \
    ../../dialogs/cypress.qrc

FORMS += \
    ../../cypress_main_window.ui \
    ../../dialogs/audiometer_dialog.ui \
    ../../dialogs/blood_pressure_dialog.ui \
    ../../dialogs/body_composition_dialog.ui \
    ../../dialogs/cdtt_dialog.ui \
    ../../dialogs/choice_reaction_dialog.ui \
    ../../dialogs/cimt_vivid_i_dialog.ui \
    ../../dialogs/cimt_vivid_iq_dialog.ui \
    ../../dialogs/dxa_dialog.ui \
    ../../dialogs/dxa_hip_dialog.ui \
    ../../dialogs/ecg_dialog.ui \
    ../../dialogs/frax_dialog.ui \
    ../../dialogs/grip_strength_dialog.ui \
    ../../dialogs/retinal_camera_dialog.ui \
    ../../dialogs/runnable_dialog.ui \
    ../../dialogs/spirometer_dialog.ui \
    ../../dialogs/thermometerdialog.ui \
    ../../dialogs/tonometer_dialog.ui \
    ../../dialogs/weigh_scale_dialog.ui \
    ../../widgets/device_settings/audiometer_settings_widget.ui \
    ../../widgets/device_settings/blood_pressure_settings_widget.ui \
    ../../widgets/device_settings/body_composition_settings_widget.ui \
    ../../widgets/device_settings/cdtt_settings_widget.ui \
    ../../widgets/device_settings/choice_reaction_settings_widget.ui \
    ../../widgets/device_settings/dexa_settings_widget.ui \
    ../../widgets/device_settings/ecg_settings_widget.ui \
    ../../widgets/device_settings/frax_settings_widget.ui \
    ../../widgets/device_settings/grip_strength_settings_widget.ui \
    ../../widgets/device_settings/retinal_camera_settings_widget.ui \
    ../../widgets/device_settings/spirometer_settings_widget.ui \
    ../../widgets/device_settings/tonometer_settings_widget.ui \
    ../../widgets/device_settings/vivid_i_settings_widget.ui \
    ../../widgets/device_settings/weight_scale_settings_widget.ui \
    ../../widgets/dicom_widget.ui \
    ../../widgets/directory_picker_widget.ui \
    ../../widgets/file_picker_widget.ui \
    ../../widgets/measurement_table.ui \
    ../../widgets/serial_port_picker_widget.ui \
    ../../widgets/serial_port_widget.ui \
    ../../widgets/settings_dialog.ui \
    ../../widgets/submission_buttons.ui \
    ../../widgets/test_info_widget.ui \
    ../../widgets/usb_port_picker.ui