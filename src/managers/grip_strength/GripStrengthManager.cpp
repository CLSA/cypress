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

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
    m_runnableName = settings.value("instruments/grip_strength/exe").toString();
    m_runnablePath = settings.value("instruments/grip_strength/dir").toString();
    m_gripTestDbPath = settings.value("instruments/grip_strength/tests").toString();
    m_gripTestDataDbPath = settings.value("instruments/grip_strength/results").toString();
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
        updateModel();
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

void GripStrengthManager::initializeConnections() {
    connect(&m_process, &QProcess::started, this, [this]() {
        if (m_verbose)
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
    QStandardItem* item;

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    m_model->setItem(0, 0, item);
    m_model->setHeaderData(0, Qt::Horizontal, "Exam ID", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    m_model->setItem(0, 1, item);
    m_model->setHeaderData(1, Qt::Horizontal, "Test ID", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    m_model->setItem(0, 2, item);
    m_model->setHeaderData(2, Qt::Horizontal, "Position", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    m_model->setItem(0, 3, item);
    m_model->setHeaderData(3, Qt::Horizontal, "Side", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    m_model->setItem(0, 4, item);
    m_model->setHeaderData(4, Qt::Horizontal, "Rep1", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    m_model->setItem(0, 5, item);
    m_model->setHeaderData(5, Qt::Horizontal, "Rep2", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    m_model->setItem(0, 6, item);
    m_model->setHeaderData(6, Qt::Horizontal, "Rep3", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    m_model->setItem(0, 7, item);
    m_model->setHeaderData(7, Qt::Horizontal, "Average", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    m_model->setItem(0, 8, item);
    m_model->setHeaderData(8, Qt::Horizontal, "Max", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    m_model->setItem(0, 9, item);
    m_model->setHeaderData(9, Qt::Horizontal, "CV", Qt::DisplayRole);
}

void GripStrengthManager::readOutput() {
   m_test.readGripTestOptions(m_gripTestDbPath);
   m_test.readGripTestResults(m_gripTestDataDbPath);

   toJsonObject();
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
    for (int row = 0; row < m_test.getMeasurementCount(); row++)
    {
        GripStrengthMeasurement measurement = m_test.getMeasurement(row);

        QStandardItem* exam_id = m_model->item(row, 0);
        if (Q_NULLPTR == exam_id)
        {
            exam_id = new QStandardItem();
            exam_id->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
            m_model->setItem(row, 0, exam_id);
        }
        if (exam_id) {
            GripStrengthMeasurement::Value value = measurement.getAttribute("exam_id");
            exam_id->setData(value.value(), Qt::DisplayRole);
        }

        QStandardItem* test_id = m_model->item(row, 1);
        if (Q_NULLPTR == test_id)
        {
            test_id = new QStandardItem();
            test_id->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
            m_model->setItem(row, 1, test_id);
        }
        if (test_id) {
            test_id->setData(measurement.getAttribute("test_id").value(), Qt::DisplayRole);
        }

        QStandardItem* position = m_model->item(row, 2);
        if (Q_NULLPTR == position)
        {
            position = new QStandardItem();
            position->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
            m_model->setItem(row, 2, position);
        }
        if (position) {
            qDebug() << "position: " << measurement.getAttribute("position").value();
            position->setData(measurement.getAttribute("position").value(), Qt::DisplayRole);
        }

        QStandardItem* trial_side = m_model->item(row, 3);
        if(Q_NULLPTR == trial_side)
        {
            trial_side = new QStandardItem();
            trial_side ->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
            m_model->setItem(row, 3, trial_side);
        }
        if (trial_side) {
            trial_side->setData(measurement.getAttribute("trial_side").value(), Qt::DisplayRole);
        }

        QStandardItem* rep1 = m_model->item(row, 4);
        if(Q_NULLPTR == rep1)
        {
            rep1 = new QStandardItem();
            rep1->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
            m_model->setItem(row, 4, rep1);
        }
        if (rep1) {
            rep1->setData(measurement.getAttribute("rep1").value(), Qt::DisplayRole);
        }
        rep1->setData(measurement.getAttribute("rep1").value(), Qt::DisplayRole);

        QStandardItem* rep2 = m_model->item(row, 5);
        if(Q_NULLPTR == rep2)
        {
            rep2 = new QStandardItem();
            rep2->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
            m_model->setItem(row, 5, rep2);
        }
        if (rep2) {
            rep2->setData(measurement.getAttribute("rep2").value(), Qt::DisplayRole);
        }
        rep2->setData(measurement.getAttribute("rep2").value(), Qt::DisplayRole);

        QStandardItem* rep3 = m_model->item(row, 6);
        if(Q_NULLPTR == rep3)
        {
            rep3 = new QStandardItem();
            rep3->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
            m_model->setItem(row, 6, rep1);
        }
        if (rep3) {
            rep3->setData(measurement.getAttribute("rep3").value(), Qt::DisplayRole);
        }
        rep3->setData(measurement.getAttribute("rep3").value(), Qt::DisplayRole);

        //QStandardItem* average= m_model->item(row, 14);
        //average->setData(measurement.getAttribute("Average").value(), Qt::DisplayRole);

        //QStandardItem* max = m_model->item(row, 14);
        //max->setData(measurement.getAttribute("Maximum").value(), Qt::DisplayRole);

        //QStandardItem* cv = m_model->item(row, 15);
        //cv->setData(measurement.getAttribute("CV").value(), Qt::DisplayRole);
    }
    //emit dataChanged();
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
    if(!input.contains("barcode"))
        m_inputData["barcode"] = Constants::DefaultBarcode;

    if(!input.contains("language"))
        m_inputData["language"] = "english";

    bool ok = true;
    QMap<QString,QMetaType::Type> typeMap {
        {"barcode",QMetaType::Type::QString},
        {"language",QMetaType::Type::QString}
    };

    qDebug() << m_inputData;
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
    //QDir working(m_runnablePath);
    //if (isDefined(m_runnableName, GripStrengthManager::FileType::Tracker5Exe) &&
    //    isDefined(m_gripTestDbPath, GripStrengthManager::FileType::GripTestDbPath) &&
    //    isDefined(m_gripTestDataDbPath, GripStrengthManager::FileType::GripTestDataDbPath) &&
    //    working.exists())
    //{
    m_process.setProgram(m_runnablePath + "/" + m_runnableName);
    m_process.setWorkingDirectory(m_runnablePath);

    emit processInitialized();

    qDebug() << m_runnablePath + "/" + m_runnableName;
    //}
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

    m_gripTestDbPath = path;
}
