
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QJsonObject>
#include <QSettings>
#include <QStandardItemModel>

#include "CypressApplication.h"
#include "auxiliary/JsonSettings.h"

#include "ECGManager.h"

ECGManager::ECGManager(QWidget* parent):
    ManagerBase(parent)
{
    m_test.setExpectedMeasurementCount(1);
}

void ECGManager::start()
{
    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started,
        this, [this]() {
            qDebug() << "process started: " << m_process.arguments().join(" ");
        });

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, &ECGManager::readOutput);

    connect(&m_process, &QProcess::errorOccurred,
        this, [](QProcess::ProcessError error)
        {
            QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
            qDebug() << "ERROR: process error occured: " << s.join(" ").toLower();
        });

    connect(&m_process, &QProcess::stateChanged,
        this, [](QProcess::ProcessState state) {
            QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
            qDebug() << "process state: " << s.join(" ").toLower();
        });

    m_process.setProcessChannelMode(QProcess::ForwardedChannels);

    configureProcess();
    emit dataChanged();
}

bool ECGManager::isDefined(const QString &fileName, const FileType &type) const
{
    return false;
}

void ECGManager::selectRunnable(const QString &runnableName)
{

}

void ECGManager::selectWorking(const QString& workingName)
{

}

void ECGManager::select()
{

}

void ECGManager::measure()
{
    clearData();
    // launch the process
    qDebug() << "starting process from measure";
    m_process.start();
}

void ECGManager::readOutput()
{

}

void ECGManager::configureProcess()
{

}

void ECGManager::clearData()
{
    m_test.reset();
}

void ECGManager::finish()
{
    if (CypressApplication::mode == Mode::Sim)
    {
        QJsonObject results = JsonSettings::readJsonFromFile(
            "C:/work/clsa/cypress/src/tests/fixtures/ecg/output.json"
        );
        if (results.empty()) return;

        bool ok = sendResultsToPine(results);
        if (!ok)
        {
            qDebug() << "Could not send results to Pine";
        }
    }
}

bool ECGManager::deleteDeviceData()
{
   return false;
}
