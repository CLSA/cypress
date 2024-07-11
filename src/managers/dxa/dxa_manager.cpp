#include "cypress_session.h"

#include "auxiliary/file_utils.h"
#include "auxiliary/windows_util.h"
#include "auxiliary/network_utils.h"

#include "data/dxa/tests/dxa_test.h"
#include "dicom/dcm_recv.h"
#include "managers/dxa/dxa_manager.h"

#include "data/dxa/measurements/dxa_measurement.h"

#include <QApplication>
#include <QException>
#include <QJsonDocument>
#include <QMap>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QString>
#include <QVariant>
#include <QSqlQuery>


DXAManager::DXAManager(QSharedPointer<DXASession> session)
    : ManagerBase(session)
{
    m_test.reset(new DXATest);

    m_runnableName = CypressSettings::readSetting("dxa/dicom/runnableName").toString();
    m_runnablePath = CypressSettings::readSetting("dxa/dicom/runnablePath").toString();

    m_aeTitle = CypressSettings::readSetting("dxa/dicom/aeTitle").toString();
    m_host = CypressSettings::readSetting("dxa/dicom/host").toString();
    m_port = CypressSettings::readSetting("dxa/dicom/port").toString();

    m_storageDirPath = CypressSettings::readSetting("dxa/dicom/storagePath").toString();
    m_logConfigPath = CypressSettings::readSetting("dxa/dicom/log_config").toString();
    m_ascConfigPath = CypressSettings::readSetting("dxa/dicom/asc_config").toString();

    m_patscanDbPath = CypressSettings::readSetting("dxa/patscanDbPath").toString();
    m_refscanDbPath = CypressSettings::readSetting("dxa/refscanDbPath").toString();

    WindowsUtil::killProcessByName(L"storescp.exe");

    qInfo() << "DXAManager::DXAManager";
    qInfo() << session->getSessionId();
    qInfo() << session->getBarcode();
    qInfo() << session->getInterviewer();
    qInfo() << session->getInputData();
}

DXAManager::~DXAManager()
{
    m_dicomServer->stop();
}

bool DXAManager::isInstalled() {
    if (CypressSettings::isSimMode())
        return true;

    const QString runnableName = CypressSettings::readSetting("dxa/dicom/runnableName").toString();
    const QString runnablePath = CypressSettings::readSetting("dxa/dicom/runnablePath").toString();
    const QString aeTitle = CypressSettings::readSetting("dxa/dicom/aeTitle").toString();
    const QString host = CypressSettings::readSetting("dxa/dicom/host").toString();
    const QString port = CypressSettings::readSetting("dxa/dicom/port").toString();

    const QString storageDirPath = CypressSettings::readSetting("dxa/dicom/storagePath").toString();
    const QString logConfigPath = CypressSettings::readSetting("dxa/dicom/log_config").toString();
    const QString ascConfigPath = CypressSettings::readSetting("dxa/dicom/asc_config").toString();

    const QString patscanDbPath = CypressSettings::readSetting("dxa/patscanDbPath").toString();
    const QString refscanDbPath = CypressSettings::readSetting("dxa/refscanDbPath").toString();

    if (runnableName.isNull() || runnableName.isEmpty()) {
        qInfo() << "DXAManager::isInstalled: runnableName is not defined";
        return false;
    }

    if (runnablePath.isNull() || runnablePath.isEmpty()) {
        qInfo() << "DXAManager::isInstalled: runnablePath is not defined";
        return false;
    }

    if (aeTitle.isNull() || aeTitle.isEmpty()) {
        qInfo() << "DXAManager::isInstalled: aeTitle is not defined";
        return false;
    }

    if (host.isNull() || host.isEmpty()) {
        qInfo() << "DXAManager::isInstalled: host is not defined";
        return false;
    }

    if (port.isNull() || port.isEmpty()) {
        qInfo() << "DXAManager::isInstalled: port is not defined";
        return false;
    }

    if (storageDirPath.isNull() || storageDirPath.isEmpty()) {
        qInfo() << "DXAManager::isInstalled: storageDirPath is not defined";
        return false;
    }

    if (logConfigPath.isNull() || logConfigPath.isNull()) {
        qInfo() << "DXAManager::isInstalled: logConfigPath is not defined";
        return false;
    }

    if (ascConfigPath.isNull() || ascConfigPath.isEmpty()) {
        qInfo() << "DXAManager::isInstalled: ascConfigPath is not defined";
        return false;
    }

    if (patscanDbPath.isNull() || patscanDbPath.isEmpty()) {
        qInfo() << "DXAManager::isInstalled: patscanDbPath is not defined";
        return false;
    }

    const QFileInfo patscanFile(patscanDbPath);
    if (!patscanFile.exists()) {
        qInfo() << "DXAManager::isInstalled: patscan file does not exist: " << patscanDbPath;
        return false;
    }

    if (!patscanFile.isFile()) {
        qInfo() << "DXAManager::isInstalled: patscan file is not a file at" << patscanDbPath;
        return false;
    }

    if (!patscanFile.isReadable()) {
        qInfo() << "DXAManager::isInstalled: patscan file is not readable at" << patscanDbPath;
        return false;
    }

    if (refscanDbPath.isNull() || refscanDbPath.isEmpty()) {
        qInfo() << "DXAManager::isInstalled: refscanDbPath is not defined at" << refscanDbPath;
        return false;
    }

    const QFileInfo refscanFile(refscanDbPath);
    if (!refscanFile.exists()) {
        qInfo() << "DXAManager::isInstalled: refscanDbPath is not defined at" << refscanDbPath;
        return false;
    }
    if (!refscanFile.isFile()) {
        qInfo() << "DXAManager::isInstalled: refscanDbPath is not a file at" << refscanDbPath;
        return false;
    }
    if (!refscanFile.isReadable()) {
        qInfo() << "DXAManager::isInstalled: refscanDbPath is not readable at" << refscanDbPath;
        return false;
    }

    const QFileInfo exeInfo(runnableName);
    if (!exeInfo.exists()) {
        qInfo() << "DXAManager::isInstalled: runnableName does not exist at" << runnableName;
        return false;
    }
    if (!exeInfo.isExecutable()) {
        qInfo() << "DXAManager::isInstalled: runnableName is not executable at" << runnableName;
        return false;
    }

    const QFileInfo workingDir(runnablePath);
    if (!workingDir.exists()) {
        qInfo() << "DXAManager::isInstalled: working directory does not exist at" << workingDir;
        return false;
    }
    if (!workingDir.isDir()) {
        qInfo() << "DXAManager::isInstalled: working directory is not writable at" << workingDir;
        return false;
    }
    if (!workingDir.isWritable()) {
        qInfo() << "DXAManager::isInstalled: working directory is not writable at" << workingDir;
        return false;
    }

    return true;
}

