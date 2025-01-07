#include "frax_manager.h"
#include "data/frax/tests/frax_test.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <QStandardItemModel>
#include <QMessageBox>

QSharedPointer<DeviceConfig> FraxManager::config = QSharedPointer<DeviceConfig>(new DeviceConfig {{
    { "runnableName",       { "frax/runnableName",       Exe }},
    { "runnablePath",       { "frax/runnablePath",       Dir }},
    { "outputFilePath",     { "frax/outputFilePath",     NonEmptyString }},
    { "inputFilePath",      { "frax/inputFilePath",      NonEmptyString }},
    { "countryCode",        { "frax/countryCode",        NonEmptyString }},
    { "typeCode",           { "frax/typeCode",           NonEmptyString }},
}});

FraxManager::FraxManager(const QSharedPointer<FraxSession> session)
    : ManagerBase(session)
{
    m_test.reset(new FraxTest);
}

// Set up device
bool FraxManager::setUp()
{
    auto fraxTest = qSharedPointerCast<FraxTest>(m_test);

    if (!fraxTest->writeInputFile(config->getSetting("inputFilePath"), m_session->getInputData())) {
        qDebug() << "FraxManager::setUp - write input file failed";
        return false;
    }

    return true;
}

bool FraxManager::start()
{
    if (!setUp()) {
        return false;
    }

    measure();
    return true;
}

void FraxManager::measure()
{
    QProcess process;

    QDir fraxDir(config->getSetting("runnablePath"));
    QFileInfo fraxExe(config->getSetting("runnableName"));

    process.setWorkingDirectory(fraxDir.absolutePath());
    process.setProgram(fraxExe.absoluteFilePath());
    process.start();

    if (!process.waitForStarted()) {
        qDebug() << "FraxTest::run - no start";
        emit error("FRAX failed to start");
        return;
    }

    if (!process.waitForFinished()) {
        qDebug() << "FraxTest::run - no finish";
        emit error("FRAX failed to finish");
        return;
    }

    readOutput();
}

void FraxManager::readOutput()
{
    auto fraxTest = qSharedPointerCast<FraxTest>(m_test);

    if (!fraxTest->readOutputFile(config->getSetting("outputFilePath"))) {
        emit error("Cannot read FRAX results");
        return;
    }

    finish();
}

bool FraxManager::clearData()
{
    m_test->reset();
    return true;
}

// Clean up the device for next time
bool FraxManager::cleanUp()
{
    QFileInfo inputFile(config->getSetting("inputFilePath"));
    if (!QFile::remove(inputFile.absoluteFilePath())) {
        return false;
    }

    QFileInfo outputFile(config->getSetting("outputFilePath"));
    if (!QFile::remove(outputFile.absoluteFilePath())) {
        return false;
    }

    return true;
}
