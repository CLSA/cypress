#include "FraxManager.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonObject>
#include <QProcess>
#include <QSettings>

FraxManager::FraxManager(QObject* parent):
	ManagerBase(parent)
{
}

void FraxManager::buildModel(QStandardItemModel* model) const
{
    // add measurement
    //
    int n_total = m_test.getNumberOfMeasurements();
    model->setRowCount(1);
    QString measurementStr = "NA";
    FraxMeasurement measurement = m_test.getMeasurement(0);
    if (measurement.isValid())
        measurementStr = measurement.toString();
    QStandardItem* item = model->item(0, 0);
    if (nullptr == item)
    {
        item = new QStandardItem();
        model->setItem(0, 0, item);
    }
    item->setData(measurementStr, Qt::DisplayRole);
}

void FraxManager::loadSettings(const QSettings& settings)
{
    // the full spec path name including exe name
    // eg., ../frax_module/blackbox.exe
    //
    QString exeName = settings.value("client/exe").toString();
    setExecutableName(exeName);
}

void FraxManager::saveSettings(QSettings* settings) const
{
    if (!m_executableExePath.isEmpty())
    {
        settings->setValue("client/exe", m_executableExePath);
        if (m_verbose)
            qDebug() << "wrote exe fullspec path to settings file";
    }
}

QJsonObject FraxManager::toJsonObject() const
{
    QJsonObject json = m_test.toJsonObject();
    QFile ofile(m_outputFilePath);
    ofile.open(QIODevice::ReadOnly);
    QByteArray buffer = ofile.readAll();
    json.insert("test_txt_file", QString(buffer.toBase64()));
    return json;
}

bool FraxManager::isDefined(const QString& exeName) const
{
    bool ok = false;
    if (!exeName.isEmpty())
    {
        QFileInfo info(exeName);
        if (info.exists() && info.isExecutable())
        {
            ok = true;
        }
    }
    return ok;
}

void FraxManager::setExecutableName(const QString&exeName)
{
    if (isDefined(exeName))
    {
        QFileInfo info(exeName);
        m_executableExePath = exeName;
        m_executableFolderPath = info.dir().absolutePath();
        m_inputFilePath = QDir(m_executableFolderPath).filePath("input.txt");
        m_oldInputFilePath = QDir(m_executableFolderPath).filePath("oldInput.txt");
        m_outputFilePath = QDir(m_executableFolderPath).filePath("output.txt");

        //setInputs(inputs);
        createInputsTxt();
        configureProcess();
    }
}

bool FraxManager::createInputsTxt()
{
    QString filePath = getInputFullPath();
    QFile file(filePath);
    if (file.exists()) {
        file.rename(m_oldInputFilePath);
        file.setFileName(getInputFullPath());
    }

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);

        stream << m_inputData["val1"].toString() + ","
            << m_inputData["val2"].toString() << ","
            << m_inputData["val3"].toString() << ","
            << m_inputData["val4"].toString() << ","
            << m_inputData["val5"].toString() << ","
            << m_inputData["val6"].toString() << ","
            << m_inputData["val7"].toString() << ","
            << m_inputData["val8"].toString() << ","
            << m_inputData["val9"].toString() << ","
            << m_inputData["val10"].toString() << ","
            << m_inputData["val11"].toString() << ","
            << m_inputData["val12"].toString() << ","
            << m_inputData["dxaHipTScore"].toString();

        file.close();
        qDebug() << "Wrote input.txt to " + filePath;
        return true;
    }
    return false;
}

