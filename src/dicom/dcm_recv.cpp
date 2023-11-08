#include "dcm_recv.h"
#include "../auxiliary/file_utils.h"
#include "dicom_directory_watcher.h"


#include <QDebug>
#include <QMessageBox>



DcmRecv::DcmRecv(
    const QString& executablePath,
    const QString& configPath,
    const QString& outputDir,
    const QString& aeTitle,
    const QString& port,
    QObject* parent
):
    QObject(parent),
    m_executablePath(executablePath),
    m_configPath(configPath),
    m_outputDir(outputDir),
    m_aeTitle(aeTitle),
    m_port(port)
{
    // Reset the output directory
    FileUtils::removeDirectory(m_outputDir);
    FileUtils::createDirectory(m_outputDir);

    connect(&m_process, &QProcess::readyReadStandardOutput, this, &DcmRecv::onReadyReadStandardOutput);
    connect(&m_process, &QProcess::readyReadStandardError, this, &DcmRecv::onReadyReadStandardError);
    connect(&m_process, &QProcess::errorOccurred, this, &DcmRecv::onErrorOccurred);
    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &DcmRecv::onFinished);

    m_watcher.reset(new DicomDirectoryWatcher(outputDir));

    connect(m_watcher.get(), &DicomDirectoryWatcher::dicomDirectoryChanged, this, &DcmRecv::onFilesReceived);
}

DcmRecv::~DcmRecv()
{
    m_process.kill();
    m_process.waitForFinished();
}

void DcmRecv::onFilesReceived()
{
    receivedFiles.clear();

    // perform validation on files
    // update UI
    QDir directory(m_outputDir);
    QStringList dcmFiles = directory.entryList(QStringList() << "*", QDir::Files);

    foreach(QString fileName, dcmFiles) {
        QFileInfo fileInfo(directory, fileName);

        if(fileInfo.isReadable() && fileInfo.isFile()) {

            DcmFileFormat fileFormat;
            OFCondition loadResult = fileFormat.loadFile(fileInfo.absoluteFilePath().toStdString().c_str());

            if (loadResult.good())
            {
                DcmDataset* dataset = fileFormat.getDataset();
                OFString studyInstanceUID;
                OFString patientId;
                OFString fileName;
                OFString modality;
                OFString studyDate;
                OFString bodyPartExamined;

                DicomFile dicomFile;
                dicomFile.fileInfo = fileInfo;

                if (dataset->findAndGetOFString(DCM_StudyInstanceUID, studyInstanceUID).good()) {
                    dicomFile.studyId = studyInstanceUID.c_str();
                }

                if (dataset->findAndGetOFString(DCM_PatientID, patientId).good()) {
                    dicomFile.patientId = patientId.c_str();
                }

                if (dataset->findAndGetOFString(DCM_BodyPartExamined, bodyPartExamined).good()) {
                    dicomFile.bodyPartExamined = bodyPartExamined.c_str();
                }

                if (dataset->findAndGetOFString(DCM_Modality, modality).good()) {
                    dicomFile.modality = modality.c_str();
                }

                if (dataset->findAndGetOFString(DCM_StudyDate, studyDate).good()) {
                    dicomFile.studyDate = studyDate.c_str();
                }

                receivedFiles.append(dicomFile);
            }
        }
    }

    emit dicomFilesReceived();
}

bool DcmRecv::start()
{
    QStringList arguments;

    arguments << m_port <<
        "--config-file" << m_configPath << "default" <<
        "--aetitle" << m_aeTitle <<
        "--output-directory" << m_outputDir <<
        "--filename-extension" << ".dcm";

    m_process.start(m_executablePath, arguments);

    bool started = m_process.waitForStarted();
    if (!started)
    {
        qDebug() << "error: could not start DICOM server";
    }

    emit running();

    return started;
}

bool DcmRecv::stop()
{
    m_process.kill();
    return true;
}

QString DcmRecv::receivedFilesDir() const
{
    return m_outputDir;
}

void DcmRecv::onReadyReadStandardOutput()
{
    QByteArray output = m_process.readAllStandardOutput();
    qDebug() << "DCMRECV stdout:" << output;
    //emit logUpdate(output);
}

void DcmRecv::onReadyReadStandardError()
{
    QByteArray error = m_process.readAllStandardError();
    qDebug() << "DCMRECV stderr:" << error;
    //emit logUpdate(error);
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
