#include "oct_manager.h"

#include "data/oct_test.h"

#include "server/sessions/oct_session.h"

#include <QFileInfo>


OCTManager::OCTManager(QSharedPointer<OCTSession> session): ManagerBase { session }
{
    m_test.reset(new OCTTest);

    m_runnableName = CypressSettings::readSetting("oct/dicom/runnableName").toString();
    m_runnablePath = CypressSettings::readSetting("oct/dicom/runnablePath").toString();
    m_webpage = CypressSettings::readSetting("oct/webpage").toString();
}

OCTManager::~OCTManager()
{
}

bool OCTManager::isInstalled()
{
    if (CypressSettings::isSimMode())
        return true;

    const QString runnableName = CypressSettings::readSetting("oct/runnableName").toString();
    const QString runnablePath = CypressSettings::readSetting("oct/runnablePath").toString();
    const QString webpage = CypressSettings::readSetting("oct/webpage").toString();

    if (runnableName.isNull() || runnableName.isEmpty()) {
        qDebug() << "runnableName is not defined";
        return false;
    }

    if (runnablePath.isNull() || runnablePath.isEmpty()) {
        qDebug() << "runnablePath is not defined";
        return false;
    }

    if (webpage.isNull() || webpage.isEmpty()) {
        qDebug() << "aeTitle is not defined";
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

    const QString command = m_runnableName;

    QStringList arguments;
    arguments << m_webpage;

    m_process.setProgram(command);
    m_process.setArguments(arguments);
    m_process.setWorkingDirectory(m_runnablePath);
    m_process.setProcessChannelMode(QProcess::ForwardedChannels);

    connect(&m_process, &QProcess::started, this, [=]() {
        qDebug() << "process started: " << m_process.arguments().join(" ");
    });

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=]() {
        qDebug() << "process finished, reading output";
        readOutput();
    });

    m_process.start();

    if (!m_process.waitForStarted())
        return false;

    emit started(m_test);
    emit dataChanged(m_test);
    emit canMeasure();

    return true;
}

void OCTManager::readOutput()
{
    qDebug() << "OCTManager::readOutput";

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
