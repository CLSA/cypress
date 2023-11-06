#include "vividi_manager.h"
#include "cypress_application.h"

#include "auxiliary/file_utils.h"

#include <QJsonDocument>

VividiManager::VividiManager(QSharedPointer<UltrasoundSession> session)
    : ManagerBase(session)
{
    m_test.reset(new CimtVividiTest);
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
    emit started(m_test.get());
    emit canMeasure();
}

void VividiManager::measure()
{
    m_test->reset();

    if (Cypress::getInstance().isSimulation())
        m_test->simulate();

    emit measured(m_test.get());

    if (m_test->isValid())
    {
      emit canFinish();
    }
}

void VividiManager::cancel()
{
}

void VividiManager::finish()
{
    QJsonObject responseJson{};

    QString host = CypressSettings::getInstance().getPineHost();
    QString endpoint = CypressSettings::getInstance().getPineEndpoint();
    QString pine_path = CypressSettings::getInstance().getAnswerUrl(m_session->getAnswerId());

    for (int i = 0; i < m_test->getMeasurementCount(); i++)
    {
        Measurement& measure = m_test->get(i);
        const QString &side = measure.getAttribute("side").toString();
        QByteArray data = FileUtils::readFileIntoByteArray(measure.getAttribute("path").toString());

        sendHTTPSRequest("PATCH",
                         pine_path + "?filename=" + measure.getAttribute("name").toString(),
                         "application/octet-stream",
                         data);

        measure.removeAttribute("PATH");
    }

    QJsonObject testJson = m_test->toJsonObject();
    QJsonObject sessionObj = m_session->getJsonObject();

    testJson.insert("session", sessionObj);
    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    sendHTTPSRequest("PATCH", pine_path, "application/json", serializedData);

    emit success("Sent measurements to Pine, you can now close this window");
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
