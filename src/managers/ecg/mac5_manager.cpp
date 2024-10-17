#include "mac5_manager.h"

#include "auxiliary/file_utils.h"
#include "auxiliary/network_utils.h"

#include "data/ecg/tests/mac5_test.h"

#include <QMessageBox>

#include <QDomDocument>

#include <QMap>
#include <QPair>
#include <QJsonArray>

DeviceConfig Mac5Manager::config {{
    {"exportPath", {"mac5/exportPath", Dir }},
}};

Mac5Manager::Mac5Manager(QSharedPointer<Mac5Session> session) : ManagerBase(std::move(session))
{
    m_exportPath = config.getSetting("exportPath");
    m_test.reset(new Mac5Test);
}

bool Mac5Manager::start()
{
    // Remove any existing data
    FileUtils::clearDirectory(m_exportPath);

    // Listen to directory changes
    m_directoryWatcher.reset(new DicomDirectoryWatcher(m_exportPath));

    // Whenever it changes, get the files
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

    qDebug() << "pdf" << m_pdfFilePath;
    qDebug() << "xml" << m_xmlFilePath;
    qDebug() << "raw" << m_rawFilePath;

    if (m_pdfFilePath.isEmpty())
    {
        qCritical() << "pdf path missing";
        return;
    }

    if (m_xmlFilePath.isEmpty())
    {
        qCritical() << "xml path missing";
        return;
    }

    if (m_rawFilePath.isEmpty())
    {
        qCritical() << "raw path missing";
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
        { { "path", m_xmlFilePath }, { "name", "Ecg.xml" }},
        { { "path", m_pdfFilePath }, { "name", "Ecg.pdf" }},
        { { "path", m_rawFilePath }, { "name", "Ecg.ecg" }},
    };
    m_test->setFiles(filePaths);

    ManagerBase::finish();
}

void Mac5Manager::measure()
{

}
