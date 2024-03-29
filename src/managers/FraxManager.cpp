#include "FraxManager.h"

#include "../auxiliary/Utilities.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonObject>
#include <QSettings>
#include <QStandardItemModel>

FraxManager::FraxManager(QObject* parent):
    ManagerBase(parent)
{
    setGroup("frax");
    m_col = 1;
    m_row = 4;

    // all managers must check for barcode and language input values
    //
    m_inputKeyList << "barcode";
    m_inputKeyList << "language";

    // key/value pairs needed to build input.txt
    //
    m_inputKeyList << "type";
    m_inputKeyList << "country_code";
    m_inputKeyList << "age";
    m_inputKeyList << "sex";
    m_inputKeyList << "body_mass_index";
    m_inputKeyList << "previous_fracture";
    m_inputKeyList << "parent_hip_fracture";
    m_inputKeyList << "current_smoker";
    m_inputKeyList << "gluccocorticoid";
    m_inputKeyList << "rheumatoid_arthritis";
    m_inputKeyList << "secondary_osteoporosis";
    m_inputKeyList << "alcohol";
    m_inputKeyList << "femoral_neck_bmd";

    m_test.setExpectedMeasurementCount(4);
}

void FraxManager::start()
{
    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started,
        this, [this]() {
            qDebug() << "process started: " << m_process.arguments().join(" ");
        });

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, &FraxManager::readOutput);

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

void FraxManager::initializeModel()
{
    // allocate 1 columns x 4 rows of frax measurement items
    //
    for(int row=0; row<m_row; row++)
    {
      QStandardItem* item = new QStandardItem();
      m_model->setItem(row, 0, item);
    }
    m_model->setHeaderData(0, Qt::Horizontal, "Frax 10 Year Fracture Risk Probabilities", Qt::DisplayRole);
}

void FraxManager::updateModel()
{
    // add the four probability measurements
    //
    if(m_test.isValid())
    {
      for(int row = 0; row < m_test.getMeasurementCount(); row++)
      {
        QStandardItem* item = m_model->item(row, 0);
        if(Q_NULLPTR == item)
        {
            item = new QStandardItem();
            m_model->setItem(row, 0, item);
        }
        item->setData(m_test.getMeasurement(row).toString(), Qt::DisplayRole);
      }
    }
    else
    {
      for(int row = 0; row < m_row; row++)
      {
          QStandardItem* item = m_model->item(row,0);
          if(Q_NULLPTR == item)
          {
            item = new QStandardItem();
            m_model->setItem(row,0,item);
          }
          item->setData(QString(), Qt::DisplayRole);
      }
    }
}

void FraxManager::loadSettings(const QSettings& settings)
{
    // the full spec path name including exe name
    // eg., ../frax_module/blackbox.exe
    //
    QString exeName = settings.value(getGroup() + "/client/exe").toString();
    selectRunnable(exeName);
}

void FraxManager::saveSettings(QSettings* settings) const
{
    if(!m_runnableName.isEmpty())
    {
        settings->beginGroup(getGroup());
        settings->setValue("client/exe", m_runnableName);
        settings->endGroup();
        if(m_verbose)
          qDebug() << "wrote exe fullspec path to settings file";
    }
}

QJsonObject FraxManager::toJsonObject() const
{
    QJsonObject json = m_test.toJsonObject();
    if(Constants::RunMode::modeSimulate != m_mode)
    {
      QFile ofile(m_outputFile);
      ofile.open(QIODevice::ReadOnly);
      QByteArray buffer = ofile.readAll();
      json.insert("test_output_file",QString(buffer.toBase64()));
      json.insert("test_output_file_mime_type","txt");
    }
    json.insert("test_input",QJsonObject::fromVariantMap(m_inputData));
    return json;
}

bool FraxManager::isDefined(const QString &exeName) const
{
    if(Constants::RunMode::modeSimulate == m_mode)
    {
       return true;
    }
    bool ok = false;
    if(!exeName.isEmpty())
    {
        QFileInfo info(exeName);
        if(info.exists() && info.isExecutable())
        {
            ok = true;
        }
    }
    return ok;
}

