#include "vividi_manager.h"
#include "cypress_application.h"

#include "auxiliary/file_utils.h"

#include <QJsonDocument>

VividiManager::VividiManager(QSharedPointer<UltrasoundSession> session)
    : ManagerBase(session)
{
    m_test.reset(new CimtVividiTest);

    QDir workingDir = QDir::current();
    QString workingDirPath = workingDir.absolutePath() + "/";

    const QString executablePath = workingDirPath + CypressSettings::getInstance().readSetting("ultrasound/dicom/executable").toString();
    const QString aeTitle = CypressSettings::getInstance().readSetting("ultrasound/dicom/aeTitle").toString();
    const QString host = CypressSettings::getInstance().readSetting("ultrasound/dicom/host").toString();
    const QString port = CypressSettings::getInstance().readSetting("ultrasound/dicom/port").toString();

    const QString storageDirPath = workingDirPath + CypressSettings::getInstance().readSetting("ultrasound/dicom/storagePath").toString();
    const QString logConfigPath = workingDirPath + CypressSettings::getInstance().readSetting("ultrasound/dicom/log_config").toString();
    const QString ascConfigPath = workingDirPath + CypressSettings::getInstance().readSetting("ultrasound/dicom/asc_config").toString();


    m_dicomServer.reset(new DcmRecv(executablePath, ascConfigPath, storageDirPath, aeTitle, port));
    connect(m_dicomServer.get(), &DcmRecv::dicomFilesReceived, this, &VividiManager::dicomFilesReceived);

    m_dicomServer->start();
}

void VividiManager::dicomFilesReceived()
{
    QList<DicomFile> &files = m_dicomServer->receivedFiles;

    foreach (DicomFile file, files)
    {
        qDebug() << file.patientId << file.modality << file.studyDate << file.studyId << file.fileInfo.absoluteFilePath();
    }
}

bool VividiManager::isAvailable()
{
    return false;
}

bool VividiManager::isInstalled()
{
    return false;
}

void VividiManager::setInputData(const QVariantMap& inputData)
{
    m_inputData = inputData;
}

void VividiManager::start()
{
    emit started(m_test.get());
    emit canMeasure();
}

void VividiManager::measure()
{
    m_test->reset();

    if (Cypress::getInstance().isSimulation())
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

    QString host = CypressSettings::getInstance().getPineHost();
    QString endpoint = CypressSettings::getInstance().getPineEndpoint();
    QString pine_path = CypressSettings::getInstance().getAnswerUrl(m_session->getAnswerId());

    for (int i = 0; i < m_test->getMeasurementCount(); i++)
    {
        Measurement& measure = m_test->get(i);
        const QString &side = measure.getAttribute("side").toString();
        QByteArray data = FileUtils::readFileIntoByteArray(measure.getAttribute("path").toString());

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
