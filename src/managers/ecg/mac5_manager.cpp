#include "mac5_manager.h"

#include "auxiliary/file_utils.h"
#include "data/ecg/tests/mac5_test.h"

#include <QMap>
#include <QPair>
#include <QJsonArray>
#include <QMessageBox>
#include <QDomDocument>

DeviceConfig Mac5Manager::config {{
    {"exportPath", {"mac5/exportPath", Dir }},
}};

Mac5Manager::Mac5Manager(QSharedPointer<Mac5Session> session) : ManagerBase(session)
{
    m_exportPath = config.getSetting("exportPath");
    m_test.reset(new Mac5Test(session));
}

bool Mac5Manager::start()
{
    // Ensure the export directory is empty and the test instance is empty
    clearData();

    // Listen to export directory changes
    m_directoryWatcher.reset(new DicomDirectoryWatcher(m_exportPath));

    // Whenever export directory changes, check the files and submit (if required files are in directory)
    connect(m_directoryWatcher.get(),
            &DicomDirectoryWatcher::dicomDirectoryChanged,
            this,
            &Mac5Manager::readOutput);

    return true;
}

void Mac5Manager::readOutput()
{
    QDir exportDir(m_exportPath);
    QFileInfoList entries = exportDir.entryInfoList();

    foreach (auto entry, entries)
    {
        if (entry.suffix().toLower() == "pdf")
        {
            m_pdfFilePath = entry.absoluteFilePath();
        }
        else if (entry.suffix().toLower() == "xml")
        {
            m_xmlFilePath = entry.absoluteFilePath();
        }
        else if (entry.suffix().toLower() == "ecg")
        {
            m_rawFilePath = entry.absoluteFilePath();
        }
    }

    if (m_pdfFilePath.isEmpty())
    {
        qDebug() << "Mac5Manager::readOutput - PDF path missing";
        return;
    }

    if (m_xmlFilePath.isEmpty())
    {
        qDebug() << "Mac5Manager::readOutput - XML path missing";
        return;
    }

    if (m_rawFilePath.isEmpty())
    {
        qDebug() << "Mac5Manager::readOutput - RAW path missing";
        return;
    }

    finish();
}

void Mac5Manager::finish()
{
    qDebug() << "Mac5Manager::finish";

    auto mac5Test = qSharedPointerCast<Mac5Test>(m_test);
    mac5Test->fromXmlFile(m_xmlFilePath);

    QList<QJsonObject> filePaths {
        {{ "path", m_xmlFilePath }, { "name", "Ecg.xml" }},
        {{ "path", m_pdfFilePath }, { "name", "Ecg.pdf" }},
        {{ "path", m_rawFilePath }, { "name", "Ecg.ecg" }},
    };
    mac5Test->setFiles(filePaths);

    try {
        mac5Test->validate();
    }
    catch (const Mac5::FilesMissingError& e) {
        emit error(e.what());
        return;
    }
    catch (const Mac5::IncorrectBarcodeError& e) {
        emit error(e.what());
        return;
    }
    catch (const QException& e) {
        emit error(e.what());
        return;
    }

    qInfo() << "Mac5Manager::finish - results are valid, sending to server";
    ManagerBase::finish();

    clearData();
}

bool Mac5Manager::clearData() {
    // Clear any existing data
    if (!FileUtils::clearDirectory(m_exportPath)) {
        qCritical() << "Mac5Manager::start - Could not clear export directory";
        return false;
    }

    m_test->reset();

    return true;
}

void Mac5Manager::measure()
{

}
