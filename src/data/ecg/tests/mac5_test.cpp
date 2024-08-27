#include "data/ecg/tests/mac5_test.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

Mac5Test::Mac5Test() {}

QString Mac5Test::toString() const
{
    return QString("");
}

bool Mac5Test::isValid() const
{
    return true;
}

QJsonObject Mac5Test::toJsonObject() const
{
    return TestBase::toJsonObject();
}

QJsonDocument Mac5Test::fromXmlFile(const QString& filePath)
{
    QJsonDocument jsonDoc;
    QDomDocument doc("ecg_xml");
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Can't open";
        return jsonDoc;
    }

    if (!doc.setContent(&file)) {
        file.close();
        return jsonDoc;
    }

    file.close();

    QJsonObject root = dfs(doc.documentElement());
    jsonDoc.setObject(root);

    //qDebug().noquote() << jsonDoc.toJson(QJsonDocument::Indented);

    return jsonDoc;
}


QJsonObject Mac5Test::dfs(QDomNode node)
{
    QJsonObject result;
    QDomElement el = node.toElement();
    QString tagName = el.tagName();

    if (tagName == "wav")
        return result;

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
            result.insert(attr.name().toLower(), attr.value().toLower());
        }
    }

    QMap<QString, QJsonArray> seenTags;
    if (node.hasChildNodes()) {
        for (int i = 0; i < node.childNodes().length(); i++) {
            QDomElement childEl = node.childNodes().at(i).toElement();
            QString childTag = childEl.tagName();
            QJsonObject childJson = dfs(node.childNodes().at(i));

            result.insert(childTag.toLower(), childJson);

            if (!seenTags.contains(childTag)) {
                seenTags.insert(childTag, QJsonArray {});
            }
            else {
                seenTags[childTag].append(childJson);
            }
        }
    }

    for (auto it = seenTags.begin(); it != seenTags.end(); ++it) {
        QString key = it.key();
        QJsonArray arr = it.value();

        if (!arr.empty()) {
            result.insert(key.toLower(), it.value());
        }
    }

    return result;
}
