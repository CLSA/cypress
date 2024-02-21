#include "cypress_session.h"

#include "auxiliary/file_utils.h"
#include "auxiliary/windows_util.h"
#include "auxiliary/network_utils.h"

#include "data/dxa/tests/dxa_test.h"
#include "dicom/dcm_recv.h"
#include "managers/dxa/dxa_manager.h"

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

    if (m_debug) {
        qDebug() << "DXAManager";

        qDebug() << session->getSessionId();
        qDebug() << session->getBarcode();
        qDebug() << session->getInterviewer();

        qDebug() << session->getInputData();
    }
}

DXAManager::~DXAManager()
{
    m_dicomServer->stop();
}

bool DXAManager::isInstalled()
{
    bool isDebugMode = CypressSettings::isDebugMode();
    bool isSimMode = CypressSettings::isSimMode();

    if (isSimMode)
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
        if (isDebugMode)
            qDebug() << "DXAManager::isInstalled - runnableName is not defined";
        return false;
    }

    if (runnablePath.isNull() || runnablePath.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DXAManager::isInstalled - runnablePath is not defined";
        return false;
    }

    if (aeTitle.isNull() || aeTitle.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DXAManager::isInstalled - aeTitle is not defined";
        return false;
    }

    if (host.isNull() || host.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DXAManager::isInstalled - host is not defined";
        return false;
    }

    if (port.isNull() || port.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DXAManager::isInstalled - port is not defined";
        return false;
    }

    if (storageDirPath.isNull() || storageDirPath.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DXAManager::isInstalled - storageDirPath is not defined";
        return false;
    }

    if (logConfigPath.isNull() || logConfigPath.isNull()) {
        if (isDebugMode)
            qDebug() << "DXAManager::isInstalled - logConfigPath is not defined";
        return false;
    }

    if (ascConfigPath.isNull() || ascConfigPath.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DXAManager::isInstalled - ascConfigPath is not defined";
        return false;
    }

    if (patscanDbPath.isNull() || patscanDbPath.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DXAManager::isInstalled - patscanDbPath is not defined";

        return false;
    }

    const QFileInfo patscanFile(patscanDbPath);
    if (!patscanFile.exists()) {
        if (isDebugMode)
            qDebug() << "DXAManager::isInstalled - patscan file does not exist: " << patscanDbPath;
        return false;
    }

    if (!patscanFile.isFile()) {
        if (isDebugMode)
            qDebug() << "DXAManager::isInstalled - patscan file is not a file at" << patscanDbPath;
        return false;
    }

    if (!patscanFile.isReadable()) {
        if (isDebugMode)
            qDebug() << "DXAManager::isInstalled - patscan file is not readable at "
                     << patscanDbPath;
        return false;
    }

    if (refscanDbPath.isNull() || refscanDbPath.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DXAManager::isInstalled - refscanDbPath is not defined at "
                     << refscanDbPath;
        return false;
    }

    const QFileInfo refscanFile(refscanDbPath);
    if (!refscanFile.exists()) {
        if (isDebugMode)
            qDebug() << "DXAManager::isInstalled - refscanDbPath is not defined at "
                     << refscanDbPath;
        return false;
    }
    if (!refscanFile.isFile()) {
        if (isDebugMode)
            qDebug() << "DXAManager::isInstalled - refscanDbPath is not a file at "
                     << refscanDbPath;
        return false;
    }
    if (!refscanFile.isReadable()) {
        if (isDebugMode)
            qDebug() << "DXAManager::isInstalled - refscanDbPath is not readable at "
                     << refscanDbPath;
        return false;
    }

    const QFileInfo exeInfo(runnableName);
    if (!exeInfo.exists()) {
        if (isDebugMode)
            qDebug() << "DXAManager::isInstalled - runnableName does not exist at" << runnableName;
        return false;
    }
    if (!exeInfo.isExecutable()) {
        if (isDebugMode)
            qDebug() << "DXAManager::isInstalled - runnableName is not executable at"
                     << runnableName;
        return false;
    }

    const QFileInfo workingDir(runnablePath);
    if (!workingDir.exists()) {
        if (isDebugMode)
            qDebug() << "DXAManager::isInstalled - working directory does not exist at"
                     << workingDir;
        return false;
    }
    if (!workingDir.isDir()) {
        if (isDebugMode)
            qDebug() << "DXAManager::isInstalled - working directory is not writable at"
                     << workingDir;
        return false;
    }
    if (!workingDir.isWritable()) {
        if (isDebugMode)
            qDebug() << "DXAManager::isInstalled - working directory is not writable at"
                     << workingDir;
        return false;
    }

    return true;
}

