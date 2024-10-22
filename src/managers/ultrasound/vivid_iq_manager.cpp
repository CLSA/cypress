#include "vivid_iq_manager.h"
#include "data/ultrasound/vivid_iq_test.h"
#include "data/ultrasound/vivid_iq_measurement.h"

#include "auxiliary/windows_util.h"
#include "auxiliary/file_utils.h"
#include "auxiliary/network_utils.h"

#include <QMessageBox>
#include <QApplication>


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

    m_dicomServer->start();

    emit started(m_test);
    emit dataChanged(m_test);

    return true;
}

void VividIQManager::dicomFilesReceived(QList<DicomFile> dicomFiles)
{
    qDebug() << "VividIQManager::dicomFilesReceived";

    //bool foundInvalidParticipant = false;
    QString invalidId;

    qint64 totalSize { 0 };

    foreach (DicomFile file, dicomFiles)
    {
        QSharedPointer<VividIQMeasurement> measure(new VividIQMeasurement);

        //if (m_session->getBarcode() != file.patientId) {
        //    qDebug() << "Received wrong id" << file.patientId;
        //    foundInvalidParticipant = true;
        //    invalidId = file.patientId;
        //    continue;
        //}

        auto measurements = m_test->getMeasurements();
        bool foundDuplicate = false;

        for (auto measurement : measurements) {
            if (measurement->getAttribute("path").toString() == file.absFilePath) {
                foundDuplicate = true;
                qDebug() << "Found duplicate: " << file.absFilePath;
                break;
            }
        }

        if (foundDuplicate) {
            continue;
        }

        QFileInfo fileInfo(file.absFilePath);
        measure->setAttribute("name", 		     fileInfo.fileName());
        measure->setAttribute("patient_id",      file.patientId);
        measure->setAttribute("instance_number", file.instanceNumber);
        measure->setAttribute("study_id", 	     file.studyId);
        measure->setAttribute("path", 		     file.absFilePath);
        measure->setAttribute("size", 		     FileUtils::bytesToHumanReadable(fileInfo.size()));

        totalSize += fileInfo.size();

        m_test->addMeasurement(measure);
    }

    //if (foundInvalidParticipant)
    //    QMessageBox::warning(nullptr, "Invalid participant", invalidId + " does not match " + m_session->getBarcode());

    emit filesReceived(m_test->getMeasurementCount(), FileUtils::bytesToHumanReadable(totalSize));
    emit dataChanged(m_test);
    checkIfFinished();
}

void VividIQManager::finish()
{
    qDebug() << "VividIQManager::finish";

    QList<QJsonObject> filePaths;

    for (int i = 0; i < m_test->getMeasurementCount(); i++) {
        Measurement& measure = m_test->get(i);

        QString name = measure.getAttribute("name").toString();
        //if (name.contains("SRc")) {
        //    name = "SRc.dcm";
        //}
        //else if (name.contains("USm.")) {
        //    name = QString("USm_%1.dcm").arg(measure.getAttribute("instance_number").toString().toInt());
        //}
        //else if (name.contains("US.")) {
        //    name = QString("US_%1.dcm").arg(measure.getAttribute("instance_number").toString().toInt());
        //}

        filePaths.append(QJsonObject {
            {
             { "name", name },
             { "path", measure.getAttribute("path").toString() }
            },
        });
    }

    m_test->setFiles(filePaths);
    ManagerBase::finish();
}
