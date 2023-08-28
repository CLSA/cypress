#include "vividi_manager.h"
#include "auxiliary/file_utils.h"
#include "cypress_application.h"

VividiManager::VividiManager(const CypressSession& session)
    : ManagerBase(session)
{
    m_dcmRecv.reset(new DcmRecv(
        "C:/work/clsa/cypress/dep/dcmtk-3.6.7-win32-install/bin/dcmrecv.exe",
        "C:/work/clsa/cypress/dep/dcmtk-3.6.7-win32-install/etc/dcmtk/storescp.cfg",
        "C:/Users/antho/Documents/dicom",
        "CLSADICOM",
        "9001"));

    m_dcmWatcher.reset(new DicomDirectoryWatcher("C:/Users/antho/Documents/dicom"));
    connect(m_dcmWatcher.get(), &DicomDirectoryWatcher::dicomDirectoryChanged, [=]() {
        // perform validation on files
        // update UI
        QDir directory("C:/Users/antho/Documents/dicom");
        QStringList dcmFiles = directory.entryList(QStringList() << "*.dcm", QDir::Files);

        foreach(QString filename, dcmFiles) {
            QFileInfo fileInfo(directory, filename);

            if(fileInfo.isReadable() && fileInfo.isFile()) {
                qDebug() << fileInfo.absoluteFilePath() << "is a readable .dcm file.";
            }
        }
    });
}

bool VividiManager::isAvailable()
{
    return isInstalled();
}

bool VividiManager::isInstalled()
{
    return FileUtils::doesExeExist("C:/work/clsa/cypress/dep/dcmtk-3.6.7-win32-install/bin/dcmrecv.exe") && \
           FileUtils::doesFileExist("C:/work/clsa/cypress/dep/dcmtk-3.6.7-win32-install/etc/dcmtk/storescp.cfg", false) && \
           FileUtils::doesDirExist("C:/Users/antho/Documents/dicom", true);
}

void VividiManager::setInputData(const QVariantMap& inputData)
{
    m_inputData = inputData;
}

void VividiManager::start()
{
    if (!isAvailable())
    {
        return;
    }

    bool started = m_dcmRecv->start();
    if (!started)
    {
        return;
    }
}

void VividiManager::measure()
{
    if (Cypress::getInstance().isSimulation()) {
      sendResultsToPine("C:/dev/clsa/cypress/src/tests/fixtures/ultrasound/output.json");
    }

    //results["cypress_session"] = m_uuid;
    //results["answer_id"] = m_answerId;
    //results["barcode"] = m_barcode;
    //results["interviewer"] = m_interviewer;

    //if (results.empty()) return;

    //bool ok = sendResultsToPine(results);
    //if (!ok)
    //{
    //    qDebug() << "Could not send results to Pine";
    //}
}

void VividiManager::finish()
{

}


bool VividiManager::setUp()
{
    return true;
}

bool VividiManager::clearData()
{
    return true;
}

bool VividiManager::cleanUp()
{
    return true;
}
