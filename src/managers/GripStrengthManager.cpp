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
        m_test.reset();
        updateModel();
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
    QStandardItem* item;

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
    m_model->setItem(0, 0, item);
    m_model->setHeaderData(0, Qt::Horizontal, "Exam ID", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
    m_model->setItem(0, 1, item);
    m_model->setHeaderData(1, Qt::Horizontal, "Test ID", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
    m_model->setItem(0, 2, item);
    m_model->setHeaderData(2, Qt::Horizontal, "Position", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
    m_model->setItem(0, 3, item);
    m_model->setHeaderData(3, Qt::Horizontal, "Side", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
    m_model->setItem(0, 4, item);
    m_model->setHeaderData(4, Qt::Horizontal, "Rep1", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
    m_model->setItem(0, 5, item);
    m_model->setHeaderData(5, Qt::Horizontal, "Rep2", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
    m_model->setItem(0, 6, item);
    m_model->setHeaderData(6, Qt::Horizontal, "Rep3", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
    m_model->setItem(0, 7, item);
    m_model->setHeaderData(7, Qt::Horizontal, "Rep4", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
    m_model->setItem(0, 8, item);
    m_model->setHeaderData(8, Qt::Horizontal, "Rep5", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
    m_model->setItem(0, 9, item);
    m_model->setHeaderData(9, Qt::Horizontal, "Rep6", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
    m_model->setItem(0, 10, item);
    m_model->setHeaderData(10, Qt::Horizontal, "Rep7", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
    m_model->setItem(0, 11, item);
    m_model->setHeaderData(11, Qt::Horizontal, "Rep8", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
    m_model->setItem(0, 12, item);
    m_model->setHeaderData(12, Qt::Horizontal, "Rep9", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
    m_model->setItem(0, 13, item);
    m_model->setHeaderData(13, Qt::Horizontal, "Rep10", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
    m_model->setItem(0, 14, item);
    m_model->setHeaderData(14, Qt::Horizontal, "Average", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
    m_model->setItem(0, 15, item);
    m_model->setHeaderData(15, Qt::Horizontal, "Max", Qt::DisplayRole);

    item = new QStandardItem();
    item->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
    m_model->setItem(0, 16, item);
    m_model->setHeaderData(16, Qt::Horizontal, "CV", Qt::DisplayRole);
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
    for (int row = 0; row < m_test.getMeasurementCount(); row++)
    {
        GripStrengthMeasurement measurement = m_test.getMeasurement(row);

        QStandardItem* exam_id = m_model->item(row, 0);
        if (Q_NULLPTR == exam_id)
        {
            exam_id = new QStandardItem();
            exam_id->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
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
            test_id->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
            m_model->setItem(row, 1, test_id);
        }
        if (test_id) {
            test_id->setData(measurement.getAttribute("test_id").value(), Qt::DisplayRole);
        }

        QStandardItem* position = m_model->item(row, 2);
        if (Q_NULLPTR == position)
        {
            position = new QStandardItem();
            position->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
            m_model->setItem(row, 2, position);
        }
        if (position) {
            qDebug() << "position: " << measurement.getAttribute("position").value();
            position->setData(measurement.getAttribute("position").value(), Qt::DisplayRole);
        }

        QStandardItem* side = m_model->item(row, 3);
        if(Q_NULLPTR == side)
        {
            side = new QStandardItem();
            side->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
            m_model->setItem(row, 3, side);
        }
        if (side) {
            side->setData(measurement.getAttribute("side").value(), Qt::DisplayRole);
        }

        QStandardItem* rep1 = m_model->item(row, 4);
        if(Q_NULLPTR == rep1)
        {
            rep1 = new QStandardItem();
            rep1->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
            m_model->setItem(row, 3, rep1);
        }
        if (rep1) {
            rep1->setData(measurement.getAttribute("Rep1").value(), Qt::DisplayRole);
        }
        rep1->setData(measurement.getAttribute("Rep1").value(), Qt::DisplayRole);

        QStandardItem* rep2 = m_model->item(row, 4);
        if(Q_NULLPTR == side)
        {
            rep2 = new QStandardItem();
            rep2->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
            m_model->setItem(row, 3, rep2);
        }
        if (rep2) {
            rep2->setData(measurement.getAttribute("Rep2").value(), Qt::DisplayRole);
        }
        rep2->setData(measurement.getAttribute("Rep2").value(), Qt::DisplayRole);

        QStandardItem* rep3 = m_model->item(row, 4);
        if(Q_NULLPTR == side)
        {
            rep1 = new QStandardItem();
            rep1->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
            m_model->setItem(row, 3, rep1);
        }
        if (rep1) {
            rep1->setData(measurement.getAttribute("side").value(), Qt::DisplayRole);
        }
        rep1->setData(measurement.getAttribute("Rep1").value(), Qt::DisplayRole);

        //QStandardItem* rep2 = m_model->item(row, 5);
        //rep2->setData(measurement.getAttribute("Rep2").value(), Qt::DisplayRole);

        //QStandardItem* rep3 = m_model->item(row, 6);
        //rep3->setData(measurement.getAttribute("Rep3").value(), Qt::DisplayRole);

        //QStandardItem* rep4 = m_model->item(row, 7);
        //rep4->setData(measurement.getAttribute("Rep4").value(), Qt::DisplayRole);

        //QStandardItem* rep5 = m_model->item(row, 8);
        //rep5->setData(measurement.getAttribute("Rep5").value(), Qt::DisplayRole);

        //QStandardItem* rep6 = m_model->item(row, 9);
        //rep6->setData(measurement.getAttribute("Rep6").value(), Qt::DisplayRole);

        //QStandardItem* rep7 = m_model->item(row, 10);
        //rep7->setData(measurement.getAttribute("Rep7").value(), Qt::DisplayRole);

        //QStandardItem* rep8 = m_model->item(row, 11);
        //rep8->setData(measurement.getAttribute("Rep8").value(), Qt::DisplayRole);

        //QStandardItem* rep9 = m_model->item(row, 12);
        //rep9->setData(measurement.getAttribute("Rep9").value(), Qt::DisplayRole);

        //QStandardItem* rep10= m_model->item(row, 13);
        //rep10->setData(measurement.getAttribute("Rep10").value(), Qt::DisplayRole);

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
