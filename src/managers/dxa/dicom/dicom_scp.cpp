#include <QFileSystemWatcher>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QException>
#include <QCoreApplication>

#include "dicom_scp.h"


DicomSCP::DicomSCP()
    : m_settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress")
{
    try {
        initConnections();
    } catch (const QException& e) {
        qCritical() << "Error initializing DicomSCP:" << e.what();
    }
}

DicomSCP::~DicomSCP() {
}

void DicomSCP::initProcess()
{
    m_process.reset(new QProcess);

    const QString basePath = QCoreApplication::applicationDirPath();
    QStringList args;

    qDebug() << basePath;

    args << "+fst"
         << m_settings.value("dicom/port").toString()
         << "--config-file"
         << basePath + "/dcmtk-3.6.7/etc/dcmtk/storescp.cfg"
         << "default"
         << "--log-config"
         << basePath + "/dcmtk-3.6.7/etc/dcmtk/logger.cfg"
         << "--output-directory"
         << m_settings.value("dicom/out_dir").toString();

    //args
    //    << "+fst"
    //    << m_settings.value("dicom/port").toString()
    //    << "--config-file"
    //    << "C:/dev/clsa/cypress/dcmtk-3.6.7/etc/dcmtk/storescp.cfg"
    //    << "default"
    //    << "--log-config"
    //    << "C:/dev/clsa/cypress/dcmtk-3.6.7/etc/dcmtk/logger.cfg"
    //    << "--output-directory"
    //    << m_settings.value("dicom/out_dir").toString();

    m_process->setWorkingDirectory(m_settings.value("dicom/working_dir", "").toString());
    m_process->setProgram(m_settings.value("dicom/program", "").toString());
    m_process->setArguments(args);

    QStringList list;
    list << m_settings.value("dicom/out_dir").toString();
    qDebug() << list;

    m_fileSystemWatcher.reset(new QFileSystemWatcher(list));
}

void DicomSCP::initConnections()
{
    initProcess();

    connect(m_fileSystemWatcher.get(), &QFileSystemWatcher::directoryChanged, this, &DicomSCP::onDirectoryChanged);
    connect(m_fileSystemWatcher.get(), &QFileSystemWatcher::fileChanged, this, &DicomSCP::onFileChanged);
    connect(m_fileSystemWatcher.get(), &QFileSystemWatcher::destroyed, this, &DicomSCP::onFileSystemWatcherDestroyed);

    connect(m_process.get(), &QProcess::stateChanged, this, &DicomSCP::onStateChanged);
    connect(m_process.get(), &QProcess::readyReadStandardOutput, this, &DicomSCP::onReadyReadStandardOutput);
    connect(m_process.get(),
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            &DicomSCP::onFinished);
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

void DicomSCP::onDirectoryChanged(const QString& path)
{
    QDir dir;
    QStringList dicomFilePaths;

    dir.setPath(path);
    dir.setSorting(QDir::SortFlag::Time);

    dicomFilePaths = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);

    qDebug() << "dicom files received: " << dicomFilePaths;

    emit dicomFilesReceived(dicomFilePaths);
}

void DicomSCP::onFileChanged()
{
    qDebug() << "file changed";
}

void DicomSCP::onFileSystemWatcherDestroyed()
{
    qDebug() << "destroyed";
}

void DicomSCP::onStateChanged()
{
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
            emit running();
            break;
    }
}

void DicomSCP::onReadyReadStandardOutput()
{
    QString output = m_process->readAllStandardOutput();
    qDebug() << output;
    emit logUpdate(output);
}

void DicomSCP::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qInfo() << "DICOM: exit" << exitCode << exitStatus;
    switch (exitStatus)
    {
        case QProcess::ExitStatus::NormalExit:
        emit exitNormal();
        break;
        case QProcess::ExitStatus::CrashExit:
        emit exitCrash();
        break;
    }
}

