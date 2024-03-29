#include <QCoreApplication>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

/*
 * Useage: create a directory for the spirometer manager
 * to use as a surrogate for EasyWarePro.exe (ie., spirometer_emulator.exe)
 * Use the same folder as the path to the EMR plugin transfer
 * directory.  Copy Cypress*.xml from cypress repository to
 * Cypress*_default.xml and create dummy mdb files
 * EasyWarePro.mdb, EwpOptions.mdb.  Copy default_12345678.pdf
 * from cypress repository to 12345678.pdf
 * The test barcode can be anything: the emulator will
 * replace the ID found in the CypressIn.xml generated by the
 * EMRPluginWriter during configuration by the manager.
 */

#include <QDomDocument>

int main(int argc, char *argv[])
{
    Q_UNUSED(argc)

    QFileInfo info(argv[0]);
    QDir path = info.absolutePath();
    QString infile1 = path.filePath("CypressIn_default.xml");
    QString infile2 = path.filePath("CypressOut_default.xml");
    QString infile3 = path.filePath("default_12345678.pdf");
    QString patient_id;
    if(QFileInfo::exists(infile1))
    {
        QString ofile = path.filePath("CypressIn.xml");
        QFile::copy(infile1,ofile);
        QFile file(ofile);
        if(file.open(QIODevice::ReadWrite|QIODevice::Text))
        {
          QDomDocument doc;
          if(doc.setContent(&file))
          {
            QDomElement docElem = doc.documentElement();
            QDomElement child = docElem.firstChildElement("Patients");
            if(!child.isNull())
            {
              child = child.firstChildElement("Patient");
              if(child.hasAttribute("ID"))
              {
                patient_id = child.attribute("ID");
              }
            }
          }
        }
        file.close();
    }
    if(QFileInfo::exists(infile2))
    {
        QString ofile = path.filePath("CypressOut.xml");
        QFile::copy(infile2,ofile);
        QFile file(ofile);
        if(file.open(QIODevice::ReadWrite|QIODevice::Text) && !patient_id.isEmpty())
        {
          QDomDocument doc("template");
          if(doc.setContent(&file))
          {
            QDomElement docElem = doc.documentElement();
            QString attachmentName = path.filePath(QString("%1.pdf").arg(patient_id));
            QDomNode elem = docElem.firstChild();
            QDomElement child = elem.firstChildElement("Parameter");

            if(!child.isNull() &&
                child.hasAttribute("Name") &&
                "Attachment" == child.attribute("Name"))
            {
              QDomNode textNode = child.namedItem("#text");
              if(!textNode.isNull() && textNode.isText())
              {
                QDomText t = textNode.toText();
                t.setData(attachmentName);
              }
            }

            elem = docElem.firstChildElement("Patients");
            if(!elem.isNull())
            {
              child = elem.firstChildElement("Patient");
              if(!child.isNull() && child.hasAttribute("ID"))
              {
                child.setAttribute("ID",patient_id);
              }
            }
          }

          file.resize(0);
          QTextStream stream;
          stream.setDevice(&file);
          doc.save(stream,2,QDomNode::EncodingPolicy::EncodingFromDocument);
          file.close();
        }
    }
    if(QFileInfo::exists(infile3) && !patient_id.isEmpty())
    {
        QString ofile = path.filePath(QString("%1.pdf").arg(patient_id));
        QFile::copy(infile3,ofile);
    }
    return EXIT_SUCCESS;
}
