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
    emit canMeasure();

    return true;
}

void VividIQManager::dicomFilesReceived(QList<DicomFile> dicomFiles)
{
    qDebug() << "VividIQManager::dicomFilesReceived";

    bool foundInvalidParticipant = false;
    QString invalidId;

    foreach (DicomFile file, dicomFiles)
    {
        QSharedPointer<VividIQMeasurement> measure(new VividIQMeasurement);

        if (m_session->getBarcode() != file.patientId) {
            qDebug() << "Received wrong id" << file.patientId;
            foundInvalidParticipant = true;
            invalidId = file.patientId;
            continue;
        }

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
        measure->setAttribute("name", 		fileInfo.fileName());
        measure->setAttribute("patient_id", file.patientId);
        measure->setAttribute("study_id", 	file.studyId);
        measure->setAttribute("path", 		file.absFilePath);
        measure->setAttribute("size", 		FileUtils::getHumanReadableFileSize(file.absFilePath));

        m_test->addMeasurement(measure);
    }

    if (foundInvalidParticipant)
        QMessageBox::warning(nullptr, "Invalid participant", invalidId + " does not match " + m_session->getBarcode());

    emit dataChanged(m_test);
    checkIfFinished();
}

void VividIQManager::finish()
{
    qDebug() << "VividIQManager::finish";
    const QString answerUrl = getAnswerUrl();

    QJsonObject responseJson {};
    QJsonObject filesJson {};

    for (int i = 0; i < m_test->getMeasurementCount(); i++) {
        Measurement& measure = m_test->get(i);
        QByteArray data = FileUtils::readFile(measure.getAttribute("path").toString());

        filesJson.insert(
            measure.getAttribute("name").toString() + "_dcm",
            FileUtils::getHumanReadableFileSize(measure.getAttribute("path").toString())
        );

        QApplication::processEvents();
        bool ok = NetworkUtils::sendHTTPSRequest("PATCH",
                                   (answerUrl
                                                  + "?filename=" + measure.getAttribute("name").toString() + ".dcm").toStdString(),
                                   "application/octet-stream",
                                   data);
        if (!ok) {
            qDebug() << "could not send file: " << measure.getAttribute("name").toString();
            emit error("Could not send file");
            return;
        }

        measure.removeAttribute("PATH");
    }

    QJsonObject testJson = m_test->toJsonObject();
    QJsonObject sessionObj = m_session->getJsonObject();

    testJson.insert("session", sessionObj);
    testJson.insert("files", filesJson);
    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    bool ok = NetworkUtils::sendHTTPSRequest("PATCH", answerUrl.toStdString(), "application/json", serializedData);
    if (!ok) {
        emit error("Could not send results");
        return;
    }
    emit success("Sent measurements to Pine, you may now close this window");
}
