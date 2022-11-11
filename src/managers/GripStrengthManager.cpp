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
    setGroup("gripstrength");
    m_col = 1;
    m_row = 1;

    // all managers must check for barcode and language input values
    //
    m_inputKeyList << "barcode";
}


void GripStrengthManager::start()
{
    initializeConnections();
    initializeModel();
    configureProcess();
}

void GripStrengthManager::measure() {
    try {
        readOutput();
    }
    catch (QException &e) {
        qDebug() << e.what();
    }
}

void GripStrengthManager::finish()
{
    try {
        m_test.reset();
    }
    catch (QException &e) {
        qDebug() << e.what();
    }
}

void GripStrengthManager::initializeConnections() {
    connect(&m_process, &QProcess::started, this, [this]() {
            qDebug() << "process started: " << m_process.arguments().join(" ");
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

    connect(this, &GripStrengthManager::processInitialized, this, &GripStrengthManager::processStart);
}

void GripStrengthManager::initializeModel()
{
    // example of 1 row 1 column display of 1 measurement test
    //
    for (int row = 0; row < m_row; row++)
    {
        QStandardItem* item = new QStandardItem();
        m_model->setItem(row, 0, item);
    }
    m_model->setHeaderData(0, Qt::Horizontal, "Weight Tests", Qt::DisplayRole);
}

void GripStrengthManager::readOutput() {
   m_test.fromParadox(m_gripTestDbPath, m_gripTestDataDbPath);
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
    if(Constants::RunMode::modeSimulate == m_mode)
    {
        // simulate mode code
    }
    json.insert("test_input",QJsonObject::fromVariantMap(m_inputData));
    return json;
}

void GripStrengthManager::updateModel()
{
    //TODO: Update, this is from BuildModel class
    // example of a 1 row 1 column model displaying one measurement
    //
    for (int row = 0; row < m_test.getMeasurementCount(); row++)
    {
        QStandardItem* item = m_model->item(row, 0);
        if (Q_NULLPTR == item)
        {
            item = new QStandardItem();
            m_model->setItem(row, 0, item);
        }
        item->setData(m_test.getMeasurement(row).toString(), Qt::DisplayRole);
    }
    emit dataChanged();
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

void GripStrengthManager::processStart()
{
    try {
        m_process.start();
    }
    catch (const std::exception& e) {
        qDebug() << e.what();
    }
}

void GripStrengthManager::setInputData(const QVariantMap& input)
{
    m_inputData = input;
    if(Constants::RunMode::modeSimulate == m_mode)
    {
      if(!input.contains("barcode"))
        m_inputData["barcode"] = Constants::DefaultBarcode;
      if(!input.contains("language"))
        m_inputData["language"] = "english";
    }
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
    m_test.reset();
    updateModel();
}

void GripStrengthManager::configureProcess()
{
    QDir working(m_runnablePath);
    if (isDefined(m_runnableName, GripStrengthManager::FileType::Tracker5Exe) &&
        isDefined(m_gripTestDbPath, GripStrengthManager::FileType::GripTestDbPath) &&
        isDefined(m_gripTestDataDbPath, GripStrengthManager::FileType::GripTestDataDbPath) &&
        working.exists())
    {
        m_process.setProgram(m_runnableName);
        m_process.setWorkingDirectory(m_runnablePath);

        emit processInitialized();
    }
}



void GripStrengthManager::setRunnablePath(const QString& path)
{
    //QString m_runnablepath = "C:/Program Files (x86)/Tracker 5"; // path to WTS.exe directory
    const QFileInfo pathDir(path);
    //if (!pathDir.exists())
    //    throw "Runnable path does not exist";
    //if (!pathDir.isDir())
    //    throw "Runnable path is not a directory";

    m_runnablePath = path;
}

void GripStrengthManager::setRunnableName(const QString& path)
{
    // "C:/Program Files (x86)/Tracker 5/WTS.exe";// full pathspec to WTS.exe
    const QFileInfo pathDir(path);
    //if (!pathDir.exists())
    //    throw "Runnable does not exist";
    //if (!pathDir.isExecutable())
    //    throw "Runnable is not an executable";

    m_runnableName = path;
}

void GripStrengthManager::setDatabasePath(const QString& path)
{
    // "C:/Users/clsa/Desktop/ZGripTestData.DB"
    const QFileInfo pathDir(path);
    if (!pathDir.exists())
        throw "Database path does not exist";
    if (!pathDir.isDir())
        throw "Database path is not a file";

    m_gripTestDataDbPath = path;
}



void GripStrengthManager::setTestPath(const QString& path)
{
    // "C:/Users/clsa/Desktop/ZGripTestData.DB"
    const QFileInfo pathDir(path);
    if (!pathDir.exists())
        throw "Database path does not exist";
    if (!pathDir.isDir())
        throw "Database path is not a file";

    m_gripTestDbPath = "C:/Users/clsa/Desktop/ZGripTest.DB";
}
