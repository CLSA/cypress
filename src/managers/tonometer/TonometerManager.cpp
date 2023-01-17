#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QSettings>
#include <QSqlDatabase>
#include <QStandardItemModel>

#include "CypressApplication.h"
#include "auxiliary/JsonSettings.h"
#include "data/AccessQueryHelper.h"

#include "TonometerManager.h"

TonometerManager::TonometerManager(QWidget* parent):
    ManagerBase(parent)
{
    m_test.setExpectedMeasurementCount(2);
}

TonometerManager::~TonometerManager()
{
  QSqlDatabase::removeDatabase("mdb_connection");
}

void TonometerManager::start()
{
    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started,
        this, [this]() {
            qDebug() << "process started: " << m_process.arguments().join(" ");
        });

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, &TonometerManager::readOutput);

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

    configureProcess();
    emit dataChanged();
}

bool TonometerManager::isDefined(const QString& fileName, const TonometerManager::FileType& type) const
{
    bool ok = false;
    QFileInfo info(fileName);
    if(type == TonometerManager::FileType::ORAApplication)
    {
      ok = info.isExecutable() && info.exists();
    }
    else
    {
      ok = info.isFile() && info.exists();
    }
    return ok;
}

void TonometerManager::select()
{
}

void TonometerManager::selectRunnable(const QString &exeName)
{

}

void TonometerManager::selectDatabase(const QString &dbName)
{

}

void TonometerManager::measure()
{

}

void TonometerManager::readOutput()
{

}

void TonometerManager::configureProcess()
{

}

void TonometerManager::clearData()
{
    m_test.reset();
}

void TonometerManager::finish()
{
    if (CypressApplication::mode == Mode::Sim)
    {
        QJsonObject results = JsonSettings::readJsonFromFile(
            "C:/work/clsa/cypress/src/tests/fixtures/tonometer/output.json"
        );
        if (results.empty()) return;

        bool ok = sendResultsToPine(results);
        if (!ok)
        {
            qDebug() << "Could not send results to Pine";
        }
    }
}
