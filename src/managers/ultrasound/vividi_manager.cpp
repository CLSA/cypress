#include "vividi_manager.h"

#include "data/cimt_vivid_i_measurement.h"
#include "data/cimt_vivid_i_test.h"

#include "auxiliary/file_utils.h"
#include "auxiliary/windows_util.h"
#include "auxiliary/network_utils.h"

#include <QJsonDocument>
#include <QMessageBox>
#include <QApplication>


VividiManager::VividiManager(QSharedPointer<UltrasoundSession> session)
    : ManagerBase(session)
{
    m_test.reset(new CimtVividiTest);
    //m_test->setExpectedMeasurementCount(10);

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

    WindowsUtil::killProcessByName(L"storescp.exe");

    connect(m_dicomServer.get(),
            &DcmRecv::dicomFilesReceived,
            this,
            &VividiManager::dicomFilesReceived);
}

VividiManager::~VividiManager()
{
    m_dicomServer->stop();
}

void VividiManager::dicomFilesReceived(QList<DicomFile> dicomFiles)
{
    emit dataChanged(m_test);

    bool invalidFiles = false;
    QString incorrectPatientId;

    foreach (DicomFile file, dicomFiles) {
        QSharedPointer<CimtVividIMeasurement> measure(new CimtVividIMeasurement);

        qDebug() << file.patientId
                 << file.modality
                 << file.studyDate
                 << file.studyId
                 << file.absFilePath
                 << file.mediaStorageUID;

        if (m_session->getBarcode() != file.patientId) {
            invalidFiles = true;
            incorrectPatientId = file.patientId;
            break;
        }

        auto measurements = m_test->getMeasurements();
        bool foundDuplicate = false;
        for (auto measurement : measurements) {
            if (measurement->getAttributeValue("path").toString() == file.absFilePath) {
                foundDuplicate = true;
                break;
            }
        }

        if (foundDuplicate)
            continue;

        if (file.mediaStorageUID == UID_UltrasoundImageStorage) {
            qDebug() << "VividIManager::dicomFilesReceived: received still image "
                 << m_stillImageIdCounter;

            measure->setAttribute("name", "STILL_IMAGE_" + QString::number(m_stillImageIdCounter));
            measure->setAttribute("path", file.absFilePath);
            measure->setAttribute("study_id", file.studyId);
            measure->setAttribute("patient_id", file.patientId);
            measure->setAttribute("side", "");
            measure->setAttribute("size", FileUtils::getHumanReadableFileSize(file.absFilePath));

            ++m_stillImageIdCounter;

        } else if (file.mediaStorageUID == UID_UltrasoundMultiframeImageStorage) {
            qDebug() << "VividIManager::dicomFilesReceived: received cineloop image"
                 << m_cineloopCounter;

            measure->setAttribute("name", "CINELOOP_" + QString::number(m_cineloopCounter));
            measure->setAttribute("path", file.absFilePath);
            measure->setAttribute("study_id", file.studyId);
            measure->setAttribute("patient_id", file.patientId);
            measure->setAttribute("side", "");
            measure->setAttribute("size", FileUtils::getHumanReadableFileSize(file.absFilePath));

            ++m_cineloopCounter;

        } else if (file.modality == "SR") {
            qDebug() << "VividIManager::dicomFilesReceived: received SR file" << m_srCounter;

            measure->setAttribute("name", "SR_" + QString::number(m_srCounter));
            measure->setAttribute("path", file.absFilePath);
            measure->setAttribute("study_id", file.studyId);
            measure->setAttribute("patient_id", file.patientId);
            measure->setAttribute("side", "");
            measure->setAttribute("size", FileUtils::getHumanReadableFileSize(file.absFilePath));

            ++m_srCounter;
        } else {
            qDebug() << "found unknown dicom file";
            continue;
        }

        m_test->addMeasurement(measure);
    }

    if (invalidFiles) {
        QMessageBox::warning(nullptr, "Incorrect participant",
                             QString("Received ID %1 which does not match %2")
                                 .arg(incorrectPatientId, m_session->getBarcode()));

        m_test->reset();
        emit dataChanged(m_test);
        return;
    }

    emit dataChanged(m_test);
    if (m_test->isValid()) {
        emit canFinish();
    }

}

