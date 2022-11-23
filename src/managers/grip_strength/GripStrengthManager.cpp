#include "GripStrengthManager.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonObject>
#include <QProcess>
#include <QSettings>
#include <QStandardItemModel>
#include <QException>


GripStrengthManager::GripStrengthManager(QObject* parent) : ManagerBase(parent)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");

    //setGroup("gripstrength");
    //m_col = 1;
    //m_row = 1;

    // all managers must check for barcode and language input values
    //
    m_inputKeyList << "barcode";

    m_workingDir   = 	 settings.value("instruments/grip_strength/workingDir").toString();
    m_runnableName = 	 settings.value("instruments/grip_strength/runnableName").toString();
    m_databaseDir  = 	 settings.value("instruments/grip_strength/databaseDir").toString();
    m_databaseName = 	 settings.value("instruments/grip_strength/databaseName").toString();
    m_databaseHost = 	 settings.value("instruments/grip_strength/databaseHost").toString();
    m_databasePort = 	 settings.value("instruments/grip_strength/databasePort").toString();
    m_databaseUser =     settings.value("instruments/grip_strength/databaseUser").toString();
    m_databasePassword = settings.value("instruments/grip_strength/databasePassword").toString();
}


void GripStrengthManager::start()
{
    initializeConnections();
    //initializeModel();
    //configureProcess();
    bool ok = readOutput();

    if (!ok)
    {
        qDebug("Could not read output");
    }
}

void GripStrengthManager::measure() {
    try {
        readOutput();
        qDebug() << m_test.toJsonObject();
        //updateModel();
    }
    catch (QException &e) {
        qDebug() << e.what();
    }
}

void GripStrengthManager::finish()
{
    qDebug() << "finish";
    try {
        //m_test.reset();
        //updateModel();
    }
    catch (QException &e) {
        qDebug() << e.what();
    }
}

bool GripStrengthManager::initializeConnections() {
    try {
        connect(&m_process, &QProcess::started, this, [this]() {
            if (m_verbose)
                qDebug() << "Tracker 5 started: " << m_process.arguments().join(" ");
        });

        connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &GripStrengthManager::canMeasure);

        connect(this, &GripStrengthManager::canMeasure, this, &GripStrengthManager::measureStart);

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

        //connect(this, &GripStrengthManager::processInitialized, this, &GripStrengthManager::processStart);
    }
    catch (QException& e)
    {
        qDebug() << e.what();
        return false;
    }

    return true;
}

void GripStrengthManager::initializeModel()
{
}

bool GripStrengthManager::readOutput()
{
    bool ok = m_test.readGripTestResults();

    if (m_verbose)
    {
        qDebug() << toJsonObject();
    }

    return ok;
}


void GripStrengthManager::loadSettings(const QSettings& settings)
{
    Q_UNUSED(settings)
}

void GripStrengthManager::saveSettings(QSettings* settings) const
{
    Q_UNUSED(settings)
}

QJsonObject GripStrengthManager::toJsonObject() const
{
    QJsonObject json = m_test.toJsonObject();
    json.insert("test_input", QJsonObject::fromVariantMap(m_inputData));

    if (m_verbose)
        qDebug() << json;

    return json;
}

void GripStrengthManager::updateModel()
{
}

bool GripStrengthManager::isDefined(const QString& value, const GripStrengthManager::FileType& fileType) const
{
    if (value.isEmpty())
        return false;

    QFileInfo info(value);
    bool ok = info.exists();
    if (fileType == GripStrengthManager::FileType::Tracker5Exe)
    {
        if ("exe" != info.completeSuffix())
        {
            ok = false;
        }
    }
    else if (fileType == GripStrengthManager::FileType::GripTestDbPath
        || fileType == GripStrengthManager::FileType::GripTestDataDbPath)
    {
        if ("DB" != info.completeSuffix())
        {
            ok = false;
        }
    }
    return ok;
}

bool GripStrengthManager::processStart()
{
    try {
        m_process.start();
        return m_process.waitForStarted();
    }
    catch (const std::exception& e) {
        qDebug() << e.what();
    }

    return false;
}

void GripStrengthManager::setInputData(const QVariantMap& input)
{
    if(!input.contains("barcode"))
        m_inputData["barcode"] = Constants::DefaultBarcode;

    if(!input.contains("language"))
        m_inputData["language"] = "english";

    bool ok = true;
    QMap<QString,QMetaType::Type> typeMap {
        {"barcode",QMetaType::Type::QString},
        {"language",QMetaType::Type::QString}
    };

    foreach(auto key, m_inputKeyList)
    {
      if(!m_inputData.contains(key))
      {
        ok = false;
        if(m_verbose)
          qDebug() << "ERROR: missing expected input " << key;
        break;
      }
      const QVariant value = m_inputData[key];
      bool valueOk = true;
      QMetaType::Type type;
      if(typeMap.contains(key))
      {
        type = typeMap[key];
        valueOk = value.canConvert(type);
      }
      if(!valueOk)
      {
        ok = false;
        if(m_verbose)
          qDebug() << "ERROR: invalid input" << key << value.toString() << QMetaType::typeName(type);
        break;
      }
    }
    if(!ok)
    {
      if(m_verbose)
        qDebug() << "ERROR: invalid input data";

      emit message(tr("ERROR: the input data is incorrect"));
      m_inputData = QVariantMap();
    }
}

void GripStrengthManager::clearData()
{
    //m_test.reset();
    //updateModel();
}

bool GripStrengthManager::configureProcess() {
    QDir working(m_runnableName);
    if (!working.exists())
    {
        if (m_verbose)
            qDebug() << "working does not exist.";

        return false;
    }

    try {
        if (m_verbose) {
            qDebug() << m_runnableName;
        }

        m_process.setProgram(m_runnableName);
        m_process.setWorkingDirectory(working.absolutePath());

        emit processInitialized();

        return true;
    }
    catch (QException& e)
    {
        qDebug() << e.what();
    }

    return false;
}

//void GripStrengthManager::setRunnablePath(const QString& path)
//{
//    //QString m_runnablepath = "C:/Program Files (x86)/Tracker 5"; // path to WTS.exe directory
//    const QFileInfo pathDir(path);
//    //if (!pathDir.exists())
//    //    throw "Runnable path does not exist";
//    //if (!pathDir.isDir())
//    //    throw "Runnable path is not a directory";
//
//    m_runnablePath = path;
//}

//void GripStrengthManager::setRunnableName(const QString& path)
//{
//    // "C:/Program Files (x86)/Tracker 5/WTS.exe";// full pathspec to WTS.exe
//    const QFileInfo pathDir(path);
//    //if (!pathDir.exists())
//    //    throw "Runnable does not exist";
//    //if (!pathDir.isExecutable())
//    //    throw "Runnable is not an executable";
//
//    m_runnableName = path;
//}

//void GripStrengthManager::setDatabasePath(const QString& path)
//{
//    // "C:/Users/clsa/Desktop/ZGripTestData.DB"
//    const QFileInfo pathDir(path);
//    if (!pathDir.exists())
//        throw "Database path does not exist";
//    if (!pathDir.isDir())
//        throw "Database path is not a file";
//
//    m_gripTestDataDbPath = path;
//}
//
//
//
//void GripStrengthManager::setTestPath(const QString& path)
//{
//    // "C:/Users/clsa/Desktop/ZGripTestData.DB"
//    const QFileInfo pathDir(path);
//    if (!pathDir.exists())
//        throw "Database path does not exist";
//    if (!pathDir.isDir())
//        throw "Database path is not a file";
//
//    m_gripTestDbPath = path;
//}
