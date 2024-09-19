
#include "easyone_test.h"

void EasyoneTest::readTrials(const QDomNode& node)
{
    QDomNodeList trialNodeList = node.childNodes();
    if (trialNodeList.isEmpty()) {
      qDebug() << "no trial child nodes";
      return;
    }

    qDebug() << "n trials" << trialNodeList.size();

    for (int i = 0; i < trialNodeList.size(); i++) {
      QSharedPointer<SpirometerMeasurement> trial(new SpirometerMeasurement);
      trial->setResultType(SpirometerMeasurement::ResultType::typeTrial);
      QDomNode trialNode = trialNodeList.at(i);

      if ("Trial" != trialNode.toElement().tagName()) {
        continue;
      }

      QDomNodeList nodeList = trialNode.childNodes();

      for (int j = 0; j < nodeList.size(); j++) {
       QDomNode node = nodeList.at(j);
       QDomElement elem = node.toElement();
       QString tag = elem.tagName();

       if ("ResultParameters" == tag) {
          readParameters(node, trial.get());
       } else if (tag.startsWith("Channel")) {
          readChannel(node, trial.get());
       } else if (SpirometerMeasurement::trialMap.contains(tag)) {
          QString s = elem.text();
          if (s.isEmpty()) {
            trial->setAttribute(SpirometerMeasurement::trialMap[tag], QVariant());
          } else {
            if ("true" == s || "false" == s) {
              trial->setAttribute(SpirometerMeasurement::trialMap[tag], "true" == s ? true : false);
            } else {
              if ("Date" == tag) {
                trial->setAttribute(SpirometerMeasurement::trialMap[tag],
                                      QDateTime::fromString(s.left(23), "yyyy-MM-dd'T'hh:mm:ss.zzz"));
              } else {
                trial->setAttribute(SpirometerMeasurement::trialMap[tag], s.toInt());
              }
            }
          }
       }
      }
      if (trial->isValid()) {
       addMeasurement(trial);
      } else {
       qDebug() << "ERROR: failed to add trial";
      }
    }

}

void EasyoneTest::readPDFReportPath(const QDomNode& node)
{
    QDomElement child = node.lastChildElement("Parameter");
    if (child.isNull()) {
        qDebug() << "readPDFReportPath: child parameter is null";
        return;
    }

    qDebug() << child.hasAttribute("Name") << child.tagName() << child.text() << child.attribute("Name");

    if (child.hasAttribute("Name") && "Attachment" == child.attribute("Name")) {
        qDebug() << "reading report path";
        addMetaData("pdf_report_path", child.text());
    }
}
