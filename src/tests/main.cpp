#include <QTest>

#include <atlbase.h>
#include <atldbcli.h>
#include <atldbsch.h> // if you are using schema template

#include "../data/ecg/tests/mac5_test.h"
#include "../data/spirometer/tests/easyone_test.h"
#include "../data/ultrasound/vivid_iq_test.h"
#include "../data/blood_pressure/tests/watch_bp_test.h"
#include "../data/retinal_camera/oct_test.h"
#include "../data/tonometer/tests/ora_test.h"

#include <QJsonDocument>
#include <QList>

bool writeToFile(const QByteArray& data, const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open the file.");
        return false;
    }

    QTextStream out(&file);
    out << data;
    file.close();

    return true;
}

int main(int argc, char *argv[])
{
    Mac5Test mac5Test;
    QJsonDocument doc = mac5Test.fromXmlFile("//ncc-ad/Users/hoarea/Desktop/Mac5Export/backup/SUE24291774SA_20240819_084519.Xml");
    mac5Test.setFiles(QList<QJsonObject> {
        { { "path", "//ncc-ad/Users/hoarea/Desktop/Mac5Export/ecg.pdf"}, {"name", "Ecg.pdf" } },
        { { "path", "//ncc-ad/Users/hoarea/Desktop/Mac5Export/ecg.Xml" }, {"name", "Ecg.xml"} },
        { { "path", "//ncc-ad/Users/hoarea/Desktop/Mac5Export/raw.ecg" }, {"name", "Ecg.ecg"} },
    });
    if (writeToFile(
            QJsonDocument(mac5Test.toJsonObject()).toJson(QJsonDocument::Indented),
            "C:/Users/hoarea/mac5_output.json")) {
        qDebug() << "JSON written to file successfully.";
        qDebug() << "Is Valid: " << mac5Test.isValid();
    }

    EasyoneTest easyoneTest;
    easyoneTest.fromFile("//ncc-ad/Users/hoarea/Desktop/EasyOneBackup/Cypress_Out.xml");
    easyoneTest.setFiles(QList<QJsonObject> {
        { {"path", "//ncc-ad/Users/hoarea/Desktop/EasyOneBackup/5000000.pdf" }, {"name", "report.pdf"} },
        { {"path", "//ncc-ad/Users/hoarea/Desktop/EasyOneBackup/Cypress_Out.xml" }, {"name", "data.xml"} }
    });
    if (writeToFile(
            QJsonDocument(easyoneTest.toJsonObject()).toJson(QJsonDocument::Indented),
            "C:/Users/hoarea/easyone_output.json")) {
        qDebug() << "JSON written to file successfully.";
        qDebug() << "Is Valid: " << easyoneTest.isValid();
    }

    VividIQTest vividIqTest;
    vividIqTest.setFiles(QList<QJsonObject> {
        {{ "path", "//ncc-ad/Users/hoarea/Desktop/VividIQExport/SRc.1.2.840.113619.2.435.288026096.1724601158.5080.3.dcm"     }, {"name", "SRc.1.2.840.113619.2.435.288026096.1724601158.5080.3.dcm" }},
        {{ "path", "//ncc-ad/Users/hoarea/Desktop/VividIQExport/USm.1.2.840.113619.2.435.288026096.1721818439.2656.3.512.dcm" }, {"name", "USm.1.2.840.113619.2.435.288026096.1721818439.2656.3.512.dcm"}},
        {{ "path", "//ncc-ad/Users/hoarea/Desktop/VividIQExport/USm.1.2.840.113619.2.435.288026096.1721818458.2656.4.512.dcm" }, {"name", "USm.1.2.840.113619.2.435.288026096.1721818458.2656.4.512.dcm"}},
    });
    if (writeToFile(
            QJsonDocument(vividIqTest.toJsonObject()).toJson(QJsonDocument::Indented),
            "C:/Users/hoarea/vivid_iq_output.json")) {
        qDebug() << "JSON written to file successfully.";
        qDebug() << "Is Valid: " << easyoneTest.isValid();
    }

    WatchBPTest watchBpTest;
    watchBpTest.fromJson({
        {},
        {},
        {},
    });
    if (writeToFile(
            QJsonDocument(watchBpTest.toJsonObject()).toJson(QJsonDocument::Indented),
            "C:/Users/hoarea/watch_bp_output.json")) {
        qDebug() << "JSON written to file successfully.";
        qDebug() << "Is Valid: " << watchBpTest.isValid();
    }

    OCTTest octTest;
    if (writeToFile(
            QJsonDocument(octTest.toJsonObject()).toJson(QJsonDocument::Indented),
            "C:/Users/hoarea/oct_output.json")) {
        qDebug() << "JSON written to file successfully.";
        qDebug() << "Is Valid: " << watchBpTest.isValid();
    }

    ORATest oraTest;
    if (writeToFile(
            QJsonDocument(oraTest.toJsonObject()).toJson(QJsonDocument::Indented),
            "C:/Users/hoarea/ora_output.json")) {
        qDebug() << "JSON written to file successfully.";
        qDebug() << "Is Valid: " << watchBpTest.isValid();
    }


    return 0;
}
