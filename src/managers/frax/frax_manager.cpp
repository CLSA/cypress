#include "frax_manager.h"
#include "data/frax/tests/frax_test.h"
#include "auxiliary/Utilities.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <QStandardItemModel>
#include <QMessageBox>

FraxManager::FraxManager(QSharedPointer<FraxSession> session)
    : ManagerBase(session)
{
    m_runnableName = CypressSettings::readSetting("frax/runnableName").toString();
    m_runnablePath = CypressSettings::readSetting("frax/runnablePath").toString();
    m_outputFilePath = CypressSettings::readSetting("frax/outputFilePath").toString();
    m_inputFilePath = CypressSettings::readSetting("frax/inputFilePath").toString();
    m_temporaryFilePath = CypressSettings::readSetting("frax/temporaryFilePath").toString();

    m_country_code = CypressSettings::readSetting("frax/countryCode").toString();
    m_type_code = CypressSettings::readSetting("frax/typeCode").toString();

    m_test.reset(new FraxTest);

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
    m_inputKeyList << "glucocorticoid";
    m_inputKeyList << "rheumatoid_arthritis";
    m_inputKeyList << "secondary_osteoporosis";
    m_inputKeyList << "alcohol";
    m_inputKeyList << "femoral_neck_bmd";

    if (m_debug) {
        qDebug() << "FraxManager";

        qDebug() << session->getSessionId();
        qDebug() << session->getBarcode();
        qDebug() << session->getInterviewer();

        qDebug() << session->getInputData();
    }
}

bool FraxManager::isInstalled(bool printMissing)
{
    const bool isSimMode = CypressSettings::isSimMode();
    if (isSimMode)
        return true;

    const QString runnableName = CypressSettings::readSetting("frax/runnableName").toString();
    const QString runnablePath = CypressSettings::readSetting("frax/runnablePath").toString();

    const QString outputFilePath = CypressSettings::readSetting("frax/outputFilePath").toString();
    const QString inputFilePath = CypressSettings::readSetting("frax/inputFilePath").toString();

    const QString temporaryFilePath = CypressSettings::readSetting("frax/temporaryFilePath").toString();

    const QString country_code = CypressSettings::readSetting("frax/countryCode").toString();
    const QString type_code = CypressSettings::readSetting("frax/typeCode").toString();

    bool installed = true;

    if (runnableName.isNull() || runnableName.isEmpty()) {
        if (printMissing)
            qDebug() << "runnableName is not defined";
        installed = false;
    }

    if (runnablePath.isNull() || runnablePath.isEmpty()) {
        if (printMissing)
            qDebug() << "runnablePath is not defined";
        installed = false;
    }

    if (outputFilePath.isNull() || outputFilePath.isEmpty()) {
        if (printMissing)
            qDebug() << "outputFilePath is not defined";
        installed = false;
    }

    if (inputFilePath.isNull() || inputFilePath.isEmpty()) {
        if (printMissing)
            qDebug() << "inputFilePath is not defined";
        installed = false;
    }

    if (temporaryFilePath.isNull() || temporaryFilePath.isEmpty()) {
        if (printMissing)
            qDebug() << "temporaryFile is not defined";
        installed = false;
    }

    if (country_code.isNull() || country_code.isEmpty()) {
        if (printMissing)
            qDebug() << "countryCode is not defined";
        installed = false;
    }

    if (type_code.isNull() || type_code.isNull()) {
        if (printMissing)
            qDebug() << "type_code is not defined";
        installed = false;
    }

    const QFileInfo info(runnableName);
    const QDir workingDirectory(runnablePath);

    if (!info.exists()) {
        if (printMissing)
            qDebug() << "executable does not exist at " << runnableName;
        installed = false;
    }

    if (!info.isExecutable()) {
        if (printMissing)
            qDebug() << "executable can not be run at " << runnableName;
        installed = false;
    }

    if (!workingDirectory.exists()) {
        if (printMissing)
            qDebug() << "working directory does not exist at "
                     << runnablePath;
        installed = false;
    }

    return installed;
}

bool FraxManager::start()
{
    if (m_debug)
        qDebug() << "FraxManager::start";

    if (!setUp()) {
        emit error("Could not setup FRAX device, please contact support.");
        return false;
    }

    measure();
    return true;
}