// Set up device
bool DXAManager::setUp()
{
    m_dicomServer.reset(
        new DcmRecv(m_runnableName, m_ascConfigPath, m_storageDirPath, m_aeTitle, m_port));

    connect(m_dicomServer.get(), &DcmRecv::running, this, [=]() {
        qDebug() << "DxaManager::running - DICOM server is running...";
    });

    connect(m_dicomServer.get(),
            &DcmRecv::dicomFilesReceived,
            this,
            &DXAManager::dicomFilesReceived);

    return true;
}

// what the manager does in response to the main application
// window invoking its run method
//
bool DXAManager::start()
{
    if (!setUp())
        return false;

    m_dicomServer->start();

    emit started(m_test);
    emit dataChanged(m_test);

    return true;
}

void DXAManager::dicomFilesReceived(QList<DicomFile> dicomFiles)
{
    // pass received dicom files to test class
    auto test = qSharedPointerCast<DXATest>(m_test);
    auto session = qSharedPointerCast<DXASession>(m_session);

    int filesReceived = test->fromDicomFiles(dicomFiles, *session);

    emit status(QString("Received %1 file(s)").arg(filesReceived));

    emit canMeasure();
    emit dataChanged(m_test);
}

// retrieve a measurement from the device
//
void DXAManager::measure()
{
    auto test = qSharedPointerCast<DXATest>(m_test);
    if (!test->hasAllNeededFiles()) {
        QMessageBox::warning(nullptr, "Warning", "Have not received all images from Hologic Apex");
        return;
    }

    // TODO: file copy logic should be in its own thread to prevent event loop blocking
    emit status("Copying files from DEXA (1/2)");
    QApplication::processEvents();

    if (!initPatScanDb()) {
        emit error("Failed to copy the PatScan.mdb file from the Apex workstation");
        return;
    }

    emit status("Copying files from DEXA (2/2)");
    QApplication::processEvents();

    if (!initReferenceDb()) {
        emit error("Failed to copy the reference.mdb file from the Apex workstation");
        return;
    }

    emit status("Gathering variables");
    QApplication::processEvents();

    test->getPatientScan(m_patscanDb, m_session->getBarcode());
    QString patientKey = test->getMetaData("PATIENT_KEY").toString();
    QJsonObject patientData {
        { "PATIENT_KEY", patientKey },
        { "BIRTHDATE",   test->getMetaData("BIRTHDATE").toString() },
        { "SEX",         test->getMetaData("SEX").toString() },
        { "ETHNICITY",   test->getMetaData("ETHNICITY").toString() }
    };

    test->getScanAnalysisData(m_patscanDb, m_referenceDb, patientData);

    m_patscanDb.close();
    m_referenceDb.close();

    emit canFinish();
    emit status("Ready to submit");
}