// what the manager does in response to the main application
// window invoking its run method
//
bool DXAManager::start()
{
    if (!setUp()) {
        return false;
    }

    m_dicomServer->start();

    emit started(m_test);
    emit dataChanged(m_test);
    emit canMeasure();

    return true;
}

// Set up device
bool DXAManager::setUp()
{
    m_dicomServer.reset(
        new DcmRecv(m_runnableName, m_ascConfigPath, m_storageDirPath, m_aeTitle, m_port));

    connect(m_dicomServer.get(), &DcmRecv::running, this, [=]() {
        if (m_debug)
            qDebug() << "DxaManager::running - DICOM server is running...";
    });

    connect(m_dicomServer.get(),
            &DcmRecv::dicomFilesReceived,
            this,
            &DXAManager::dicomFilesReceived);

    return true;
}

void DXAManager::dicomFilesReceived(QList<DicomFile> dicomFiles)
{
    // pass received dicom files to test class
    QSharedPointer<DXATest> test = qSharedPointerCast<DXATest>(m_test);
    QSharedPointer<DXASession> session = qSharedPointerCast<DXASession>(m_session);

    test->fromDicomFiles(dicomFiles, *session);
    emit dataChanged(m_test);
    emit canMeasure();
}

// retrieve a measurement from the device
//
void DXAManager::measure()
{
    if (m_sim) {
        m_test->reset();
        m_test->simulate();

        emit dataChanged(m_test);
        emit canFinish();

        return;
    }

    auto test = qSharedPointerCast<DXATest>(m_test);
    if (!test->hasAllNeededFiles()) {
        QMessageBox::warning(nullptr, "Warning", "Have not received all images from Hologic Apex");
        return;
    }

    // get patscan db variables for measurements
    const QFileInfo patscanFileInfo(m_patscanDbPath);
    const QString localPatscanPath= QDir::currentPath() + "/" + patscanFileInfo.fileName();

    if (patscanFileInfo.exists() && patscanFileInfo.isReadable()) {
        if (QFileInfo(localPatscanPath).exists()) {
            QFile::remove(localPatscanPath);
        }
        if (m_debug)
            qDebug() << localPatscanPath;

        if (!QFile::copy(m_patscanDbPath, localPatscanPath)) {
            if (m_debug)
                qDebug() << "error copying patscan db from " << m_patscanDbPath << "to"
                         << localPatscanPath;
        }
    } else {
        qDebug() << "could not access patscanDb at" << m_patscanDbPath;
        emit error("Could not access Apex workstation");
        return;
    }

    // get refscan db variables for measurements
    const QFileInfo refscanFileInfo(m_refscanDbPath);
    const QString localRefScanPath = QDir::currentPath() + "/" + refscanFileInfo.fileName();

    if (refscanFileInfo.exists() && refscanFileInfo.isReadable()) {
        if (QFileInfo(localRefScanPath).exists()) {
            QFile::remove(localRefScanPath);
        }
        if (m_debug)
            qDebug() << localRefScanPath;

        if (!QFile::copy(m_refscanDbPath, localRefScanPath)) {
            if (m_debug)
                qDebug() << "error copying refscan db from " << m_refscanDbPath << "to"
                         << localRefScanPath;
        }
    } else {
        emit error("Could not access Apex workstation");
        return;
    }

    // calculate totals and averages
    QSqlDatabase patscanDb = QSqlDatabase::addDatabase("QODBC", "patscan");
    patscanDb.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};DBQ="
                              + QDir::toNativeSeparators(localPatscanPath));
    if (!patscanDb.open()) {
        qDebug() << "Error: " << patscanDb.lastError().text();
        emit error("Could not open PatScan.mdb");
    } else {
        qDebug() << "connected to the patscanDb at " << localPatscanPath;
    }


    QSqlDatabase referenceDb = QSqlDatabase::addDatabase("QODBC", "reference");
    referenceDb.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};DBQ="
                                + QDir::toNativeSeparators(localRefScanPath));
    if (!referenceDb.open()) {
        qDebug() << "Error: " << patscanDb.lastError().text();
        emit error("Could not open reference.mdb");
    } else {
        qDebug() << "connected to the reference.mdb at " << localRefScanPath;
    }

    test->getPatientScan(patscanDb, m_session->getBarcode());

    QString patientKey = test->getMetaData("PATIENT_KEY").toString();
    QJsonObject patientData {
        { "PATIENT_KEY", patientKey },
        { "BIRTHDATE",   test->getMetaData("BIRTHDATE").toString() },
        { "SEX",         test->getMetaData("SEX").toString() },
        { "ETHNICITY",   test->getMetaData("ETHNICITY").toString() }
    };

    test->wholeBodyMeasurement->getScanAnalysisData(patscanDb, referenceDb, patientData);
    test->leftForearmMeasurement->getScanAnalysisData(patscanDb, referenceDb, patientData);
    test->rightForearmMeasurement->getScanAnalysisData(patscanDb, referenceDb, patientData);
    test->apSpineMeasurement->getScanAnalysisData(patscanDb, referenceDb, patientData);

    emit canFinish();

    patscanDb.close();
    patscanDb.close();
}

