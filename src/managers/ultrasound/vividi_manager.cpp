#include "vividi_manager.h"

#include "data/cimt_vivid_i_measurement.h"
#include "data/cimt_vivid_i_test.h"

#include "auxiliary/file_utils.h"
#include "auxiliary/windows_util.h"
#include "auxiliary/network_utils.h"

#include <QJsonDocument>
#include <QMessageBox>
#include <QApplication>


DeviceConfig VividiManager::config {{
    {"runnableName",    {"ultrasound/dicom/runnableName", Exe             }},
    {"runnablePath",    {"ultrasound/dicom/runnablePath", Dir             }},
    {"aeTitle",         {"ultrasound/dicom/aeTitle", 	  NonEmptyString  }},
    {"host",            {"ultrasound/dicom/host",         NonEmptyString  }},
    {"port", 			{"ultrasound/dicom/port",         NonEmptyString  }},
    {"storagePath",     {"ultrasound/dicom/storagePath",  Dir             }},
    {"logConfigPath",   {"ultrasound/dicom/log_config",   File            }},
    {"ascConfigPath",   {"ultrasound/dicom/asc_config",   File            }},
}};

VividiManager::VividiManager(QSharedPointer<UltrasoundSession> session)
    : ManagerBase(session)
{
    qInfo() << "VividiManager::VividiManager";

    m_test.reset(new CimtVividiTest);

    m_runnableName = config.getSetting("runnableName");
    m_runnablePath = config.getSetting("runnablePath");

    m_aeTitle = config.getSetting("aeTitle");
    m_host = config.getSetting("host");
    m_port = config.getSetting("port");

    m_storageDirPath = config.getSetting("storagePath");
    m_logConfigPath = config.getSetting("logConfigPath");
    m_ascConfigPath = config.getSetting("ascConfigPath");

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
    qInfo() << "VividiManager::dicomFilesReceived";

    bool foundInvalidParticipant = false;
    QString invalidId;

    foreach (DicomFile file, dicomFiles) {
        QSharedPointer<CimtVividIMeasurement> measure(new CimtVividIMeasurement);

        qDebug() << file.patientId
                 << file.modality
                 << file.studyDate
                 << file.studyId
                 << file.absFilePath
                 << file.mediaStorageUID;

        if (m_session->getBarcode() != file.patientId) {
            qDebug() << "Received wrong ID" << file.patientId;
            foundInvalidParticipant = true;
            invalidId = file.patientId;

            continue;
        }

        auto measurements = m_test->getMeasurements();
        bool foundDuplicate = false;
        for (auto measurement : measurements) {
            if (measurement->getAttributeValue("path").toString() == file.absFilePath) {
                foundDuplicate = true;
                qDebug() << "Found duplicate: " << file.absFilePath;
                break;
            }
        }

        if (foundDuplicate) {
            continue;
        }

        if (file.mediaStorageUID == UID_UltrasoundImageStorage) {
            qInfo() << "VividIManager::dicomFilesReceived: received still image "
                 << m_stillImageIdCounter;

            measure->setAttribute("name", "STILL_IMAGE_" + QString::number(m_stillImageIdCounter));
            measure->setAttribute("path", file.absFilePath);
            measure->setAttribute("study_id", file.studyId);
            measure->setAttribute("patient_id", file.patientId);
            measure->setAttribute("side", "");
            measure->setAttribute("size", FileUtils::getHumanReadableFileSize(file.absFilePath));

            ++m_stillImageIdCounter;

        } else if (file.mediaStorageUID == UID_UltrasoundMultiframeImageStorage) {
            qInfo() << "VividIManager::dicomFilesReceived: received cineloop image"
                 << m_cineloopCounter;

            measure->setAttribute("name", "CINELOOP_" + QString::number(m_cineloopCounter));
            measure->setAttribute("path", file.absFilePath);
            measure->setAttribute("study_id", file.studyId);
            measure->setAttribute("patient_id", file.patientId);
            measure->setAttribute("side", "");
            measure->setAttribute("size", FileUtils::getHumanReadableFileSize(file.absFilePath));

            ++m_cineloopCounter;

        } else if (file.modality == "SR") {
            qInfo() << "VividIManager::dicomFilesReceived: received SR file" << m_srCounter;

            measure->setAttribute("name", "SR_" + QString::number(m_srCounter));
            measure->setAttribute("path", file.absFilePath);
            measure->setAttribute("study_id", file.studyId);
            measure->setAttribute("patient_id", file.patientId);
            measure->setAttribute("side", "");
            measure->setAttribute("size", FileUtils::getHumanReadableFileSize(file.absFilePath));

            ++m_srCounter;
        } else {
            qInfo() << "VividIManager::dicomFilesReceived: found unknown dicom file";
            continue;
        }

        m_test->addMeasurement(measure);
    }

    if (foundInvalidParticipant)
        QMessageBox::warning(nullptr, "Invalid participant", invalidId + " does not match " + m_session->getBarcode());

    emit dataChanged(m_test);
    if (m_test->isValid())
        emit canFinish();
    else {
        emit cannotFinish();
    }
}

bool VividiManager::start()
{
    qInfo() << "VividiManager::start";

    m_dicomServer->start();

    emit started(m_test);
    emit dataChanged(m_test);
    emit canMeasure();

    return true;
}

void VividiManager::measure()
{
    qInfo() << "VividiManager::measure";

    if (m_test->isValid())
        emit canFinish();
    else
        emit cannotFinish();
}

void VividiManager::finish()
{
    qInfo() << "VividiManager::finish";

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
    qInfo() << "VividiManager::setUp";
    return true;
}

bool VividiManager::clearData()
{
    qInfo() << "VividiManager::clearData";

    m_test->reset();

    m_stillImageIdCounter = 1;
    m_cineloopCounter = 1;
    m_srCounter = 1;

    return true;
}

bool VividiManager::cleanUp()
{
    qInfo() << "VividiManager::cleanUp";

    return clearData();
}