void FraxManager::readOutputs()
{
    QFile file(getOutputFullPath());

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream instream(&file);
        QString line = instream.readLine();
        file.close();

        QStringList lineSplit = line.split(",");
        if (lineSplit.length() >= 17) {
            m_inputData["val1"] = lineSplit[0];
            m_inputData["val2"] = QString(lineSplit[1]).toDouble();
            m_inputData["val3"] = QString(lineSplit[2]).toDouble();
            m_inputData["val4"] = QString(lineSplit[3]).toDouble();
            m_inputData["val5"] = QString(lineSplit[4]).toDouble();
            m_inputData["val6"] = QString(lineSplit[5]).toDouble();
            m_inputData["val7"] = QString(lineSplit[6]).toDouble();
            m_inputData["val8"] = QString(lineSplit[7]).toDouble();
            m_inputData["val9"] = QString(lineSplit[8]).toDouble();
            m_inputData["val10"] = QString(lineSplit[9]).toDouble();
            m_inputData["val11"] = QString(lineSplit[10]).toDouble();
            m_inputData["val12"] = QString(lineSplit[11]).toDouble();
            m_inputData["dxaHipTScore"] = QString(lineSplit[12]).toDouble();
            m_outputData["fracRisk1"] = QString(lineSplit[13]).toDouble();
            m_outputData["fracRisk2"] = QString(lineSplit[14]).toDouble();
            m_outputData["fracRisk3"] = QString(lineSplit[15]).toDouble();
            m_outputData["fracRisk4"] = QString(lineSplit[16]).toDouble();
        }
    }
}

void FraxManager::clean()
{
    if (!m_outputFilePath.isEmpty())
    {
        QFile ofile(m_outputFilePath);
        ofile.remove();
    }

    if (!m_inputFilePath.isEmpty()) {
        QFile curInFile(m_inputFilePath);
        QFile oldInFile(m_oldInputFilePath);
        if (oldInFile.exists()) {
            if (curInFile.exists()) {
                curInFile.remove();
            }
            oldInFile.rename(m_inputFilePath);
        }
    }
}

void  FraxManager::measure()
{
    // launch the process
    qDebug() << "starting process from measure";
    m_process.start();
    //m_process.waitForFinished();
}

void FraxManager::setInputs(const QMap<QString, QVariant> &inputs)
{
    // TODO: check minimum inputs required
    //
    m_inputData = inputs;
    createInputsTxt();
    configureProcess();
}

void FraxManager::readOutput()
{
    if (QProcess::NormalExit != m_process.exitStatus())
    {
        qDebug() << "ERROR: process failed to finish correctly: cannot read output";
        return;
    }
    else
        qDebug() << "process finished successfully";

    QFileInfo outFile(m_outputFilePath);
    if (outFile.exists())
    {
        qDebug() << "found output txt file " << m_outputFilePath;
        m_test.fromFile(m_outputFilePath);
        if (m_test.isValid())
        {
            emit canWrite();
        }
        else
            qDebug() << "ERROR: input from file produced invalid test results";

        emit dataChanged();

    }
    else
        qDebug() << "ERROR: no output csv file found";
}

void FraxManager::configureProcess()
{
    // the exe is present
    QFileInfo info(m_executableExePath);
    QDir working(m_executableFolderPath);
    if (info.exists() && info.isExecutable() &&
        working.exists())
    {
        qDebug() << "OK: configuring command";

        m_process.setProcessChannelMode(QProcess::ForwardedChannels);
        m_process.setProgram(m_executableExePath);
        m_process.setWorkingDirectory(m_executableFolderPath);

        qDebug() << "process working dir: " << m_executableFolderPath;

        connect(&m_process, &QProcess::started,
            this, [this]() {
                qDebug() << "process started: " << m_process.arguments().join(" ");
            });

        connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &FraxManager::readOutput);

        connect(&m_process, &QProcess::errorOccurred,
            this, [](QProcess::ProcessError error)
            {
                QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"), QString::SkipEmptyParts);
                qDebug() << "ERROR: process error occured: " << s.join(" ").toLower();
            });

        connect(&m_process, &QProcess::stateChanged,
            this, [](QProcess::ProcessState state) {
                QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"), QString::SkipEmptyParts);
                qDebug() << "process state: " << s.join(" ").toLower();

            });

        emit canMeasure();
    }
    else
        qDebug() << "failed to configure process";
}

void FraxManager::clearData()
{
    m_test.reset();
    m_outputFilePath.clear();
    emit dataChanged();
}