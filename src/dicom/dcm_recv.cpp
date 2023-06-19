#include "dcm_recv.h"
#include <QDebug>

DcmRecv::DcmRecv(QObject* parent) :
    QObject(parent)
{
    connect(&m_process, &QProcess::readyReadStandardOutput, this, &DcmRecv::onReadyReadStandardOutput);
    connect(&m_process, &QProcess::readyReadStandardError, this, &DcmRecv::onReadyReadStandardError);
    connect(&m_process, &QProcess::errorOccurred, this, &DcmRecv::onErrorOccurred);
    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &DcmRecv::onFinished);
}

DcmRecv::~DcmRecv()
{
    m_process.terminate();
    m_process.waitForFinished();
}

bool DcmRecv::startDcmRecv(const QString& executablePath, const QString& aetitle, quint16 port)
{
    if (!m_tempDir.isValid())
    {
        qWarning() << "Temporary directory creation failed";
        return false;
    }

    QStringList arguments;
    arguments << aetitle << QString::number(port) << "--output-directory" << m_tempDir.path();
    m_process.start(executablePath, arguments);

    bool started = m_process.waitForStarted();
    if (started)
    {
        emit running();
    }

    return started;
}

QString DcmRecv::receivedFilesDir() const
{
    return m_tempDir.path();
}

void DcmRecv::onReadyReadStandardOutput()
{
    QByteArray output = m_process.readAllStandardOutput();
    qDebug() << "DCMRECV stdout:" << output;
    emit logUpdate(output);
}

void DcmRecv::onReadyReadStandardError()
{
    QByteArray error = m_process.readAllStandardError();
    qDebug() << "DCMRECV stderr:" << error;
    emit logUpdate(error);
}

void DcmRecv::onErrorOccurred(QProcess::ProcessError error)
{
    qWarning() << "DCMRECV process error:" << error;
}

void DcmRecv::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "DCMRECV process finished with exit code" << exitCode << "and exit status" << exitStatus;
    if (exitCode == 0)
    {
        emit exitNormal();
    }
    else
    {
        emit exitCrash();
    }
}
