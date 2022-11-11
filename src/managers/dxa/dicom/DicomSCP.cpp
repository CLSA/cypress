#include "DicomSCP.h"

#include <QDebug>
#include <QFile>
#include <signal.h>

#include "LogWorker.h"

DicomSCP::DicomSCP(QObject *parent)
    : QObject{parent}
{
    initConnections();
}

DicomSCP::~DicomSCP() {
    //m_process->close();
    //m_process->waitForFinished();
    //delete m_process;

    //m_logWorker->terminate();
    //m_logWorker->wait();
    //delete m_logWorker;

    //QProcess process;
    //process.execute("taskkill /im dcmrecv.exe /f");
    //process.waitForFinished();
}

void DicomSCP::initConnections()
{
    m_logWorker.reset(new LogWorker());
    m_process.reset(new QProcess());

    QStringList args;
    args
        << "-xf" << "../etc/dcmtk/storescp.cfg"
        << "default"
        << "--series-date-subdir"
        << "--short-unique-names"
        << "--filename-extension" << ".dcm"
        << "-od" << "C:/Users/Anthony/Documents/PACS"
        << "-lc" << "../etc/dcmtk/logger.cfg"
        << "9001";
    m_process->setWorkingDirectory("C:/work/clsa/cypress/dcmtk-3.6.7-win64-chocolatey/bin");
    m_process->setProgram("C:/work/clsa/cypress/dcmtk-3.6.7-win64-chocolatey/bin/dcmrecv.exe");
    m_process->setArguments(args);

    // Move worker object onto thread
    //connect(m_logWorker, &LogWorker::finished, m_logWorker, &QObject::deleteLater);
    connect(m_logWorker.get(), &LogWorker::updateLog, this, &DicomSCP::readLog);

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
                m_logWorker->start();
                emit running();
                break;
        }
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

void DicomSCP::readLog(QString line)
{
    emit logUpdate(line);
}

bool DicomSCP::start()
{
    m_process->start();
    return m_process->waitForStarted();
}

bool DicomSCP::stop()
{
    m_process->close();
    m_process->waitForFinished();

    m_logWorker->terminate();
    m_logWorker->wait();

    return true;
}

