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
        emit error("Could not access Apex workstation");
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
        emit error("Could not access Apex workstation");
        return;
    }

    // calculate totals and averages
    QSqlDatabase patscanDb = QSqlDatabase::addDatabase("QODBC", "patscan");
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


    QSqlDatabase refscanDb = QSqlDatabase::addDatabase("QODBC", "reference");
    refscanDb.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};DBQ="
                              + refscanFileInfo.absoluteFilePath());
    if (!refscanDb.open()) {
        qDebug() << "Error: " << patscanDb.lastError().text();
        // Handle error
    } else {
        // Connection is successful, you can query the database
        qDebug() << "connected to the patscanDb at " << patscanFileInfo.absoluteFilePath();
        refscanDb.close();
    }

    QSqlQuery query;
    query.prepare("SELECT PATIENT_KEY, BIRTHDATE, SEX, ETHNICITY FROM PATIENT WHERE IDENTIFIER1 = :identifier1");
    query.bindValue(":identifier1", m_session->getBarcode());

    if (!query.exec()) {
        qDebug() << "patient query failed" << query.lastError().text();
    }

    QString patientKey = query.value("PATIENT_KEY").toString();
    QString patientDOB = query.value("BIRTHDATE").toString();
    QString patientSex = query.value("SEX").toString();
    QString ethnicity = "W";

    QSqlQuery query2;
    QString scanType = "1";
    query2.prepare("SELECT SCANID, SCAN_MODE, SCAN_DATE FROM ScanAnalysis WHERE PATIENT_KEY = :patientKey AND SCAN_TYPE = :scanType");
    query2.bindValue(":patientKey", patientKey);
    query2.bindValue(":scanType", scanType);

    if (!query.exec()) {
        qDebug() << "scan id query failed" << query.lastError().text();
    }


    QSqlQuery query3;

    // Spine
    query3.prepare("SELECT * FROM Spine WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    if (!query3.exec()) {
        qDebug() << "spine query failed" << query3.lastError().text();
    }

    long NO_REGIONS;
    long STARTING_REGION;

    bool L1_INCLUDED;
    double L1_AREA;
    double L1_BMC;
    double L1_BMD;

    bool L2_INCLUDED;
    double L2_AREA;
    double L2_BMC;
    double L2_BMD;

    bool L3_INCLUDED;
    double L3_AREA;
    double L3_BMC;
    double L3_BMD;

    bool L4_INCLUDED;
    double L4_AREA;
    double L4_BMC;
    double L4_BMD;

    double TOT_AREA;
    double TOT_BMC;
    double TOT_BMD;
    double STD_TOT_BMD;
    long ROI_TYPE;
    double ROI_WIDTH;
    double ROI_HEIGHT;
    QString PHYSICIAN_COMMENT;

    // Hip
    query3.prepare("SELECT * FROM Hip WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    if (!query3.exec()) {
        qDebug() << "hip query failed: " << query3.lastError().text();
    }
    double TROCH_AREA;
    double TROCH_BMC;
    double TROCH_BMD;
    double INTER_AREA;
    double INTER_BMC;
    double INTER_BMD;
    double NECK_AREA;
    double NECK_BMC;
    double NECK_BMD;
    double WARDS_AREA;
    double WARDS_BMC;
    double WARDS_BMD;
    double HTOT_AREA;
    double HTOT_BMC;
    double HTOT_BMD;

    long ROI_TYPE;
    double ROI_WIDTH;
    double ROI_HEIGHT;
    double AXIS_LENGTH;
    QString PHYSICIAN_COMMENT;

    query3.prepare("SELECT * FROM HipHSA WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    if (!query3.exec()) {
        qDebug() << "hipHSA query failed" << query3.lastError().text();
    }

    double NN_BMD;
    double NN_CSA;
    double NN_CSMI;
    double NN_WIDTH;
    double NN_ED;
    double NN_ACT;
    double NN_PCD;
    double NN_CMP;
    double NN_SECT_MOD;
    double NN_BR;
    double IT_BMD;
    double IT_CSA;
    double IT_CSMI;
    double IT_WIDTH;
    double IT_ED;
    double IT_ACT;
    double IT_PCD;
    double IT_CMP;
    double IT_SECT_MOD;
    double IT_BR;
    double FS_BMD;
    double FS_CSA;
    double FS_CSMI;
    double FS_WIDTH;
    double FS_ED;
    double FS_ACT;
    double FS_PCD;
    double FS_CMP;
    double FS_SECT_MOD;
    double FS_BR;
    double SHAFT_NECK_ANGLE;


    // Whole body
    query3.prepare("SELECT * FROM Wbody WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    if (!query3.exec()) {
        qDebug() << query3.lastError().text();
    }

    double WBTOT_AREA;
    double WBTOT_BMC;
    double WBTOT_BMD;
    double SUBTOT_AREA;
    double SUBTOT_BMC;
    double SUBTOT_BMD;
    double HEAD_AREA;
    double HEAD_BMC;
    double HEAD_BMD;
    double LARM_AREA;
    double LARM_BMC;
    double LARM_BMD;
    double RARM_AREA;
    double RARM_BMC;
    double RARM_BMD;
    double LRIB_AREA;
    double LRIB_BMC;
    double LRIB_BMD;
    double RRIB_AREA;
    double RRIB_BMC;
    double RRIB_BMD;
    double T_S_AREA;
    double T_S_BMC;
    double T_S_BMD;
    double L_S_AREA;
    double L_S_BMC;
    double L_S_BMD;
    double PELV_AREA;
    double PELV_BMC;
    double PELV_BMD;
    double LLEG_AREA;
    double LLEG_BMC;
    double LLEG_BMD;
    double RLEG_AREA;
    double RLEG_BMC;
    double RLEG_BMD;
    QString PHYSICIAN_COMMENT;

    query3.prepare("SELECT * FROM WbodyComposition WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    if (!query3.exec()) {
        qDebug() << query3.lastError().text();
    }

    double FAT_STD;
    double LEAN_STD;
    double BRAIN_FAT;
    double WATER_LBM;
    double HEAD_FAT;
    double HEAD_LEAN;
    double HEAD_MASS;
    double HEAD_PFAT;
    double LARM_FAT;
    double LARM_LEAN;
    double LARM_MASS;
    double LARM_PFAT;
    double RARM_FAT;
    double RARM_LEAN;
    double RARM_MASS;
    double RARM_PFAT;
    double TRUNK_FAT;
    double TRUNK_LEAN;
    double TRUNK_MASS;
    double TRUNK_PFAT;
    double L_LEG_FAT;
    double L_LEG_LEAN;
    double L_LEG_MASS;
    double L_LEG_PFAT;
    double R_LEG_FAT;
    double R_LEG_LEAN;
    double R_LEG_MASS;
    double R_LEG_PFAT;
    double SUBTOT_FAT;
    double SUBTOT_LEAN;
    double SUBTOT_MASS;
    double SUBTOT_PFAT;
    double WBTOT_FAT;
    double WBTOT_LEAN;
    double WBTOT_MASS;
    double WBTOT_PFAT;
    QString PHYSICIAN_COMMENT;

    query3.prepare("SELECT * FROM SubRegionBone WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    if (query3.exec()) {
        qDebug() << query3.lastError().text();
    }

    double NET_AVG_AREA;
    double NET_AVG_BMC;
    double NET_AVG_BMD;
    double GLOBAL_AREA;
    double GLOBAL_BMC;
    double GLOBAL_BMD;

    long NO_REGIONS;

    QString REG1_NAME;
    double REG1_AREA;
    double REG1_BMC;
    double REG1_BMD;

    QString REG2_NAME;
    double REG2_AREA;
    double REG2_BMC;
    double REG2_BMD;

    QString REG3_NAME;
    double REG3_AREA;
    double REG3_BMC;
    double REG3_BMD;

    QString REG4_NAME;
    double REG4_AREA;
    double REG4_BMC;
    double REG4_BMD;

    QString REG5_NAME;
    double REG5_AREA;
    double REG5_BMC;
    double REG5_BMD;

    QString REG6_NAME;
    double REG6_AREA;
    double REG6_BMC;
    double REG6_BMD;

    QString REG7_NAME;
    double REG7_AREA;
    double REG7_BMC;
    double REG7_BMD;

    QString REG8_NAME;
    double REG8_AREA;
    double REG8_BMC;
    double REG8_BMD;

    QString REG9_NAME;
    double REG9_AREA;
    double REG9_BMC;
    double REG9_BMD;

    QString REG10_NAME;
    double REG10_AREA;
    double REG10_BMC;
    double REG10_BMD;

    QString REG11_NAME;
    double REG11_AREA;
    double REG11_BMC;
    double REG11_BMD;

    QString REG12_NAME;
    double REG12_AREA;
    double REG12_BMC;
    double REG12_BMD;

    QString REG13_NAME;
    double REG13_AREA;
    double REG13_BMC;
    double REG13_BMD;

    QString REG14_NAME;
    double REG14_AREA;
    double REG14_BMC;
    double REG14_BMD;

    QString PHYSICIAN_COMMENT;

    query3.prepare("SELECT * FROM SubRegionComposition WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");

    double NET_AVG_FAT;
    double NET_AVG_LEAN;
    double NET_AVG_MASS;
    double NET_AVG_PFAT;
    double GLOBAL_FAT;
    double GLOBAL_LEAN;
    double GLOBAL_MASS;
    double GLOBAL_PFAT;

    long NO_REGIONS;

    QString REG1_NAME;
    double REG1_FAT;
    double REG1_LEAN;
    double REG1_MASS;
    double REG1_PFAT;

    QString REG2_NAME;
    double REG2_FAT;
    double REG2_LEAN;
    double REG2_MASS;
    double REG2_PFAT;

    QString REG3_NAME;
    double REG3_FAT;
    double REG3_LEAN;
    double REG3_MASS;
    double REG3_PFAT;

    QString REG4_NAME;
    double REG4_FAT;
    double REG4_LEAN;
    double REG4_MASS;
    double REG4_PFAT;

    QString REG5_NAME;
    double REG5_FAT;
    double REG5_LEAN;
    double REG5_MASS;
    double REG5_PFAT;

    QString REG6_NAME;
    double REG6_FAT;
    double REG6_LEAN;
    double REG6_MASS;
    double REG6_PFAT;

    QString REG7_NAME;
    double REG7_FAT;
    double REG7_LEAN;
    double REG7_MASS;
    double REG7_PFAT;

    QString REG8_NAME;
    double REG8_FAT;
    double REG8_LEAN;
    double REG8_MASS;
    double REG8_PFAT;

    QString REG9_NAME;
    double REG9_FAT;
    double REG9_LEAN;
    double REG9_MASS;
    double REG9_PFAT;

    QString REG10_NAME;
    double REG10_FAT;
    double REG10_LEAN;
    double REG10_MASS;
    double REG10_PFAT;

    QString REG11_NAME;
    double REG11_FAT;
    double REG11_LEAN;
    double REG11_MASS;
    double REG11_PFAT;

    QString REG12_NAME;
    double REG12_FAT;
    double REG12_LEAN;
    double REG12_MASS;
    double REG12_PFAT;

    QString REG13_NAME;
    double REG13_FAT;
    double REG13_LEAN;
    double REG13_MASS;
    double REG13_PFAT;

    QString REG14_NAME;
    double REG14_FAT;
    double REG14_LEAN;
    double REG14_MASS;
    double REG14_PFAT;

    int TISSUE_ANALYSIS_METHOD;
    QString PHYSICIAN_COMMENT;

    query3.prepare("SELECT * FROM ObesityIndices WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    double FAT_STD;
    double LEAN_STD;
    double BRAIN_FAT;
    double WATER_LBM;
    double TOTAL_PERCENT_FAT;
    double BODY_MASS_INDEX;
    double ANDROID_GYNOID_RATIO;
    double ANDROID_PERCENT_FAT;
    double GYNOID_PERCENT_FAT;
    double FAT_MASS_RATIO;
    double TRUNK_LIMB_FAT_MASS_RATIO;
    double FAT_MASS_HEIGHT_SQUARED;
    double TOTAL_FAT_MASS;
    double LEAN_MASS_HEIGHT_SQUARED;
    double APPENDAGE_LEAN_MASS_HEIGHT_2;
    double TOTAL_LEAN_MASS;
    QString PHYSICIAN_COMMENT;

    query3.prepare("SELECT * FROM AndroidGynoidComposition WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    double ANDROID_FAT;
    double ANDROID_LEAN;
    double GYNOID_FAT;
    double GYNOID_LEAN;
    QString PHYSICIAN_COMMENT;

    // Forearm
    query3.prepare("SELECT * FROM Forearm WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    double R_13_AREA;
    double R_13_BMC;
    double R_13_BMD;
    double R_MID_AREA;
    double R_MID_BMC;
    double R_MID_BMD;
    double R_UD_AREA;
    double R_UD_BMC;
    double R_UD_BMD;
    double U_13_AREA;
    double U_13_BMC;
    double U_13_BMD;
    double U_MID_AREA;
    double U_MID_BMC;
    double U_MID_BMD;
    double U_UD_AREA;
    double U_UD_BMC;
    double U_UD_BMD;
    double RTOT_AREA;
    double RTOT_BMC;
    double RTOT_BMD;
    double UTOT_AREA;
    double UTOT_BMC;
    double UTOT_BMD;
    double RU13TOT_AREA;
    double RU13TOT_BMC;
    double RU13TOT_BMD;
    double RUMIDTOT_AREA;
    double RUMIDTOT_BMC;
    double RUMIDTOT_BMD;
    double RUUDTOT_AREA;
    double RUUDTOT_BMC;
    double RUUDTOT_BMD;
    double RUTOT_AREA;
    double RUTOT_BMC;
    double RUTOT_BMD;
    double ROI_TYPE;
    double ROI_WIDTH;
    double ROI_HEIGHT;
    double ARM_LENGTH;
    QString PHYSICIAN_COMMENT;



    // scoring
    //emit dataChanged(m_test);
    //if (m_test->isValid()) {
    emit canFinish();
    //} else {
    //    QMessageBox::warning(nullptr, "Error", "Invalid or incomplete test results");
    //}
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
        emit error("Could not transmit file");
        return;
    }

    ok = NetworkUtils::sendHTTPSRequest("PATCH",
                          (host + endpoint + QString::number(answer_id)
                           + "?filename=" + wb_1_file_name + ".dcm").toStdString(),
                          "application/octet-stream",
                          wb_1);
    if (!ok) {
        emit error("Could not transmit file");
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
        emit error("Could not transmit file");
        return;
    }

    ok = NetworkUtils::sendHTTPSRequest("PATCH",
                          (host + endpoint + QString::number(answer_id)
                                         + "?filename=" + iva_ot_file_name + ".dcm").toStdString(),
                          "application/octet-stream",
                          iva_ot);
    if (!ok) {
        emit error("Could not transmit file");
        return;
    }

    ok = NetworkUtils::sendHTTPSRequest("PATCH",
                          (host + endpoint + QString::number(answer_id)
                                         + "?filename=" + iva_pr_file_name + ".dcm").toStdString(),
                          "application/octet-stream",
                          iva_pr);
    if (!ok) {
        emit error("Could not transmit file");
        return;
    }

    ok = NetworkUtils::sendHTTPSRequest("PATCH",
                          (host + endpoint + QString::number(answer_id)
                                         + "?filename=" + iva_measure_file_name + ".dcm").toStdString(),
                          "application/octet-stream",
                          iva_measure);
    if (!ok) {
        emit error("Could not transmit file");
        return;
    }

    ok = NetworkUtils::sendHTTPSRequest("PATCH",
                          (host + endpoint + QString::number(answer_id)
                                         + "?filename=" + fa_1_file_name + ".dcm").toStdString(),
                          "application/octet-stream",
                          fa_1);
    if (!ok) {
        emit error("Could not transmit file");
        return;
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
