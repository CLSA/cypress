#include "oct_manager.h"

#include "data/oct_test.h"

#include "auxiliary/windows_util.h"

#include "server/sessions/oct_session.h"


OCTManager::OCTManager(QSharedPointer<OCTSession> session): ManagerBase { session }
{
    m_test.reset(new OCTTest);

    m_runnableName = CypressSettings::readSetting("oct/dicom/runnableName").toString();
    m_runnablePath = CypressSettings::readSetting("oct/dicom/runnablePath").toString();

    m_aeTitle = CypressSettings::readSetting("oct/dicom/aeTitle").toString();
    m_host = CypressSettings::readSetting("oct/dicom/host").toString();
    m_port = CypressSettings::readSetting("oct/dicom/port").toString();

    m_storageDirPath = CypressSettings::readSetting("oct/dicom/storagePath").toString();
    m_logConfigPath = CypressSettings::readSetting("oct/dicom/log_config").toString();
    m_ascConfigPath = CypressSettings::readSetting("oct/dicom/asc_config").toString();

    m_dicomServer.reset(
        new DcmRecv(
            m_runnableName,
            m_ascConfigPath,
            m_storageDirPath,
            m_aeTitle,
            m_port
        )
    );

    WindowsUtil::killProcessByName(L"storescp.exe");

    connect(
        m_dicomServer.get(),
        &DcmRecv::dicomFilesReceived,
        this,
        &OCTManager::dicomFilesReceived
    );
}

OCTManager::~OCTManager()
{
    m_dicomServer->stop();
}

bool OCTManager::isInstalled()
{
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

bool OCTManager::start()
{
    qDebug() << "OCTManager::start";

    m_dicomServer->start();

    emit started(m_test);
    emit dataChanged(m_test);
    emit canMeasure();

    return true;
}

void OCTManager::dicomFilesReceived(QList<DicomFile> dicomFiles)
{
    qDebug() << "dicom files received:" << dicomFiles.length();
}

void OCTManager::measure()
{
    qDebug() << "OCTManager::measure";
    if (m_test->isValid())
        emit canFinish();
}

void OCTManager::finish()
{
    qDebug() << "OCTManager::finish";
}

bool OCTManager::clearData()
{
    qDebug() << "OCTManager::clearData";

    m_test->reset();
    return true;
}

bool OCTManager::setUp()
{
    qDebug() << "OCTManager::setUp";

    return true;
}

bool OCTManager::cleanUp()
{
    qDebug() << "OCTManager::cleanUp";

    return true;
}
