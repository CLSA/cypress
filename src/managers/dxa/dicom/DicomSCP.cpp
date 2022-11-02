#include "DicomSCP.h"

#include <QDebug>
#include <QFile>
#include <signal.h>

DicomSCP::DicomSCP(QObject *parent)
    : QObject{parent}
{
    initConnections();
}

DicomSCP::~DicomSCP() {
}

void DicomSCP::initProcess()
{
    m_process.reset(new QProcess());

    QStringList args;
    args
        << "-xf" << "../etc/dcmtk/storescp.cfg"
        << "default"
        << "--filename-extension" << ".dcm"
        << "-od" << "../storage"
        << "-lc" << "../etc/dcmtk/filelog.cfg"
        << "9001";

    m_process->setWorkingDirectory("C:/work/clsa/cypress/dcmtk-3.6.7/bin");
    m_process->setProgram("C:/work/clsa/cypress/dcmtk-3.6.7/bin/dcmrecv.exe");
    m_process->setArguments(args);
}

void DicomSCP::initConnections()
{
    initProcess();
    start();

    connect(m_process.get(), &QProcess::stateChanged, this, [this]() {
        const QProcess::ProcessState state = this->m_process->state();
        switch (state)
        {
            case QProcess::ProcessState::NotRunning:
                emit notRunning();
                break;
            case QProcess::ProcessState::Starting:
                emit starting();
                break;
            case QProcess::ProcessState::Running:
                qDebug() << "log worker";
                //m_logWorker->start();
                emit running();
                break;
        }
    });

    connect(m_process.get(), &QProcess::readyReadStandardOutput, this, [this]() {
        QString output = m_process->readAllStandardOutput();
        emit logUpdate(output);
    });

    connect(m_process.get(),
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            [=](int exitCode, QProcess::ExitStatus exitStatus)
            {
                qInfo() << "DICOM Storage SCP: exit " << exitCode << exitStatus;
                switch (exitStatus)
                {
                    case QProcess::ExitStatus::NormalExit:
                        emit exitNormal();
                    case QProcess::ExitStatus::CrashExit:
                        emit exitCrash();
                }
            }
    );
}

bool DicomSCP::start()
{
    m_process->start();

    return m_process->waitForStarted();
}

bool DicomSCP::stop()
{
    m_process->close();
    return m_process->waitForFinished();
}