// implementation of final clean up of device after disconnecting and all
// data has been retrieved and processed by any upstream classes
//
void DXAManager::finish()
{
    QSharedPointer<DXATest> test = qSharedPointerCast<DXATest>(m_test);

    int answer_id = m_session->getAnswerId();
    QString barcode = m_session->getBarcode();


    // Whole body
    QByteArray wb_1 = FileUtils::readFile(test->wholeBodyMeasurement->m_wholeBody1.absFilePath);
    QByteArray wb_2 = FileUtils::readFile(test->wholeBodyMeasurement->m_wholeBody2.absFilePath);

    QString wb_1_file_name = test->wholeBodyMeasurement->m_wholeBody1.name + ".dcm";
    QString wb_2_file_name = test->wholeBodyMeasurement->m_wholeBody2.name + ".dcm";

    QByteArray sp_1 = FileUtils::readFile(test->apSpineMeasurement->m_apSpineFile.absFilePath);

    // AP Lumbar Spine
    QString sp_1_file_name = test->apSpineMeasurement->m_apSpineFile.name + ".dcm";

    QByteArray iva_ot = FileUtils::readFile(test->ivaImagingMeasurement->m_dicomOtFile.absFilePath);
    QByteArray iva_pr = FileUtils::readFile(test->ivaImagingMeasurement->m_dicomPrFile.absFilePath);
    QByteArray iva_measure = FileUtils::readFile(
        test->ivaImagingMeasurement->m_dicomMeasureFile.absFilePath);

    // IVA
    QString iva_ot_file_name = test->ivaImagingMeasurement->m_dicomOtFile.name + ".dcm";
    QString iva_pr_file_name = test->ivaImagingMeasurement->m_dicomPrFile.name + ".dcm";
    QString iva_measure_file_name = test->ivaImagingMeasurement->m_dicomMeasureFile.name + ".dcm";

    // Forearm Left
    QString fa_1_file_name = "FA_L_DICOM.dcm";
    QByteArray fa_1 = FileUtils::readFile(test->leftForearmMeasurement->m_forearmDicomFile.absFilePath);

    // Forearm Right
    QString fa_2_file_name = "FA_R_DICOM.dcm";
    QByteArray fa_2 = FileUtils::readFile(test->leftForearmMeasurement->m_forearmDicomFile.absFilePath);

    QJsonObject testJson = m_test->toJsonObject();
    QJsonObject sessionObj = m_session->getJsonObject();
    QJsonObject metadata = m_test->getMetaData().toJsonObject();

    QJsonObject files = {};
    files.insert(wb_1_file_name.replace(".", "_"),
                 test->wholeBodyMeasurement->m_wholeBody1.size);
    files.insert(wb_2_file_name.replace(".", "_"),
                 test->wholeBodyMeasurement->m_wholeBody2.size);
    files.insert(sp_1_file_name.replace(".", "_"),
                 test->apSpineMeasurement->m_apSpineFile.size);
    files.insert(iva_ot_file_name.replace(".", "_"),
                 test->ivaImagingMeasurement->m_dicomOtFile.size);
    files.insert(iva_pr_file_name.replace(".", "_"),
                 test->ivaImagingMeasurement->m_dicomPrFile.size);
    files.insert(iva_measure_file_name.replace(".", "_"),
                 test->ivaImagingMeasurement->m_dicomMeasureFile.size);
    files.insert(fa_1_file_name.replace(".", "_"),
                 test->leftForearmMeasurement->m_forearmDicomFile.size);
    files.insert(fa_2_file_name.replace(".", "_"),
                 test->rightForearmMeasurement->m_forearmDicomFile.size);

    testJson.insert("session", sessionObj);
    testJson.insert("files", files);

    QJsonObject responseJson = {};
    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    QString host = CypressSettings::getPineHost();
    QString endpoint = CypressSettings::getPineEndpoint();

    bool ok = false;
    ok = NetworkUtils::sendHTTPSRequest("PATCH",
                          (host + endpoint + QString::number(answer_id)).toStdString(),
                          "application/json",
                          serializedData);
    if (!ok) {
            qDebug() << "error transmitting results";
        return;
    }

    if (!test->wholeBodyMeasurement->m_wholeBody1.absFilePath.isEmpty()) {
        ok = NetworkUtils::sendHTTPSRequest("PATCH",
                  (host + endpoint + QString::number(answer_id)
                                             + "?filename=" + wb_1_file_name.replace("_", ".")).toStdString(),
                  "application/octet-stream",
                  wb_1);
        if (!ok) {
            qDebug() << "error transmitting file: " << wb_1_file_name;
            return;
        }
    }

    if (!test->wholeBodyMeasurement->m_wholeBody2.absFilePath.isEmpty()) {
        ok = NetworkUtils::sendHTTPSRequest("PATCH",
                  (host + endpoint + QString::number(answer_id)
                         + "?filename=" + wb_2_file_name.replace("_", ".")).toStdString(),
                  "application/octet-stream",
                  wb_2);
        if (!ok) {
            qDebug() << "error transmitting file: " << wb_2_file_name;
            return;
        }
    }

    if (test->apSpineMeasurement->m_hasApSpineFile) {
        ok = NetworkUtils::sendHTTPSRequest("PATCH",
                  (host + endpoint + QString::number(answer_id)
                         + "?filename=" + sp_1_file_name.replace("_", ".")).toStdString(),
                  "application/octet-stream",
                  sp_1);
        if (!ok) {
            qDebug() << "error transmitting file: " << sp_1_file_name;
            return;
        }
    }

    if (test->ivaImagingMeasurement->hasOtFile) {
        ok = NetworkUtils::sendHTTPSRequest("PATCH",
                  (host + endpoint + QString::number(answer_id)
                         + "?filename=" + iva_ot_file_name.replace("_", ".")).toStdString(),
                  "application/octet-stream",
                  iva_ot);
        if (!ok) {
            qDebug() << "error transmitting file: " << iva_ot_file_name;
            return;
        }
    }

    if (test->ivaImagingMeasurement->hasPrFile) {
        ok = NetworkUtils::sendHTTPSRequest("PATCH",
                  (host + endpoint + QString::number(answer_id)
                         + "?filename=" + iva_pr_file_name.replace("_", ".")).toStdString(),
                  "application/octet-stream",
                  iva_pr);
        if (!ok) {
            qDebug() << "error transmitting file: " << iva_pr_file_name;
            return;
        }
    }

    if (test->ivaImagingMeasurement->hasMeasureFile) {
        ok = NetworkUtils::sendHTTPSRequest("PATCH",
                  (host + endpoint + QString::number(answer_id)
                         + "?filename=" + iva_measure_file_name.replace("_", ".")).toStdString(),
                  "application/octet-stream",
                  iva_measure);
        if (!ok) {
            qDebug() << "error transmitting file: " << iva_measure_file_name;
            return;
        }
    }

    if (!test->leftForearmMeasurement->m_forearmDicomFile.absFilePath.isNull()) {
        ok = NetworkUtils::sendHTTPSRequest("PATCH",
                  (host + endpoint + QString::number(answer_id)
                         + "?filename=" + fa_1_file_name.replace("_", ".")).toStdString(),
                  "application/octet-stream",
                  fa_1);
        if (!ok) {
            //emit error("Could not transmit file");
            qDebug() << "error transmitting file: " << fa_1_file_name;
            return;
        }
    }

    if (!test->rightForearmMeasurement->m_forearmDicomFile.absFilePath.isNull()) {
        ok = NetworkUtils::sendHTTPSRequest("PATCH",
                  (host + endpoint + QString::number(answer_id)
                         + "?filename=" + fa_2_file_name.replace("_", ".")).toStdString(),
                  "application/octet-stream",
                  fa_2);
        if (!ok) {
            //emit error("Could not transmit file");
            qDebug() << "error transmitting file: " << fa_2_file_name;
            return;
        }
    }

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
