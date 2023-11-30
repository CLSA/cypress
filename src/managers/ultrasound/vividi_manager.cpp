#include "vividi_manager.h"

#include "data/cimt_vivid_i_test.h"
#include "auxiliary/file_utils.h"

#include <QJsonDocument>

VividiManager::VividiManager(QSharedPointer<UltrasoundSession> session)
    : ManagerBase(session)
{
    m_test.reset(new CimtVividiTest);

    QDir workingDir = QDir::current();
    QString workingDirPath = workingDir.absolutePath() + "/";

    const QString executablePath = workingDirPath + CypressSettings::readSetting("ultrasound/dicom/executable").toString();
    const QString aeTitle = CypressSettings::readSetting("ultrasound/dicom/aeTitle").toString();
    const QString host = CypressSettings::readSetting("ultrasound/dicom/host").toString();
    const QString port = CypressSettings::readSetting("ultrasound/dicom/port").toString();

    const QString storageDirPath = workingDirPath + CypressSettings::readSetting("ultrasound/dicom/storagePath").toString();
    const QString logConfigPath = workingDirPath + CypressSettings::readSetting("ultrasound/dicom/log_config").toString();
    const QString ascConfigPath = workingDirPath + CypressSettings::readSetting("ultrasound/dicom/asc_config").toString();

    m_dicomServer.reset(new DcmRecv(executablePath, ascConfigPath, storageDirPath, aeTitle, port));
    connect(m_dicomServer.get(), &DcmRecv::dicomFilesReceived, this, &VividiManager::dicomFilesReceived);

    m_dicomServer->start();
}

void VividiManager::dicomFilesReceived()
{
    QList<DicomFile> &files = m_dicomServer->receivedFiles;

    foreach (DicomFile file, files)
    {
        if (CypressSettings::isDebugMode())
            qDebug() << file.patientId << file.modality << file.studyDate << file.studyId << file.fileInfo.absoluteFilePath();
    }
}

bool VividiManager::isInstalled()
{
    return false;
}

void VividiManager::start()
{
    emit started(m_test.get());
    emit dataChanged(m_test.get());
    emit canMeasure();
}

void VividiManager::measure()
{
    m_test->reset();

    if (CypressSettings::isSimMode())
    {
        m_test->simulate();
        emit measured(m_test.get());
        emit canFinish();
    }
}

void VividiManager::cancel()
{
}

void VividiManager::finish()
{
    QJsonObject responseJson{};

    QString host = CypressSettings::getPineHost();
    QString endpoint = CypressSettings::getPineEndpoint();
    QString pine_path = CypressSettings::getAnswerUrl(m_session->getAnswerId());

    for (int i = 0; i < m_test->getMeasurementCount(); i++)
    {
        Measurement& measure = m_test->get(i);
        const QString &side = measure.getAttribute("side").toString();
        QByteArray data = FileUtils::readFile(measure.getAttribute("path").toString());

        sendHTTPSRequest("PATCH",
                         pine_path + "?filename=" + measure.getAttribute("name").toString(),
                         "application/octet-stream",
                         data);

        measure.removeAttribute("PATH");
    }

    QJsonObject testJson = m_test->toJsonObject();
    QJsonObject sessionObj = m_session->getJsonObject();

    testJson.insert("session", sessionObj);
    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    sendHTTPSRequest("PATCH", pine_path, "application/json", serializedData);

    emit success("Sent measurements to Pine, you can now close this window");
}


bool VividiManager::setUp()
{
    return true;
}

bool VividiManager::clearData()
{
    return true;
}

bool VividiManager::cleanUp()
{
    return true;
}
