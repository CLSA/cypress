#include "vivid_iq_manager.h"
#include "data/ultrasound/vivid_iq_test.h"
#include "data/ultrasound/vivid_iq_measurement.h"

#include "auxiliary/windows_util.h"
#include "auxiliary/file_utils.h"

DeviceConfig VividIQManager::config {{
    { "runnableName", { "vivid_iq/dicom/runnableName", Exe  }},
    { "runnablePath", { "vivid_iq/dicom/runnablePath", Dir  }},
    { "aeTitle",      { "vivid_iq/dicom/aeTitle",      NonEmptyString }},
    { "host",         { "vivid_iq/dicom/host",         NonEmptyString }},
    { "port",         { "vivid_iq/dicom/port",         NonEmptyString }},
    { "storagePath",  { "vivid_iq/dicom/storagePath",  Dir }},
    { "logConfig",    { "vivid_iq/dicom/log_config",   File }},
    { "ascConfig",    { "vivid_iq/dicom/asc_config",   File }},
}};

VividIQManager::VividIQManager(QSharedPointer<CypressSession> session) : ManagerBase(session)
{
    m_test.reset(new VividIQTest);

    m_runnableName = config.getSetting("runnableName");
    m_runnablePath = config.getSetting("runnablePath");

    m_aeTitle = config.getSetting("aeTitle");
    m_host = config.getSetting("host");
    m_port = config.getSetting("port");

    m_storageDirPath = config.getSetting("storagePath");
    m_logConfigPath = config.getSetting("logConfig");
    m_ascConfigPath = config.getSetting("ascConfig");

    m_dicomServer.reset(
        new DcmRecv(m_runnableName, m_ascConfigPath, m_storageDirPath, m_aeTitle, m_port));

    WindowsUtil::killProcessByName(L"storescp.exe");

    connect(m_dicomServer.get(),
            &DcmRecv::dicomFilesReceived,
            this,
            &VividIQManager::dicomFilesReceived);
}

bool VividIQManager::start()
{
    qDebug() << "VividIQManager::start";
    return true;
}

void VividIQManager::finish()
{
    qDebug() << "VividIQManager::finish";
}

void VividIQManager::dicomFilesReceived(QList<DicomFile> dicomFiles)
{
    foreach (DicomFile file, dicomFiles)
    {
        QSharedPointer<VividIQMeasurement> measure(new VividIQMeasurement);

        if (m_session->getBarcode() != file.patientId) {
            qDebug() << "Received wrong id" << file.patientId;
            continue;
        }

        measure->setAttribute("patient_id", file.patientId);
        measure->setAttribute("study_id", 	file.studyId);
        measure->setAttribute("path", 		file.absFilePath);
        measure->setAttribute("size", 		FileUtils::getHumanReadableFileSize(file.absFilePath));

        m_test->addMeasurement(measure);
    }

    emit dataChanged(m_test);
    checkIfFinished();
}
