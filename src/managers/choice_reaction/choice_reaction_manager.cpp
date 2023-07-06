#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonObject>
#include <QProcess>
#include <QSettings>
#include <QStandardItemModel>

#include "cypress_application.h"
#include "auxiliary/json_settings.h"

#include "choice_reaction_manager.h"

QString ChoiceReactionManager::CCB_PREFIX = "CLSA_ELCV";
QString ChoiceReactionManager::CCB_CLINIC = "CYPRESS";

ChoiceReactionManager::ChoiceReactionManager(const CypressSession& session)
    : ManagerBase(session)
{
    qDebug() << "ChoiceReactionManager inputData: " << m_inputData;
}

bool ChoiceReactionManager::isAvailable()
{
    return false;
}

bool ChoiceReactionManager::isInstalled()
{
    return false;
}

void ChoiceReactionManager::start()
{
    // connect signals and slots to QProcess one time only
    //
    //connect(&m_process, &QProcess::started,
    //    this, [this]() {
    //        qDebug() << "process started: " << m_process.arguments().join(" ");
    //    });

    //connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
    //    this, &ChoiceReactionManager::readOutput);

    //connect(&m_process, &QProcess::errorOccurred,
    //    this, [](QProcess::ProcessError error)
    //    {
    //        QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
    //        qDebug() << "ERROR: process error occured: " << s.join(" ").toLower();
    //    });

    //connect(&m_process, &QProcess::stateChanged,
    //    this, [](QProcess::ProcessState state) {
    //        QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
    //        qDebug() << "process state: " << s.join(" ").toLower();
    //    });

    //m_process.setProcessChannelMode(QProcess::ForwardedChannels);

    //configureProcess();
    emit dataChanged();
}

void ChoiceReactionManager::configureProcess()
{

}

void ChoiceReactionManager::readOutput()
{

}

void ChoiceReactionManager::measure()
{
    if (Cypress::getInstance().isSimulation()) {
        sendResultsToPine("C:/dev/clsa/cypress/src/tests/fixtures/choice_reaction/output.json");
        return;
    }


    //if (results.empty()) return;

    //results["cypress_session"] = m_uuid;
    //results["answer_id"] = m_answerId;
    //results["barcode"] = m_barcode;
    //results["interviewer"] = m_interviewer;

    //bool ok = sendResultsToPine(results);
    //if (!ok)
    //{
    //    qDebug() << "Could not send results to Pine";
    //}
}

void ChoiceReactionManager::finish()
{
    //if (CypressApplication::getInstance().isSimulation())
    //{
    //    QJsonObject results = JsonSettings::readJsonFromFile(
    //        "C:/dev/clsa/cypress/src/tests/fixtures/choice_reaction/output.json"
    //    );
    //    if (results.empty()) return;

    //    bool ok = sendResultsToPine(results);
    //    if (!ok)
    //    {
    //        qDebug() << "Could not send results to Pine";
    //    }
    //}
}

bool ChoiceReactionManager::clearData()
{
    return false;
}


// Set up device
bool ChoiceReactionManager::setUp()
{
    return true;
}

// Clean up the device for next time
bool ChoiceReactionManager::cleanUp()
{
    return true;
}

void ChoiceReactionManager::setInputData(const QVariantMap& inputData)
{
    Q_UNUSED(inputData)
}
