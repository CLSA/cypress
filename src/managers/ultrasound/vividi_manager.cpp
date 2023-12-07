#include "vividi_manager.h"

#include "data/cimt_vivid_i_measurement.h"
#include "data/cimt_vivid_i_test.h"

#include "auxiliary/file_utils.h"

#include <QJsonDocument>

VividiManager::VividiManager(QSharedPointer<UltrasoundSession> session)
    : ManagerBase(session)
{
    m_test.reset(new CimtVividiTest);
    m_test->setExpectedMeasurementCount(10);

    m_runnableName = CypressSettings::readSetting("ultrasound/dicom/runnableName").toString();
    m_runnablePath = CypressSettings::readSetting("ultrasound/dicom/runnablePath").toString();

    m_aeTitle = CypressSettings::readSetting("ultrasound/dicom/aeTitle").toString();
    m_host = CypressSettings::readSetting("ultrasound/dicom/host").toString();
    m_port = CypressSettings::readSetting("ultrasound/dicom/port").toString();

    m_storageDirPath = CypressSettings::readSetting("ultrasound/dicom/storagePath").toString();
    m_logConfigPath = CypressSettings::readSetting("ultrasound/dicom/log_config").toString();
    m_ascConfigPath = CypressSettings::readSetting("ultrasound/dicom/asc_config").toString();

    m_dicomServer.reset(
        new DcmRecv(m_runnableName, m_ascConfigPath, m_storageDirPath, m_aeTitle, m_port));

    connect(m_dicomServer.get(),
            &DcmRecv::dicomFilesReceived,
            this,
            &VividiManager::dicomFilesReceived);
}

VividiManager::~VividiManager()
{
    m_dicomServer->stop();
}

void VividiManager::dicomFilesReceived()
{
    QList<DicomFile> &files = m_dicomServer->receivedFiles;

    foreach (DicomFile file, files)
    {
        QSharedPointer<CimtVividIMeasurement> measure(new CimtVividIMeasurement);

        if (m_debug)
            qDebug() << file.patientId << file.modality << file.studyDate << file.studyId << file.fileInfo.absoluteFilePath();

        if (file.mediaStorageUID == UID_UltrasoundImageStorage) {
            if (m_debug) {
                qDebug() << "VividIManager::dicomFilesReceived: received still image "
                         << m_stillImageIdCounter;
            }

            measure->setAttribute("name", "STILL_IMAGE_" + QString::number(m_stillImageIdCounter));
            measure->setAttribute("path", file.fileInfo.absoluteFilePath());
            measure->setAttribute("side", "");

            ++m_stillImageIdCounter;

        } else if (file.mediaStorageUID == UID_UltrasoundMultiframeImageStorage) {
            if (m_debug) {
                qDebug() << "VividIManager::dicomFilesReceived: received cineloop image"
                         << m_cineloopCounter;
            }

            measure->setAttribute("name", "CINELOOP_" + QString::number(m_cineloopCounter));
            measure->setAttribute("path", file.fileInfo.absoluteFilePath());
            measure->setAttribute("side", "");

            ++m_cineloopCounter;

        } else if (file.modality == "SR") {
            if (m_debug) {
                qDebug() << "VividIManager::dicomFilesReceived: received SR file" << m_srCounter;
            }

            measure->setAttribute("name", "SR_" + QString::number(m_srCounter));
            measure->setAttribute("path", file.fileInfo.absoluteFilePath());
            measure->setAttribute("side", "");

            ++m_srCounter;
        } else {
            qDebug() << "found unknown dicom file";
            continue;
        }

        m_test->addMeasurement(measure);
    }
}

bool VividiManager::isInstalled()
{
    QString runnableName = CypressSettings::readSetting("ultrasound/dicom/runnableName").toString();
    QString runnablePath = CypressSettings::readSetting("ultrasound/dicom/runnablePath").toString();

    QString aeTitle = CypressSettings::readSetting("ultrasound/dicom/aeTitle").toString();
    QString host = CypressSettings::readSetting("ultrasound/dicom/host").toString();
    QString port = CypressSettings::readSetting("ultrasound/dicom/port").toString();

    QString storageDirPath = CypressSettings::readSetting("ultrasound/dicom/storagePath").toString();
    QString logConfigPath = CypressSettings::readSetting("ultrasound/dicom/log_config").toString();
    QString ascConfigPath = CypressSettings::readSetting("ultrasound/dicom/asc_config").toString();

    return true;
}

void VividiManager::start()
{
    m_dicomServer->start();

    emit started(m_test.get());
    emit dataChanged(m_test.get());
    emit canMeasure();
}

void VividiManager::measure()
{
    if (CypressSettings::isSimMode())
    {
        m_test->simulate();

        emit dataChanged(m_test.get());
        emit canFinish();
    }
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
    if (m_debug) {
        qDebug() << "VividiManager::cleanUp";
    }

    return true;
}

bool VividiManager::clearData()
{
    if (m_debug) {
        qDebug() << "VividiManager::clearData";
    }

    m_test->reset();

    m_stillImageIdCounter = 1;
    m_cineloopCounter = 1;
    m_srCounter = 1;

    return true;
}

bool VividiManager::cleanUp()
{
    if (m_debug) {
        qDebug() << "VividiManager::cleanUp";
    }

    return clearData();
}