bool VividiManager::isInstalled()
{
    qDebug() << "UltrasoundManager::isInstalled";

    if (CypressSettings::isSimMode())
        return true;

    const QString runnableName = CypressSettings::readSetting("ultrasound/dicom/runnableName").toString();
    const QString runnablePath = CypressSettings::readSetting("ultrasound/dicom/runnablePath").toString();
    const QString aeTitle = CypressSettings::readSetting("ultrasound/dicom/aeTitle").toString();
    const QString host = CypressSettings::readSetting("ultrasound/dicom/host").toString();
    const QString port = CypressSettings::readSetting("ultrasound/dicom/port").toString();

    const QString storageDirPath = CypressSettings::readSetting("ultrasound/dicom/storagePath").toString();
    const QString logConfigPath = CypressSettings::readSetting("ultrasound/dicom/log_config").toString();
    const QString ascConfigPath = CypressSettings::readSetting("ultrasound/dicom/asc_config").toString();

    if (runnableName.isNull() || runnableName.isEmpty()) {
        qDebug() << "runnableName is not defined";
        return false;
    }

    if (runnablePath.isNull() || runnablePath.isEmpty()) {
        qDebug() << "runnablePath is not defined";
        return false;
    }

    if (aeTitle.isNull() || aeTitle.isEmpty()) {
        qDebug() << "aeTitle is not defined";
        return false;
    }

    if (host.isNull() || host.isEmpty()) {
        qDebug() << "host is not defined";
        return false;
    }

    if (port.isNull() || port.isEmpty()) {
        qDebug() << "port is not defined";
        return false;
    }

    if (storageDirPath.isNull() || storageDirPath.isEmpty()) {
        qDebug() << "storageDirPath is not defined";
        return false;
    }

    if (logConfigPath.isNull() || logConfigPath.isNull()) {
        qDebug() << "logConfigPath is not defined";
        return false;
    }

    if (ascConfigPath.isNull() || ascConfigPath.isEmpty()) {
        qDebug() << "ascConfigPath is not defined";
        return false;
    }

    const QFileInfo exeInfo(runnableName);
    if (!exeInfo.exists()) {
        qDebug() << "runnableName does not exist at" << runnableName;
        return false;
    }
    if (!exeInfo.isExecutable()) {
        qDebug() << "runnableName is not executable at" << runnableName;
        return false;
    }

    const QFileInfo workingDir(runnablePath);
    if (!workingDir.exists()) {
        qDebug() << "working directory does not exist at" << workingDir;
        return false;
    }
    if (!workingDir.isDir()) {
        qDebug() << "working directory is not writable at" << workingDir;
        return false;
    }
    if (!workingDir.isWritable()) {
        qDebug() << "working directory is not writable at" << workingDir;
        return false;
    }

    return true;
}

bool VividiManager::start()
{
    m_dicomServer->start();

    emit started(m_test);
    emit dataChanged(m_test);
    emit canMeasure();

    return true;
}

void VividiManager::measure()
{
    if (m_test->isValid())
        emit canFinish();
    else
        emit cannotFinish();
}

void VividiManager::finish()
{
    qDebug() << "VividiManager::finish";
    qDebug() << m_test->toJsonObject();

    if (!m_test->isValid())
    {
        QMessageBox::warning(nullptr, "Incomplete", "Please ensure the side has been selected for all files");
        return;
    }

    const int answer_id = m_session->getAnswerId();
    const QString pineOrigin = m_session->getOrigin();
    const QString answerUrl = pineOrigin + "/answer/" + QString::number(answer_id);

    QJsonObject responseJson{};
    QJsonObject filesJson {};

    for (int i = 0; i < m_test->getMeasurementCount(); i++)
    {
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


bool VividiManager::setUp()
{
    qDebug() << "VividiManager::cleanUp";
    return true;
}

bool VividiManager::clearData()
{
    qDebug() << "VividiManager::clearData";

    m_test->reset();

    m_stillImageIdCounter = 1;
    m_cineloopCounter = 1;
    m_srCounter = 1;

    return true;
}

bool VividiManager::cleanUp()
{
    qDebug() << "VividiManager::cleanUp";
    return clearData();
}




