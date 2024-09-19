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
    qDebug() << "Finish";
    QDomDocument doc("ecg_xml");
    QFile file(m_xmlFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Can't open";
        return;
    }

    if (!doc.setContent(&file)) {
        qCritical() << "Could not set XML file";
        file.close();
        return;
    }

    file.close();

    QJsonObject root = dfs(doc.documentElement());
    QJsonDocument jsonDoc(root);

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

QJsonObject Mac5Manager::dfs(QDomNode node)
{

    // The result object
    QJsonObject result;
    QDomElement el = node.toElement();
    QString tagName = el.tagName();

    // Put all the attributes into the result object
    for (int i = 0; i < el.attributes().length(); i++)
    {
        auto attr = el.attributes().item(i).toAttr();

        if (attr.name() == "V") {
            result.insert("value", attr.value());
        }
        else if (attr.name() == "U") {
            result.insert("unit", attr.value());
        }
        else {
            result.insert(attr.name(), attr.value());
        }
    }


    // Add children nodes to the object recursively,
    // if it's a duplicate tag put all duplicates into QJsonArray child

    QMap<QString, QJsonArray> seenTags;
    if (node.hasChildNodes())
    {
        for (int i = 0; i < node.childNodes().length(); i++)
        {
            QDomElement childEl = node.childNodes().at(i).toElement();
            QString childTag = childEl.tagName();
            QJsonObject childJson = dfs(node.childNodes().at(i));

            result.insert(childTag, childJson);

            if (!seenTags.contains(childTag))
            {
                seenTags.insert(childTag, QJsonArray {});
            }
            else
            {
                seenTags[childTag].append(childJson);
            }
        }
    }

    for (auto it = seenTags.begin(); it != seenTags.end(); ++it)
    {
        QString key = it.key();
        QJsonArray arr = it.value();

        if (!arr.empty())
            result.insert(key, it.value());
    }

    return result;
}
