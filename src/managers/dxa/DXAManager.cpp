#include "managers/dxa/DXAManager.h"

#include <QMap>
#include <QVariant>
#include <QString>
#include <QSql>
#include <QProcess>
#include <QSettings>
#include <QException>

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdeftag.h"

const QMap<QString, QString> DXAManager::ranges = {
    // forearm
    { "RU13TOT_BMD",    "1.." },
    { "RUMIDTOT_BMD",   ".2." },
    { "RUUDTOT_BMD",    "..3" },
    { "RUTOT_BMD",      "123" },
    { "R_13_BMD",       "R.." },
    { "R_MID_BMD",      ".R." },
    { "R_UD_BMD",       "..R" },
    { "RTOT_BMD",       "RRR" },
    { "U_13_BMD",       "U.." },
    { "U_MID_BMD",      ".U." },
    { "U_UD_BMD",       "..U" },
    { "UTOT_BMD",       "UUU" },

    // whole body
    { "WBTOT_BMD",      "NULL" },

    // hip
    { "NECK_BMD",       "1..." },
    { "TROCH_BMD",      ".2.." },
    { "INTER_BMD",      "..3." },
    { "WARDS_BMD",      "...4" },
    { "HTOT_BMD",       "123." },

    // ap lumbar spine
    { "L1_BMD",         "1..." },
    { "L2_BMD",         ".2.." },
    { "L3_BMD",         "..3." },
    { "L4_BMD",         "...4" },
    { "TOT_BMD",        "1234" },
    { "TOT_L1_BMD",     "1..." },
    { "TOT_L2_BMD",     ".2.." },
    { "TOT_L3_BMD",     "..3." },
    { "TOT_L4_BMD",     "...4" },
    { "TOT_L1L2_BMD",   "12.." },
    { "TOT_L1L3_BMD",   "1.3." },
    { "TOT_L1L4_BMD",   "1..4" },
    { "TOT_L2L3_BMD",   ".23." },
    { "TOT_L2L4_BMD",   ".2.4" },
    { "TOT_L3L4_BMD",   "..34" },
    { "TOT_L1L2L3_BMD", "123." },
    { "TOT_L1L2L4_BMD", "12.4" },
    { "TOT_L1L3L4_BMD", "1.34" },
    { "TOT_L2L3L4_BMD", ".234" },
};

DXAManager::DXAManager(QObject *parent)
    : ManagerBase{parent}, m_dicomSCP(new DicomSCP(parent))
{
}

DXAManager::~DXAManager()
{
    m_dicomSCP->stop();
    delete m_dicomSCP;
}

void DXAManager::loadSettings(const QSettings &)
{

}

void DXAManager::saveSettings(QSettings*) const
{

}

void loadSettings(const QSettings &)
{

}

void saveSettings(QSettings*)
{

}

bool DXAManager::validateDicomFile(DcmFileFormat loadedFileFormat)
{
    return false;
}

void DXAManager::dicomFilesReceived(QStringList paths)
{
    QList<DcmFileFormat> validatedDicomFiles = getValidatedFiles(paths);
    QVariantMap scanAnalysisData = extractScanAnalysisData();
    QVariantMap scores = computeTandZScores();

    QJsonObject scanAnalysisJson = QJsonObject::fromVariantMap(scanAnalysisData);
    QJsonObject scoresJson = QJsonObject::fromVariantMap(scanAnalysisData);
}

QList<DcmFileFormat> DXAManager::getValidatedFiles(QStringList filePaths)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
    QList<DcmFileFormat> validatedDicomFiles;

    DcmFileFormat fileFormat;
    QList<QString>::const_iterator i;

    try {
        for (i = filePaths.begin(); i != filePaths.end(); ++i)
        {
            const QString filePath = *i;
            QString fileName = settings.value("dicom/out_dir", "").toString() + "/" + filePath.toStdString().c_str();
            fileName = fileName.replace("/", "\\");

            OFCondition status = fileFormat.loadFile(fileName.toStdString().c_str());
            if (status.good())
            {
                bool isValid = validateDicomFile(fileFormat);
                if (isValid)
                {
                    qDebug() << "DICOM File is valid: " << isValid;
                    validatedDicomFiles.append(fileFormat);
                }
            }
            else
            {
              qDebug() << "Error: cannot read DICOM file (" << status.text() << ")" << endl;
            }
        }
    }

    catch (QException &e)
    {
        qDebug() << e.what();
    }

    return validatedDicomFiles;
}

bool DXAManager::startDicomServer()
{
    return m_dicomSCP->start();
}

bool DXAManager::endDicomServer()
{
    //return m_dicomSCP->stop();
    return true;
}

void DXAManager::dicomServerExitNormal()
{

}

void DXAManager::dicomServerExitCrash()
{

}
