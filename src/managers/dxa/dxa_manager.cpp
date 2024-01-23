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

    QString runnableName = CypressSettings::readSetting("dxa/dicom/runnableName").toString();
    QString runnablePath = CypressSettings::readSetting("dxa/dicom/runnablePath").toString();
    QString aeTitle = CypressSettings::readSetting("dxa/dicom/aeTitle").toString();
    QString host = CypressSettings::readSetting("dxa/dicom/host").toString();
    QString port = CypressSettings::readSetting("dxa/dicom/port").toString();

    QString storageDirPath = CypressSettings::readSetting("dxa/dicom/storagePath").toString();
    QString logConfigPath = CypressSettings::readSetting("dxa/dicom/log_config").toString();
    QString ascConfigPath = CypressSettings::readSetting("dxa/dicom/asc_config").toString();

    QString patscanDbPath = CypressSettings::readSetting("dxa/patscanDbPath").toString();
    QString refscanDbPath = CypressSettings::readSetting("dxa/refscanDbPath").toString();

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

    QFileInfo patscanFile(patscanDbPath);
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

    QFileInfo refscanFile(refscanDbPath);
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

    QFileInfo exeInfo(runnableName);
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

    QFileInfo workingDir(runnablePath);
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
    setUp();

    m_dicomServer->start();

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

    if (!test->hasAllNeededFiles()) {
        if (m_debug)
            qDebug() << "DXAManager - does not have all needed files";
    }
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

    QSharedPointer<DXATest> test = qSharedPointerCast<DXATest>(m_test);

    if (!test->hasAllNeededFiles()) {
        QMessageBox::warning(nullptr, "Warning", "Have not received all images from Hologic Apex");
        return;
    }

    // get patscan db variables for measurements
    QFileInfo patscanFileInfo(m_patscanDbPath);
    if (patscanFileInfo.exists() && patscanFileInfo.isReadable()) {
        QString localPath = QDir::currentPath() + "/" + patscanFileInfo.fileName();
        if (QFileInfo(localPath).exists()) {
            QFile::remove(localPath);
        }
        if (m_debug)
            qDebug() << localPath;

        if (!QFile::copy(m_patscanDbPath, localPath)) {
            if (m_debug)
                qDebug() << "error copying patscan db from " << m_patscanDbPath << "to"
                         << localPath;
        }
    } else {
        qDebug() << "could not access patscanDb at" << m_patscanDbPath;
        QMessageBox::critical(nullptr, "Error", "Could not access Apex workstation");
        return;
    }

    // get refscan db variables for measurements
    QFileInfo refscanFileInfo(m_refscanDbPath);
    if (refscanFileInfo.exists() && refscanFileInfo.isReadable()) {
        QString localPath = QDir::currentPath() + "/" + refscanFileInfo.fileName();
        if (QFileInfo(localPath).exists()) {
            QFile::remove(localPath);
        }

        if (m_debug)
            qDebug() << localPath;

        if (!QFile::copy(m_patscanDbPath, localPath)) {
            if (m_debug)
                qDebug() << "error copying refscan db from " << m_refscanDbPath << "to"
                         << localPath;
        }
    } else {
        qDebug() << "could not access refscanDb at" << m_refscanDbPath;
        QMessageBox::critical(nullptr, "Error", "Could not access Apex workstation");
        return;
    }

    // calculate totals and averages
    QSqlDatabase patscanDb = QSqlDatabase::addDatabase("QODBC");
    patscanDb.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};DBQ="
                              + patscanFileInfo.absoluteFilePath());
    if (!patscanDb.open()) {
        qDebug() << "Error: " << patscanDb.lastError().text();
        // Handle error
    } else {
        // Connection is successful, you can query the database
        qDebug() << "connected to the patscanDb at " << patscanFileInfo.absoluteFilePath();
        patscanDb.close();
    }

    QSqlDatabase refscanDb = QSqlDatabase::addDatabase("QODBC");
    patscanDb.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};DBQ="
                              + refscanFileInfo.absoluteFilePath());
    if (!refscanDb.open()) {
        qDebug() << "Error: " << patscanDb.lastError().text();
        // Handle error
    } else {
        // Connection is successful, you can query the database
        qDebug() << "connected to the patscanDb at " << patscanFileInfo.absoluteFilePath();
        refscanDb.close();
    }

    // scoring
    emit dataChanged(m_test);
    if (m_test->isValid()) {
        emit canFinish();
    } else {
        QMessageBox::warning(nullptr, "Error", "Invalid or incomplete test results");
    }
}