// implementation of final clean up of device after disconnecting and all
// data has been retrieved and processed by any upstream classes
//
void DXAManager::finish()
{
    auto test = qSharedPointerCast<DXATest>(m_test);

    const int answer_id = m_session->getAnswerId();
    const QString barcode = m_session->getBarcode();

    const QString pineOrigin = m_session->getOrigin();
    const QString answerUrl = pineOrigin + "/answer/" + QString::number(answer_id);

    QJsonObject files {};

    foreach (auto measure, test->getMeasurements()) {
        const auto dxaMeasure = qSharedPointerDynamicCast<DXAMeasurement>(measure);

        const QString name = dxaMeasure->getAttribute("NAME").toString();
        const QString path = dxaMeasure->dicomFile.absFilePath;
        const QString fileName = dxaMeasure->dicomFile.fileName;

        if (!path.isEmpty()) {
            files.insert(name + "_dcm", dxaMeasure->dicomFile.size);

            const QByteArray data = FileUtils::readFile(path);

            bool ok = NetworkUtils::sendHTTPSRequest(
                "PATCH", (answerUrl + "?filename=" + fileName).toStdString(),
                "application/octet-stream",
                data
            );

            if (!ok) {
                qDebug() << "error transmitting file: " << fileName;
                return;
            }
        }
    }

    QJsonObject testJson = m_test->toJsonObject();
    QJsonObject sessionObj = m_session->getJsonObject();
    QJsonObject metadata = m_test->getMetaData().toJsonObject();

    testJson.insert("session", sessionObj);
    testJson.insert("files", files);
    testJson.insert("metadata", metadata);

    QJsonObject responseJson = {};
    responseJson.insert("value", testJson);

    const QJsonDocument jsonDoc(responseJson);
    const QByteArray serializedData = jsonDoc.toJson();

    bool ok = false;
    ok = NetworkUtils::sendHTTPSRequest("PATCH",
                          answerUrl.toStdString(),
                          "application/json",
                          serializedData);
    if (!ok) {
        qDebug() << "error transmitting results";
        return;
    }

    QString jsonString = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << jsonString.toStdString().c_str();

    emit success("Success: files saved to Pine");
    cleanUp();
}

bool DXAManager::cleanUp()
{
    return clearData();
}

bool DXAManager::clearData()
{
    m_test->reset();
    emit dataChanged(m_test);

    return true;
}

bool DXAManager::initPatScanDb() {
    if (!copyPatScanDb())
        return false;

    // calculate totals and averages
    m_patscanDb = QSqlDatabase::addDatabase("QODBC", "patscan");
    m_patscanDb.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};DBQ="
                              + QDir::toNativeSeparators(m_patscanDbFileInfo.absoluteFilePath()));
    if (!m_patscanDb.open())
        return false;

    return true;
}

bool DXAManager::initReferenceDb() {
    if (!copyReferenceDb())
        return false;

    m_referenceDb = QSqlDatabase::addDatabase("QODBC", "reference");
    m_referenceDb.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};DBQ="
                                + QDir::toNativeSeparators(m_referenceDbFileInfo.absoluteFilePath()));
    if (!m_referenceDb.open())
        return false;

    return true;
}

bool DXAManager::copyPatScanDb() {
    // Copies the PatScan.mdb file from the DEXA/APEX workstation and sets up a reference to the file.
    // Assumes that the Apex workstation is sharing the file

    // Returns true if successful, otherwise false
    const QFileInfo patscanFileInfo(m_patscanDbPath);
    if (!patscanFileInfo.exists()) {
        qDebug() << "patscan file does not exist";
        return false;
    }

    if (!patscanFileInfo.isReadable()) {
        qDebug() << "patscan file is not readable";
        return false;
    }

    const QFileInfo localPatScanFileInfo(QDir::current().absoluteFilePath(patscanFileInfo.fileName()));
    qDebug() << "local pat scan location" << localPatScanFileInfo.absoluteFilePath();
    if (localPatScanFileInfo.exists()) {
        if (!QFile::remove(localPatScanFileInfo.absoluteFilePath()))
            return false;
    }

    if (!QFile::copy(m_patscanDbPath, localPatScanFileInfo.absoluteFilePath())) {
        qDebug() << "error copying patscan db from " << m_refscanDbPath << "to" << localPatScanFileInfo.absoluteFilePath();
        return false;
    }

    m_patscanDbFileInfo = localPatScanFileInfo;

    return true;
}


bool DXAManager::copyReferenceDb() {
    // Copies the reference.mdb file from the DEXA computer, assumes that the Apex workstation is sharing the file
    //
    const QFileInfo apexReferenceFileInfo(m_refscanDbPath);
    if (!apexReferenceFileInfo.exists())
        return false;

    if (!apexReferenceFileInfo.isReadable())
        return false;

    const QFileInfo localReferenceFileInfo(QDir::current().absoluteFilePath(apexReferenceFileInfo.fileName()));
    if (localReferenceFileInfo.exists()) {
        if (!QFile::remove(localReferenceFileInfo.absoluteFilePath())) {
            qDebug() << "could not remove existing local reference db at: " << m_refscanDbPath;
            return false;
        }
    }

    if (!QFile::copy(m_refscanDbPath, localReferenceFileInfo.absoluteFilePath())) {
        qDebug() << "error copying refscan db from " << m_refscanDbPath << "to" << localReferenceFileInfo.absoluteFilePath();
        return false;
    }

    m_referenceDbFileInfo = localReferenceFileInfo;

    return true;
}
