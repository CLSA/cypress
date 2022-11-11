#include <QFileSystemWatcher>
#include <QFile>
#include <QDir>
#include <QDebug>

#include <signal.h>

#include "DicomSCP.h"

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


    // args at https://support.dcmtk.org/docs/dcmrecv.html
    QStringList args;
    args
        << "-xf" << "../etc/dcmtk/storescp.cfg"
        << "default"
        << "--filename-extension" << ".dcm"
        << "-od" << "../storage"
        << "-lc" << "../etc/dcmtk/filelog.cfg"
        << "+fst"
        << "9001";

    m_process->setWorkingDirectory("C:/work/clsa/cypress/dcmtk-3.6.7/bin");
    m_process->setProgram("C:/work/clsa/cypress/dcmtk-3.6.7/bin/dcmrecv.exe");
    m_process->setArguments(args);

    QStringList list;
    list << "C:/work/clsa/cypress/dcmtk-3.6.7/storage/";

    m_fileSystemWatcher.reset(new QFileSystemWatcher(list));
}

void DicomSCP::initConnections()
{
    initProcess();
    start();

    connect(m_fileSystemWatcher.get(), &QFileSystemWatcher::directoryChanged, this, [this](const QString& path) {
        QStringList nameFilters;
        nameFilters << "*.dcm";

        QDir dir;
        QStringList dicomFilePaths;

        dir.setPath(path);
        dir.setNameFilters(nameFilters);
        dir.setSorting(QDir::SortFlag::Time);

        dicomFilePaths = dir.entryList();

        QStringList::Iterator iterator;
        for (iterator = dicomFilePaths.begin(); iterator != dicomFilePaths.end(); ++iterator)
        {
           qDebug() << (*iterator).toLocal8Bit().constData();
        }

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