// implementation of final clean up of device after disconnecting and all
// data has been retrieved and processed by any upstream classes
//
void DXAManager::finish()
{
    int answer_id = m_session->getAnswerId();
    QString barcode = m_session->getBarcode();

    DXATest *test = static_cast<DXATest *>(m_test.get());

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

    // Forearm
    QString fa_1_file_name = "FA_DICOM.dcm";
    QByteArray fa_1 = FileUtils::readFile(test->forearmMeasurement->m_forearmDicomFile.absFilePath);

    QJsonObject testJson = m_test->toJsonObject();
    QJsonObject sessionObj = m_session->getJsonObject();
    QJsonObject metadata = m_test->getMetaData().toJsonObject();

    QJsonObject files = {};
    files.insert(wb_1_file_name.replace(QRegExp(".dcm"), ""),
                 test->wholeBodyMeasurement->m_wholeBody1.size);
    files.insert(wb_2_file_name.replace(QRegExp(".dcm"), ""),
                 test->wholeBodyMeasurement->m_wholeBody2.size);
    files.insert(sp_1_file_name.replace(QRegExp(".dcm"), ""),
                 test->apSpineMeasurement->m_apSpineFile.size);
    files.insert(iva_ot_file_name.replace(QRegExp(".dcm"), ""),
                 test->ivaImagingMeasurement->m_dicomOtFile.size);
    files.insert(iva_pr_file_name.replace(QRegExp(".dcm"), ""),
                 test->ivaImagingMeasurement->m_dicomPrFile.size);
    files.insert(iva_measure_file_name.replace(QRegExp(".dcm"), ""),
                 test->ivaImagingMeasurement->m_dicomMeasureFile.size);
    files.insert(fa_1_file_name.replace(QRegExp(".dcm"), ""),
                 test->forearmMeasurement->m_forearmDicomFile.size);

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
        return;
    }

    ok = NetworkUtils::sendHTTPSRequest("PATCH",
                          (host + endpoint + QString::number(answer_id)
                           + "?filename=" + wb_1_file_name + ".dcm").toStdString(),
                          "application/octet-stream",
                          wb_1);
    if (!ok) {
        return;
    }

    ok = NetworkUtils::sendHTTPSRequest("PATCH",
                          (host + endpoint + QString::number(answer_id)
                                         + "?filename=" + wb_2_file_name + ".dcm").toStdString(),
                          "application/octet-stream",
                          wb_2);
    if (!ok) {
        return;
    }
    ok = NetworkUtils::sendHTTPSRequest("PATCH",
                          (host + endpoint + QString::number(answer_id)
                                         + "?filename=" + sp_1_file_name + ".dcm").toStdString(),
                          "application/octet-stream",
                          sp_1);
    if (!ok) {
        return;
    } else {

    }

    ok = NetworkUtils::sendHTTPSRequest("PATCH",
                          (host + endpoint + QString::number(answer_id)
                                         + "?filename=" + iva_ot_file_name + ".dcm").toStdString(),
                          "application/octet-stream",
                          iva_ot);
    if (!ok) {
        return;
    }
    else {

    }

    ok = NetworkUtils::sendHTTPSRequest("PATCH",
                          (host + endpoint + QString::number(answer_id)
                                         + "?filename=" + iva_pr_file_name + ".dcm").toStdString(),
                          "application/octet-stream",
                          iva_pr);
    if (!ok) {
        return;
    }

    ok = NetworkUtils::sendHTTPSRequest("PATCH",
                          (host + endpoint + QString::number(answer_id)
                                         + "?filename=" + iva_measure_file_name + ".dcm").toStdString(),
                          "application/octet-stream",
                          iva_measure);
    if (!ok) {
        return;
    }
    else {

    }

    ok = NetworkUtils::sendHTTPSRequest("PATCH",
                          (host + endpoint + QString::number(answer_id)
                                         + "?filename=" + fa_1_file_name + ".dcm").toStdString(),
                          "application/octet-stream",
                          fa_1);
    if (!ok) {
        return;
    }
    else {

    }

    emit success("");

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
