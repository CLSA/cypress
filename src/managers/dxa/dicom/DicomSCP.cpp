#include <QFileSystemWatcher>
#include <QFile>
#include <QDir>
#include <QDebug>

#include <signal.h>

#include "DicomSCP.h"
#include <QSettings>

DicomSCP::DicomSCP(QObject *parent)
    : QObject{parent}
{
    initConnections();
}

DicomSCP::~DicomSCP() {
}

void DicomSCP::initProcess()
{
    // args at https://support.dcmtk.org/docs/dcmrecv.html
    m_process.reset(new QProcess);

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
    QStringList args;

    args
        << "+fst"
        << settings.value("dicom/port").toString()
        << "--config-file"
        << "C:/work/clsa/cypress/dcmtk-3.6.7/etc/dcmtk/storescp.cfg"
        << "default"
        << "--log-config"
        << "C:/work/clsa/cypress/dcmtk-3.6.7/etc/dcmtk/logger.cfg"
        << "--output-directory"
        << settings.value("dicom/out_dir").toString();

    //qDebug() << "args" << args;

    m_process->setWorkingDirectory(settings.value("dicom/working_dir", "").toString());
    m_process->setProgram(settings.value("dicom/program", "").toString());
    m_process->setArguments(args);

    QStringList list;
    list << settings.value("dicom/out_dir").toString();
    qDebug() << list;

    m_fileSystemWatcher.reset(new QFileSystemWatcher(list));
}

void DicomSCP::initConnections()
{
    initProcess();
    //start();

    connect(m_fileSystemWatcher.get(), &QFileSystemWatcher::directoryChanged, this, [this](const QString& path) {
        QDir dir;
        QStringList dicomFilePaths;

        dir.setPath(path);
        dir.setSorting(QDir::SortFlag::Time);

        dicomFilePaths = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);

        qDebug() << "dicom files received: " << dicomFilePaths;

        emit dicomFilesReceived(dicomFilePaths);
    });

    connect(m_fileSystemWatcher.get(), &QFileSystemWatcher::fileChanged, this, []() {
        qDebug() << "file changed";
    });

    connect(m_fileSystemWatcher.get(), &QFileSystemWatcher::destroyed, this, []() {
        qDebug() << "destroyed";
    });

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
        qDebug() << output;
        emit logUpdate(output);
    });

    connect(m_process.get(),
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            [=](int exitCode, QProcess::ExitStatus exitStatus)
            {
                qInfo() << "DICOM: exit" << exitCode << exitStatus;
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
    bool ok = m_process->waitForStarted();
    qDebug() << ok;
    return ok;
}

bool DicomSCP::stop()
{
    m_process->close();
    return m_process->waitForFinished();
}
