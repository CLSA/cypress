#include "dcm_recv.h"
#include "dicom_directory_watcher.h"

#include "auxiliary/file_utils.h"
#include "cypress_settings.h"

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
    m_debug = CypressSettings::isDebugMode();

    if (!clearOutputDirectory())
        qDebug() << "DcmRecv::DcmRecv - could not clear output directory";

    if (!configureProcess())
        qDebug() << "DcmRecv::DcmRecv - could not configure process";

    if (!initializeOutputWatcher())
        qDebug() << "DcmRecv::DcmRecv - could not start directory watcher";
}

DcmRecv::~DcmRecv()
{
    m_process.kill();
    m_process.waitForFinished();

    clearOutputDirectory();
}

bool DcmRecv::initializeOutputWatcher()
{
    m_watcher.reset(new DicomDirectoryWatcher(m_outputDir));
    connect(m_watcher.get(),
            &DicomDirectoryWatcher::dicomDirectoryChanged,
            this,
            &DcmRecv::onFilesReceived);

    return true;
}

bool DcmRecv::clearOutputDirectory()
{
    // Reset the output directory
    QDir outputDir(m_outputDir);

    if (outputDir.exists())
        outputDir.removeRecursively();

    if (!outputDir.mkpath(m_outputDir)) {
        qDebug() << "DCMRECV: could not create the output directory";
        return false;
    }

    return true;
}

bool DcmRecv::configureProcess()
{
    if (!FileUtils::clearDirectory(m_outputDir)) {
        qDebug() << "DCMRECV: could not could not clear output directory";
        return false;
    }

    connect(&m_process,
            &QProcess::readyReadStandardOutput,
            this,
            &DcmRecv::onReadyReadStandardOutput);
    connect(&m_process, &QProcess::readyReadStandardError, this, &DcmRecv::onReadyReadStandardError);
    connect(&m_process, &QProcess::errorOccurred, this, &DcmRecv::onErrorOccurred);
    connect(&m_process,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            &DcmRecv::onFinished);

    return true;
}

void DcmRecv::onFilesReceived()
{
    QList<DicomFile> receivedFiles;

    // perform validation on files
    // update UI
    QDir outputDir(m_outputDir);
    QStringList dcmFiles = outputDir.entryList(QStringList() << "*", QDir::Files);

    foreach(QString fileName, dcmFiles) {
        QFileInfo fileInfo(outputDir, fileName);

        if(fileInfo.isReadable() && fileInfo.isFile()) {

            DcmFileFormat fileFormat;
            OFCondition loadResult = fileFormat.loadFile(fileInfo.absoluteFilePath().toStdString().c_str());

            if (loadResult.good())
            {
                DcmDataset* dataset = fileFormat.getDataset();

                OFString mediaStorageUID;
                OFString studyInstanceUID;
                OFString patientId;
                OFString fileName;
                OFString modality;
                OFString studyDate;
                OFString bodyPartExamined;
                OFString seriesNumber;
                OFString laterality;

                DicomFile dicomFile;
                dicomFile.absFilePath = fileInfo.absoluteFilePath();

                if (dataset->findAndGetOFString(DCM_SOPClassUID, mediaStorageUID).good()) {
                    qDebug() << mediaStorageUID.c_str();
                    dicomFile.mediaStorageUID = mediaStorageUID.c_str();
                } else {
                    qDebug() << "couldn't find media storage";
                }

                if (dataset->findAndGetOFString(DCM_Laterality, laterality).good()) {
                    dicomFile.laterality = laterality.c_str();
                }

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

                if (dataset->findAndGetOFString(DCM_SeriesNumber, seriesNumber).good()) {
                    dicomFile.seriesNumber = seriesNumber.c_str();
                }

                qInfo() << "DICOM file";
                qInfo() << dicomFile.mediaStorageUID;
                qInfo() << dicomFile.laterality;
                qInfo() << dicomFile.studyId;
                qInfo() << dicomFile.patientId;
                qInfo() << dicomFile.bodyPartExamined;
                qInfo() << dicomFile.modality;
                qInfo() << dicomFile.studyDate;
                qInfo() << dicomFile.seriesNumber;
                qInfo() << "\n";

                receivedFiles.append(dicomFile);
            }
        }
    }

    emit dicomFilesReceived(receivedFiles);
}

bool DcmRecv::start()
{
    QStringList arguments;

    arguments << m_port <<
        "--config-file" << m_configPath << "default" <<
        "--aetitle" << m_aeTitle <<
        "--output-directory" << m_outputDir <<
        "--filename-extension" << ".dcm";

    qDebug() << m_executablePath;

    m_process.start(m_executablePath, arguments);
    if (!m_process.waitForStarted()) {
        qDebug() << "DcmRecv: could not start DICOM server" << m_process.errorString();
        emit notRunning();
    }

    qDebug() << "started dicom server" << arguments;

    emit running();

    return true;
}

bool DcmRecv::stop()
{
    qDebug() << "kill dcmrecv" << endl;
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
}

void DcmRecv::onReadyReadStandardError()
{
    QByteArray error = m_process.readAllStandardError();
    qDebug() << "DCMRECV stderr:" << error;
}

void DcmRecv::onErrorOccurred(QProcess::ProcessError error)
{
    qWarning() << "DCMRECV process error:" << error;
}

void DcmRecv::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "DCMRECV process finished with exit code" << exitCode << "and exit status"
             << exitStatus;

    if (exitCode == 0)
        emit exitNormal();
    else
        emit exitCrash();
}
