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

    QJsonObject root = dfs(doc.documentElement()).toObject();
    for (QJsonObject::const_iterator it = root.begin(); it != root.end(); ++it) {
        QString key = it.key();
        QJsonValue value = it.value();

        // Print the key and the value
        addMetaData(key, value);
    }

    return jsonDoc;
}



QJsonValue Mac5Test::dfs(const QDomNode& node)
{
    QSet<QString> excludedNodes { "wav" };
    QSet<QString> excludedUnits { "{enum}", "{unitless}", "{bool}" };
    QSet<QString> excludedAttributes { "BT", "INV" };

    QJsonObject result;
    QDomElement el = node.toElement();
    QString tagName = el.tagName().toLower();

    if (excludedNodes.contains(tagName))
        return QJsonValue();

    if ((el.hasAttribute("V") && !el.hasAttribute("U")) || excludedUnits.contains(el.attribute("U"))) {
        if (el.attribute("V") == "false" || el.attribute("V") == "true")
            return QJsonValue(el.attribute("V") == "true");

        return QJsonValue(el.attribute("V"));
    }

    QJsonObject attributeData;
    for (int i = 0; i < el.attributes().count(); ++i) {
        const QDomNode attrNode = el.attributes().item(i);
        const QString attrName = attrNode.nodeName();
        const QString attrValue = attrNode.nodeValue();

        if (excludedAttributes.contains(attrName))
            continue;
        else if (attrName == "U")
            attributeData["unit"] = attrValue;
        else if (attrName == "V")
            attributeData["value"] = attrValue;
        else
            attributeData[attrName.toLower()] = attrValue;
    }

    if (!node.hasChildNodes())
        return QJsonValue(attributeData);

    QMap<QString, QJsonArray> arrayTags;
    for (int i = 0; i < node.childNodes().length(); i++) {
        QDomElement childEl = node.childNodes().at(i).toElement();
        QString childTag = childEl.tagName();
        QJsonValue childJson = dfs(node.childNodes().at(i));

        if (excludedNodes.contains(childTag))
            continue;

        if (hasMultipleChildrenOfSameType(node, childTag)) {
            if (childJson.isObject()) {
                for (int i = 0; i < childEl.attributes().count(); ++i) {
                    const QDomNode attrNode = childEl.attributes().item(i);
                    const QString attrName = attrNode.nodeName();
                    const QString attrValue = attrNode.nodeValue();

                    QJsonObject json = childJson.toObject();
                    json[attrName] = attrValue;
                    childJson = json;
                }
            }

            if (!arrayTags.contains(childTag)) {
                arrayTags.insert(childTag, QJsonArray { childJson });
            }
            else {
                arrayTags[childTag].append(childJson);
            }
        }
        else {
            result.insert(childTag, childJson);
        }
    }

    for (auto it = arrayTags.constBegin(); it != arrayTags.constEnd(); ++it) {
        const QString key = it.key().toLower();
        const QJsonArray arr = it.value();

        if (!arr.empty()) {
            result.insert(key, it.value());
        }
    }

    return result;
}

bool Mac5Test::hasMultipleChildrenOfSameType(const QDomNode& parentNode, const QString& tagName) {
    int count = 0;
    QDomNodeList childNodes = parentNode.childNodes();

    for (int i = 0; i < childNodes.size(); ++i) {
        QDomNode child = childNodes.at(i);
        if (child.isElement()) {
            QDomElement element = child.toElement();
            if (element.tagName() == tagName) {
                count++;
            }
        }
    }
    return count > 1;
}
