#include "vividi_manager.h"

#include "data/cimt_vivid_i_measurement.h"
#include "data/cimt_vivid_i_test.h"

#include "auxiliary/file_utils.h"
#include "auxiliary/windows_util.h"
#include "auxiliary/network_utils.h"

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
    foreach (DicomFile file, dicomFiles) {
        QSharedPointer<CimtVividIMeasurement> measure(new CimtVividIMeasurement);

        if (m_debug)
            qDebug() << file.patientId << file.modality << file.studyDate << file.studyId
                     << file.absFilePath << file.mediaStorageUID;

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

        //measure->setAttribute("name", "STILL_IMAGE_" + QString::number(m_stillImageIdCounter));
        //measure->setAttribute("path", file.absFilePath);
        //measure->setAttribute("study_id", file.studyId);
        //measure->setAttribute("patient_id", file.patientId);
        //measure->setAttribute("side", "");

        if (file.mediaStorageUID == UID_UltrasoundImageStorage) {
            if (m_debug) {
                qDebug() << "VividIManager::dicomFilesReceived: received still image "
                         << m_stillImageIdCounter;
            }

            measure->setAttribute("name", "STILL_IMAGE_" + QString::number(m_stillImageIdCounter));
            measure->setAttribute("path", file.absFilePath);
            measure->setAttribute("study_id", file.studyId);
            measure->setAttribute("patient_id", file.patientId);
            measure->setAttribute("side", "");

            ++m_stillImageIdCounter;

        } else if (file.mediaStorageUID == UID_UltrasoundMultiframeImageStorage) {
            if (m_debug) {
                qDebug() << "VividIManager::dicomFilesReceived: received cineloop image"
                         << m_cineloopCounter;
            }

            measure->setAttribute("name", "CINELOOP_" + QString::number(m_cineloopCounter));
            measure->setAttribute("path", file.absFilePath);
            measure->setAttribute("study_id", file.studyId);
            measure->setAttribute("patient_id", file.patientId);
            measure->setAttribute("side", "");

            ++m_cineloopCounter;

        } else if (file.modality == "SR") {
            if (m_debug) {
                qDebug() << "VividIManager::dicomFilesReceived: received SR file" << m_srCounter;
            }

            measure->setAttribute("name", "SR_" + QString::number(m_srCounter));
            measure->setAttribute("path", file.absFilePath);
            measure->setAttribute("study_id", file.studyId);
            measure->setAttribute("patient_id", file.patientId);
            measure->setAttribute("side", "");

            ++m_srCounter;
        } else {
            qDebug() << "found unknown dicom file";
            continue;
        }

        m_test->addMeasurement(measure);
    }

    emit dataChanged(m_test);
}

bool VividiManager::isInstalled()
{
    const bool isDebugMode = CypressSettings::isDebugMode();

    const QString runnableName = CypressSettings::readSetting("dxa/dicom/runnableName").toString();
    const QString runnablePath = CypressSettings::readSetting("dxa/dicom/runnablePath").toString();
    const QString aeTitle = CypressSettings::readSetting("dxa/dicom/aeTitle").toString();
    const QString host = CypressSettings::readSetting("dxa/dicom/host").toString();
    const QString port = CypressSettings::readSetting("dxa/dicom/port").toString();

    const QString storageDirPath = CypressSettings::readSetting("dxa/dicom/storagePath").toString();
    const QString logConfigPath = CypressSettings::readSetting("dxa/dicom/log_config").toString();
    const QString ascConfigPath = CypressSettings::readSetting("dxa/dicom/asc_config").toString();

    if (runnableName.isNull() || runnableName.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - runnableName is not defined";
        return false;
    }

    if (runnablePath.isNull() || runnablePath.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - runnablePath is not defined";
        return false;
    }

    if (aeTitle.isNull() || aeTitle.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - aeTitle is not defined";
        return false;
    }

    if (host.isNull() || host.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - host is not defined";
        return false;
    }

    if (port.isNull() || port.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - port is not defined";
        return false;
    }

    if (storageDirPath.isNull() || storageDirPath.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - storageDirPath is not defined";
        return false;
    }

    if (logConfigPath.isNull() || logConfigPath.isNull()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - logConfigPath is not defined";
        return false;
    }

    if (ascConfigPath.isNull() || ascConfigPath.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - ascConfigPath is not defined";
        return false;
    }

    const QFileInfo exeInfo(runnableName);
    if (!exeInfo.exists()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - runnableName does not exist at"
                     << runnableName;
        return false;
    }
    if (!exeInfo.isExecutable()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - runnableName is not executable at"
                     << runnableName;
        return false;
    }

    const QFileInfo workingDir(runnablePath);
    if (!workingDir.exists()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - working directory does not exist at"
                     << workingDir;
        return false;
    }
    if (!workingDir.isDir()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - working directory is not writable at"
                     << workingDir;
        return false;
    }
    if (!workingDir.isWritable()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - working directory is not writable at"
                     << workingDir;
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
    if (CypressSettings::isSimMode())
    {
        m_test->simulate();

        emit dataChanged(m_test);
        emit canFinish();
    }

    if (m_test->isValid()) {
        emit canFinish();
    }
}

void VividiManager::finish()
{

    const QString host = CypressSettings::getPineHost();
    const QString endpoint = CypressSettings::getPineEndpoint();
    const QString pine_path = CypressSettings::getAnswerUrl(m_session->getAnswerId());

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

        bool ok = NetworkUtils::sendHTTPSRequest("PATCH",
                                   (pine_path
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

    bool ok = NetworkUtils::sendHTTPSRequest("PATCH", pine_path.toStdString(), "application/json", serializedData);
    if (!ok) {
        emit error("Could not send results");
        return;
    }
    emit success("Sent measurements to Pine, you may now close this window");
}


bool VividiManager::setUp()
{
    if (m_debug)
        qDebug() << "VividiManager::cleanUp";

    return true;
}

bool VividiManager::clearData()
{
    if (m_debug)
        qDebug() << "VividiManager::clearData";

    m_test->reset();

    m_stillImageIdCounter = 1;
    m_cineloopCounter = 1;
    m_srCounter = 1;

    return true;
}

bool VividiManager::cleanUp()
{
    if (m_debug)
        qDebug() << "VividiManager::cleanUp";

    return clearData();
}