void FraxManager::selectRunnable(const QString &runnableName)
{
    if(isDefined(runnableName))
    {
        QFileInfo info(runnableName);
        m_runnableName = runnableName;
        m_runnablePath = info.absolutePath();
        m_outputFile = QDir(m_runnablePath).filePath("output.txt");
        m_inputFile =  QDir(m_runnablePath).filePath("input.txt");
        m_temporaryFile = QDir(m_runnablePath).filePath("input_ORIG.txt");

        emit runnableSelected();
        configureProcess();
    }
    else
        emit canSelectRunnable();
}

void FraxManager::measure()
{
    if(Constants::RunMode::modeSimulate == m_mode)
    {
        readOutput();
        return;
    }
    clearData();
    // launch the process
    qDebug() << "starting process from measure";
    m_process.start();
}

void FraxManager::setInputData(const QVariantMap& input)
{
    m_inputData = input;
    if(Constants::RunMode::modeSimulate == m_mode)
    {
      if(!input.contains("barcode"))
        m_inputData["barcode"] = Constants::DefaultBarcode;
      if(!input.contains("language"))
        m_inputData["language"] = "en";

      if(!input.contains("type"))
        m_inputData["type"] = "t";
      if(!input.contains("country_code"))
        m_inputData["country_code"] = 19;
      if(!input.contains("age"))
        m_inputData["age"] = 84.19;
      if(!input.contains("sex"))
        m_inputData["sex"] = "male";
      if(!input.contains("body_mass_index"))
        m_inputData["body_mass_index"] = 24.07;
      if(!input.contains("previous_fracture"))
        m_inputData["previous_fracture"] = false;
      if(!input.contains("parent_hip_fracture"))
        m_inputData["parent_hip_fracture"] = false;
      if(!input.contains("current_smoker"))
        m_inputData["current_smoker"] = false;
      if(!input.contains("gluccocorticoid"))
        m_inputData["gluccocorticoid"] = false;
      if(!input.contains("rheumatoid_arthritis"))
        m_inputData["rheumatoid_arthritis"] = false;
      if(!input.contains("secondary_osteoporosis"))
        m_inputData["secondary_osteoporosis"] = false;
      if(!input.contains("alcohol"))
        m_inputData["alcohol"] = false;
      if(!input.contains("femoral_neck_bmd"))
        m_inputData["femoral_neck_bmd"] = -1.1;
    }
    bool ok = true;
    QMap<QString,QMetaType::Type> typeMap {
        {"barcode",QMetaType::Type::QString},
        {"language",QMetaType::Type::QString},
        {"type",QMetaType::Type::QString},
        {"country_code",QMetaType::Type::Int},
        {"sex",QMetaType::Type::QString},
        {"previous_fracture",QMetaType::Type::Bool},
        {"parent_hip_fracture",QMetaType::Type::Bool},
        {"current_smoker",QMetaType::Type::Bool},
        {"gluccocorticoid",QMetaType::Type::Bool},
        {"rheumatoid_arthritis",QMetaType::Type::Bool},
        {"secondary_osteoporosis",QMetaType::Type::Bool},
        {"alcohol",QMetaType::Type::Bool},
        {"age",QMetaType::Type::Double},
        {"body_mass_index",QMetaType::Type::Double},
        {"femoral_neck_bmd",QMetaType::Type::Double}
    };
    foreach(const auto key, m_inputKeyList)
    {
      if(!m_inputData.contains(key))
      {
        ok = false;
        if(m_verbose)
          qDebug() << "ERROR: missing expected input " << key;
        break;
      }
      else
      {
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
    }
    if(!ok)
    {
      if(m_verbose)
        qDebug() << "ERROR: invalid input data";

      emit message(tr("ERROR: the input data is incorrect"));
      m_inputData = QVariantMap();
    }
    else
      configureProcess();
}

void FraxManager::readOutput()
{
    if(Constants::RunMode::modeSimulate == m_mode)
    {
        if(m_verbose)
          qDebug() << "simulating read out";

        m_test.simulate(m_inputData);
        emit message(tr("Ready to save results..."));
        emit canWrite();
        updateModel();
        return;
    }

    if (QProcess::NormalExit != m_process.exitStatus())
    {
        qDebug() << "ERROR: process failed to finish correctly: cannot read output";
        return;
    }
    else
        qDebug() << "process finished successfully";

    if(QFileInfo::exists(m_outputFile))
    {
        qDebug() << "found output txt file " << m_outputFile;
        m_test.fromFile(m_outputFile);
        if(m_test.isValid())
        {
            emit message(tr("Ready to save results..."));
            emit canWrite();
        }
        else
            qDebug() << "ERROR: input from file produced invalid test results";

        updateModel();
    }
    else
        qDebug() << "ERROR: no output.txt file found";
}

void FraxManager::configureProcess()
{
    if(Constants::RunMode::modeSimulate == m_mode &&
       !m_inputData.isEmpty())
    {
        emit message(tr("Ready to measure..."));
        emit canMeasure();
        return;
    }

    // blackbox.exe and input.txt file are present
    //
    QDir working(m_runnablePath);
    if(isDefined(m_runnableName) &&
       working.exists() && QFileInfo::exists(m_inputFile) &&
       !m_inputData.isEmpty())
    {
        qDebug() << "OK: configuring command";

        m_process.setProgram(m_runnableName);
        m_process.setWorkingDirectory(m_runnablePath);

        qDebug() << "process working dir: " << m_runnablePath;

        // backup original intput.txt
        //
        if(!QFileInfo::exists(m_temporaryFile))
        {
          if(QFile::copy(m_inputFile, m_temporaryFile))
              qDebug() << "wrote backup of"<< m_inputFile << "to" << m_temporaryFile;
          else
              qDebug() << "ERROR: failed to backup default " << m_inputFile;
        }
        // generate input.txt file content
        // exclude interview barcode and language
        //
        QStringList list;
        foreach(const auto key, m_inputKeyList)
        {
            if("barcode" == key || "language" == key) continue;

            QVariant value = m_inputData[key];
            if("sex" == key)
              value = "male" == value.toString() ? 0 : 1;
            else if("femoral_neck_bmd" == key)
            {
                value = Utilities::tscore(value.toDouble());
            }
            else
            {
              if(QVariant::Bool == value.type())
              {
                value = value.toUInt();
              }
            }
            list << value.toString();
        }
        QString line = list.join(",");
        QFile ofile(m_inputFile);
        if(ofile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream stream(&ofile);
            stream << line << Qt::endl;
            ofile.close();
            qDebug() << "populated input.txt file " << m_inputFile;
            qDebug() << "content = " << line;
        }
        else
        {
            qDebug() << "ERROR: failed writing to " << m_inputFile;
            return;
        }

        emit message(tr("Ready to measure..."));
        emit canMeasure();
    }
    else
        qDebug() << "failed to configure process";
}

void FraxManager::clearData()
{
    m_test.reset();
    updateModel();
}

void FraxManager::finish()
{
    m_test.reset();
    if(QProcess::NotRunning != m_process.state())
    {
        m_process.close();
    }

    // remove blackbox.exe generated output.txt file
    //
    if(!m_outputFile.isEmpty() && QFileInfo::exists(m_outputFile))
    {
        qDebug() << "removing output file " << m_outputFile;
        QFile ofile(m_outputFile);
        ofile.remove();
        m_outputFile.clear();
    }

    // remove the default input.txt file
    //
    if(!m_temporaryFile.isEmpty() && QFileInfo::exists(m_temporaryFile))
    {
        // remove the inputfile first
        QFile ifile(m_inputFile);
        ifile.remove();
        QFile::copy(m_temporaryFile, m_inputFile);
        qDebug() << "restored backup from " << m_temporaryFile;
        QFile tempFile(m_temporaryFile);
        tempFile.remove();
        m_temporaryFile.clear();
    }
}