void FraxManager::measure()
{
    if (m_debug)
        qDebug() << "FraxManager::measure";

    clearData();

    if (m_sim) {
        QVariantMap map;
        QJsonObject inputData = m_session->getInputData();

        map.insert("type", 						m_type_code);
        map.insert("country_code", 				m_country_code);

        map.insert("age", 						inputData.value("age"));
        map.insert("sex", 						inputData.value("sex"));
        map.insert("bmi", 						inputData.value("bmi"));
        map.insert("previous_fracture", 		inputData.value("previous_fracture"));
        map.insert("parent_hip_fracture", 		inputData.value("parent_hip_fracture"));
        map.insert("current_smoker", 			inputData.value("current_smoker"));
        map.insert("glucocorticoid", 			inputData.value("glucocorticoid"));
        map.insert("rheumatoid_arthritis", 		inputData.value("rheumatoid_arthritis"));
        map.insert("secondary_osteoporosis", 	inputData.value("secondary_osteoporosis"));
        map.insert("t_score", 					inputData.value("t_score"));
        map.insert("z_score", 					inputData.value("z_score"));
        map.insert("alcohol", 					inputData.value("alcohol"));
        map.insert("birth_date", 				inputData.value("birth_date"));
        map.insert("interview_date", 			inputData.value("interview_date"));
        map.insert("height", 					inputData.value("height"));
        map.insert("height2", 					inputData.value("height2"));

        m_test->simulate(map);

        emit dataChanged(m_test);
        emit canFinish();

        return;
    }

    if (m_process.state() != QProcess::NotRunning) {
        QMessageBox::critical(nullptr, "Error", "Program is already running");
        return;
    }

    m_process.start();

    if (!m_process.waitForStarted()) {
        QMessageBox::critical(nullptr, "Error", "Could not launch FRAX");
        return;
    }
}

void FraxManager::readOutput()
{
    if (m_debug)
        qDebug() << "FraxManager::readOutput";

    auto test = qSharedPointerCast<FraxTest>(m_test);

    if (!QFileInfo::exists(m_outputFilePath)) {
        emit error("FRAX results not found");
        return;
    }

    test->fromFile(m_outputFilePath);

    qDebug() << test->toJsonObject();

    finish();
}

void FraxManager::configureProcess()
{
    if (m_debug)
        qDebug() << "FraxManager::configureProcess";

    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started, this, [this]() {
        qDebug() << "process started: " << m_process.arguments().join(" ");
    });

    // read output from csv after process finishes
    connect(&m_process,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            &FraxManager::readOutput);

    // error occured,
    connect(&m_process, &QProcess::errorOccurred, this, [=](QProcess::ProcessError error) {
        QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"),
                                                                    Qt::SkipEmptyParts);
        if (m_debug)
            qDebug() << "ERROR: process error occured: " << s.join(" ").toLower();
    });

    connect(&m_process, &QProcess::stateChanged, this, [=](QProcess::ProcessState state) {
        QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"),
                                                                    Qt::SkipEmptyParts);
        if (m_debug)
            qDebug() << "process state: " << s.join(" ").toLower();
    });

    // blackbox.exe and input.txt file are present
    //
    QDir workingDirectory(m_runnablePath);
    QFileInfo inputFileInfo(m_inputFilePath);
    QFileInfo fraxExecutableInfo(m_runnableName);
    QFileInfo temporaryFileInfo(m_temporaryFilePath);

    if (!workingDirectory.exists()) {
        emit error("working directory does not exist");
        return;
    }

    if (!fraxExecutableInfo.exists()) {
        emit error("frax exe does not exist");
        return;
    }

    m_process.setProgram(m_runnableName);
    m_process.setWorkingDirectory(m_runnablePath);

    // backup original input.txt
    //
    //if (!QFileInfo::exists(m_temporaryFilePath)) {
    //    if (!QFile::copy(m_inputFilePath, m_temporaryFilePath))
    //        emit error("Could not configure FRAX");
    //}

    // generate input.txt file content
    // exclude interview barcode and language
    //
    QStringList list;
    QJsonObject sessionInputData = m_session->getInputData();

    foreach (const auto key, m_inputKeyList) {
        QVariant value = sessionInputData[key].toVariant();

        if ("sex" == key)
            value = value.toString()[0].toLower() == 'm' ? 0 : 1;

        else {
            if(QVariant::Bool == value.type())
                value = value.toUInt();
        }

        list << value.toString();
    }

    // Write input.txt file
    QString line = list.join(",");
    qDebug() << line;

    QFile input_file(m_inputFilePath);

    if (input_file.exists()) {
        if (!input_file.remove()) {
            qDebug() << "could not remove file";
        }
    }

    if(input_file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&input_file);
        stream << line << Qt::endl;
        input_file.close();

        if (m_debug)
        {
            qDebug() << "populated input.txt file " << m_inputFilePath;
            qDebug() << "content = " << line;
        }
    }
    else
        emit error("Failed writing to input file");
}

bool FraxManager::clearData()
{
    if (m_debug)
        qDebug() << "FraxManager::clearData";

    m_test->reset();
    return true;
}


// Set up device
bool FraxManager::setUp()
{
    if (m_debug)
        qDebug() << "FraxManager::setUp";

    clearData();

    if (!cleanUp()) {
        return false;
    }

    configureProcess();

    return true;
}

// Clean up the device for next time
bool FraxManager::cleanUp()
{
    // remove blackbox.exe generated output.txt file
    //
    if (m_debug)
        qDebug() << "FraxManager::cleanUp";

    if(QProcess::NotRunning != m_process.state())
        m_process.close();


    if (QFileInfo::exists(m_outputFilePath)) {
        QFile outputFile(m_outputFilePath);
        if (!outputFile.remove()) {
            qDebug() << "could not remove output file";
        }
        else {
            qDebug() << "removed output file";
        }
    }

    return true;
}
