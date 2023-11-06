#include "cypress_application.h"
#include "frax_manager.h"

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
    m_test.reset(new FraxTest);
}

bool FraxManager::isAvailable()
{
    return false;
}

bool FraxManager::isInstalled()
{
    return false;
}

void FraxManager::start()
{
    configureProcess();

    emit started(m_test.get());
    emit canMeasure();
}


bool FraxManager::isDefined(const QString &exeName) const
{
    Q_UNUSED(exeName)
    return true;
}

void FraxManager::selectRunnable(const QString &runnableName)
{
    Q_UNUSED(runnableName)
}

void FraxManager::measure()
{
    m_test->reset();

    if (Cypress::getInstance().isSimulation())
    {
        QVariantMap map;
        QJsonObject inputData = m_session->getInputData();

        map.insert("type", 						inputData.value("type"));
        map.insert("country_code", 				inputData.value("country_code"));
        map.insert("age", 						inputData.value("age"));
        map.insert("sex", 						inputData.value("sex"));
        map.insert("bmi", 						inputData.value("bmi"));
        map.insert("previous_fracture", 		inputData.value("previous_fracture"));
        map.insert("parent_hip_fracture", 		inputData.value("parent_hip_fracture"));
        map.insert("current_smoker", 			inputData.value("current_smoker"));
        map.insert("gluccocorticoid", 			inputData.value("gluccocorticoid"));
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

        emit measured(m_test.get());
        emit canFinish();

        return;
    }

    m_process.start();
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

    FraxTest* test = static_cast<FraxTest*>(m_test.get());

    if(QFileInfo::exists(m_outputFile))
    {
        qDebug() << "found output txt file " << m_outputFile;
        test->fromFile(m_outputFile);
        if(test->isValid())
        {
            emit measured(test);
            emit canFinish();
        }
        else
        {
            qDebug() << "ERROR: input from file produced invalid test results";
        }
    }
    else
    {
        qDebug() << "ERROR: no output.txt file found";
    }
}

void FraxManager::configureProcess()
{
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
            {
                value = "male" == value.toString() ? 0 : 1;
            }
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


        // Write input.txt file
        QString line = list.join(",");
        QFile input_file(m_inputFile);

        if(input_file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream stream(&input_file);
            stream << line << Qt::endl;
            input_file.close();

            qDebug() << "populated input.txt file " << m_inputFile;
            qDebug() << "content = " << line;
        }
        else
        {
            qDebug() << "ERROR: failed writing to " << m_inputFile;
            QMessageBox::critical(nullptr, "Error", "Failed writing to input file");

            return;
        }
    }
    else
    {
        QMessageBox::critical(nullptr, "Error", "Failed to configure FRAX process");
    }
}

bool FraxManager::clearData()
{
    //m_test.reset();
    return false;
}

void FraxManager::finish()
{
    QJsonObject responseJson{};

    int answer_id = m_session->getAnswerId();

    QJsonObject testJson = m_test->toJsonObject();
    testJson.insert("session", m_session->getJsonObject());

    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    QString answerUrl = CypressSettings::getInstance().getAnswerUrl(answer_id);
    sendHTTPSRequest("PATCH", answerUrl, "application/json", serializedData);

    emit success("sent");

    cleanUp();
}

// Set up device
bool FraxManager::setUp()
{
    configureProcess();

    return true;
}

// Clean up the device for next time
bool FraxManager::cleanUp()
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

    return true;
}

void FraxManager::setInputData(const QVariantMap& inputData)
{
    Q_UNUSED(inputData)
}
