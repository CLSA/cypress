#include "vividi_manager.h"
#include "cypress_application.h"

#include "auxiliary/file_utils.h"

#include <QJsonDocument>

VividiManager::VividiManager(const CypressSession& session)
    : ManagerBase(session), m_test(new CimtVividiTest)
{
    //m_dcmRecv.reset(new DcmRecv(
    //    "C:/work/clsa/cypress/dep/dcmtk-3.6.7-win32-install/bin/dcmrecv.exe",
    //    "C:/work/clsa/cypress/dep/dcmtk-3.6.7-win32-install/etc/dcmtk/storescp.cfg",
    //    "C:/Users/antho/Documents/dicom",
    //    "CLSADICOM",
    //    "9001"));

    //m_dcmWatcher.reset(new DicomDirectoryWatcher("C:/Users/antho/Documents/dicom"));
    //connect(m_dcmWatcher.get(), &DicomDirectoryWatcher::dicomDirectoryChanged, [=]() {
    //    // perform validation on files
    //    // update UI
    //    QDir directory("C:/Users/antho/Documents/dicom");
    //    QStringList dcmFiles = directory.entryList(QStringList() << "*.dcm", QDir::Files);

    //    foreach(QString filename, dcmFiles) {
    //        QFileInfo fileInfo(directory, filename);

    //        if(fileInfo.isReadable() && fileInfo.isFile()) {
    //            qDebug() << fileInfo.absoluteFilePath() << "is a readable .dcm file.";
    //        }
    //    }
    //});
}

bool VividiManager::isAvailable()
{
    return false;
}

bool VividiManager::isInstalled()
{
    return false;
}

void VividiManager::setInputData(const QVariantMap& inputData)
{
    m_inputData = inputData;
}

void VividiManager::start()
{
    qDebug() << "started!";

    emit started(m_test);
    emit canMeasure();
}

void VividiManager::measure()
{
    m_test->reset();
    m_test->simulate();

    emit measured(m_test);

    if (m_test->isValid())
    {
      emit canFinish();
    }
}

void VividiManager::cancel()
{
    qDebug() << "VividIManager::cancel";
}

void VividiManager::finish()
{
    QJsonObject responseJson {};

    int answer_id = m_session.getAnswerId();

    for (int i = 0; i < m_test->getMeasurementCount(); i++)
    {
        Measurement& measure = m_test->get(i);
        const QString& side = measure.getAttribute("SIDE").toString();

        sendHTTPSRequest("PATCH", "https://blueberry.clsa-elcv.ca/qa/pine/api/answer/" + QString::number(answer_id) + "?filename=CINELOOP_1_" + side + ".dcm", "application/octet-stream", FileUtils::readFileIntoByteArray(measure.getAttribute("CINELOOP_1").toString()));
        sendHTTPSRequest("PATCH", "https://blueberry.clsa-elcv.ca/qa/pine/api/answer/" + QString::number(answer_id) + "?filename=STILL_IMAGE_1_" + side + ".dcm", "application/octet-stream", FileUtils::readFileIntoByteArray(measure.getAttribute("STILL_IMAGE_1").toString()));
        sendHTTPSRequest("PATCH", "https://blueberry.clsa-elcv.ca/qa/pine/api/answer/" + QString::number(answer_id) + "?filename=STILL_IMAGE_2_" + side + ".dcm", "application/octet-stream", FileUtils::readFileIntoByteArray(measure.getAttribute("STILL_IMAGE_2").toString()));
        sendHTTPSRequest("PATCH", "https://blueberry.clsa-elcv.ca/qa/pine/api/answer/" + QString::number(answer_id) + "?filename=STILL_IMAGE_3_" + side + ".dcm", "application/octet-stream", FileUtils::readFileIntoByteArray(measure.getAttribute("STILL_IMAGE_3").toString()));
        sendHTTPSRequest("PATCH", "https://blueberry.clsa-elcv.ca/qa/pine/api/answer/" + QString::number(answer_id) + "?filename=SR_1_" + side + ".dcm", "application/octet-stream", FileUtils::readFileIntoByteArray(measure.getAttribute("SR_1").toString()));

        measure.removeAttribute("CINELOOP_1");
        measure.removeAttribute("STILL_IMAGE_1");
        measure.removeAttribute("STILL_IMAGE_2");
        measure.removeAttribute("STILL_IMAGE_3");
        measure.removeAttribute("SR_1");
    }

    QJsonObject testJson = m_test->toJsonObject();
    QJsonObject sessionObj = m_session.getJsonObject();

    testJson.insert("session", sessionObj);
    responseJson.insert("value", responseJson);

    QJsonDocument jsonDoc(testJson);
    QByteArray serializedData = jsonDoc.toJson();

    sendHTTPSRequest("PATCH", "https://blueberry.clsa-elcv.ca/qa/pine/api/answer/" + QString::number(answer_id), "application/json", serializedData);

    emit success("sent");
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
