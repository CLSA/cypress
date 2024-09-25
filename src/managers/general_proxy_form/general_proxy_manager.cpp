#include "general_proxy_manager.h"
#include "auxiliary/pdf_form_filler.h"
#include "auxiliary/file_utils.h"
#include "auxiliary/network_utils.h"
#include "cypress_settings.h"

#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>

DeviceConfig GeneralProxyManager::config {{
    { "pdftkPath",    {"general_proxy/pdftkPath",    Exe }},
    { "runnableName", {"general_proxy/runnableName", Exe }},
    { "runnablePath", {"general_proxy/runnablePath", Dir }},
}};

GeneralProxyManager::GeneralProxyManager(QSharedPointer<GenProxySession> session): ManagerBase(session) {
    m_runnableName = CypressSettings::readSetting("general_proxy/runnableName").toString();
    m_runnablePath = CypressSettings::readSetting("general_proxy/runnablePath").toString();
    m_pdftkPath = CypressSettings::readSetting("general_proxy/pdftkPath").toString();
    m_outputFilePath = QDir::currentPath() + "/" + m_session->getBarcode() + ".pdf";
}

bool GeneralProxyManager::start() {
    PDFFormFiller filler;
    QJsonObject inputData;

    inputData["enrollmentId"] = m_session->getUID();

    QString language = m_session->getInputData().value("language").toString();
    QDir currentDir = QDir::currentPath();

    QString fdfPath = "general_proxy/gen_proxy_v1_1.fdf";
    if (language == "fr") {
        fdfPath = "general_proxy/gen_proxy_v1_1_fr.fdf";
    }

    QString outputPath = filler.fillPDF(
        currentDir.filePath(QString("general_proxy/gen_proxy_v1_1_%1.pdf").arg(language)),
        currentDir.filePath(fdfPath),
        inputData,
        m_outputFilePath
    );

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            &GeneralProxyManager::readOutput);

    connect(&m_process, &QProcess::errorOccurred, this, [=](QProcess::ProcessError error) {
        QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"),
                                                                    Qt::SkipEmptyParts);
        qDebug() << "ERROR: process error occured: " << s.join(" ").toLower();
    });

    connect(&m_process, &QProcess::stateChanged, this, [=](QProcess::ProcessState state) {
        QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"),
                                                                    Qt::SkipEmptyParts);
        qDebug() << "process state: " << s.join(" ").toLower();
    });

    QStringList arguments { QDir::toNativeSeparators(outputPath) };
    m_process.setProgram(m_runnableName);
    m_process.setArguments(arguments);
    m_process.setWorkingDirectory(m_runnablePath);
    m_process.start();

    return true;
}

void GeneralProxyManager::readOutput() {
    const QFileInfo outputFile(m_outputFilePath);
    if (!outputFile.exists()) {
        emit error("Output file does not exist");
        return;
    }
    else if (!outputFile.isFile()) {
        emit error("Output file is invalid");
        return;
    }
    else if (!outputFile.isReadable()) {
        emit error("Output file is not readable");
        return;
    }

    finish();
}

void GeneralProxyManager::finish() {

    const int answer_id = m_session->getAnswerId();
    const QString pineOrigin = m_session->getOrigin();
    const QString answerUrl = pineOrigin + "/answer/" + QString::number(answer_id);

    PDFFormFiller filler;
    const QString dataFields = filler.dumpDataFields(m_outputFilePath);
    const QJsonObject dataFieldsJson = filler.parseDataFields(dataFields);

    qDebug() << "dataFieldsJson: " << dataFieldsJson;

    QJsonObject testJson {};
    QJsonObject filesJson {
        { "general_proxy_pdf", FileUtils::getHumanReadableFileSize(m_outputFilePath) }
    };

    testJson.insert("files", filesJson);
    testJson.insert("metadata", {});
    testJson.insert("results", QJsonArray { dataFieldsJson } );
    testJson.insert("session", m_session->getJsonObject());

    const QJsonObject responseJson { { "value", testJson } };
    const QJsonDocument jsonDoc(responseJson);
    const QByteArray serializedData = jsonDoc.toJson();

    bool ok = NetworkUtils::sendHTTPSRequest(
        Poco::Net::HTTPRequest::HTTP_PATCH,
        answerUrl.toStdString(),
        "application/json",
        serializedData
    );
    if (!ok)
        emit error("Something went wrong");

    ok = NetworkUtils::sendHTTPSRequest(
        Poco::Net::HTTPRequest::HTTP_PATCH,
        (answerUrl + "?filename=general_proxy.pdf").toStdString(),
        "application/octet-stream",
        FileUtils::readFile(m_outputFilePath)
    );

    cleanUp();

    if (ok)
        emit success("Save successful. You may now close this window");
    else
        emit error("Something went wrong.");
}

bool GeneralProxyManager::cleanUp() {
    if (QFileInfo::exists(m_outputFilePath) && !QFile::remove(m_outputFilePath)) {
        qDebug() << "could not remove output file";
        return false;
    }
    return true;
}
