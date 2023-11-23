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
    m_executablePath = CypressSettings::readSetting("frax/executablePath").toString();
    m_workingDirectoryPath = CypressSettings::readSetting("frax/workingDirectoryPath").toString();
    m_outputFile = CypressSettings::readSetting("frax/outputFilePath").toString();
    m_inputFile = CypressSettings::readSetting("frax/inputFilePath").toString();
    m_temporaryFile = CypressSettings::readSetting("frax/temporaryFilePath").toString();

    m_country_code = CypressSettings::readSetting("frax/countryCode").toString();
    m_type_code = CypressSettings::readSetting("frax/typeCode").toString();

    m_test.reset(new FraxTest);
}

bool FraxManager::isInstalled()
{
    return true;
}

void FraxManager::start()
{
    if (m_debug)
    {
        qDebug() << "FraxManager::start";
    }

    configureProcess();

    emit started(m_test.get());
    emit canMeasure();
}


void FraxManager::measure()
{
    if (m_debug)
    {
        qDebug() << "FraxManager::measure";
    }

    m_test->reset();

    if (CypressSettings::isSimMode())
    {
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
    if (m_debug)
    {
        qDebug() << "FraxManager::readOutput";
    }

    if (QProcess::NormalExit != m_process.exitStatus())
    {

        QMessageBox::critical(nullptr, "Error", "FRAX hasn't finished running");

        return;
    }

    FraxTest* test = static_cast<FraxTest*>(m_test.get());
    if(QFileInfo::exists(m_outputFile))
    {
        test->fromFile(m_outputFile);
        if(test->isValid())
        {
            emit measured(test);
            emit canFinish();
        }
        else
        {
            QMessageBox::critical(nullptr, "Error", "FRAX results are invalid");
        }
    }
    else
    {
        QMessageBox::critical(nullptr, "Error", "FRAX results not found");
    }
}

void FraxManager::configureProcess()
{
    if (m_debug)
    {
        qDebug() << "FraxManager::configureProcess";
    }

    // blackbox.exe and input.txt file are present
    //
    QDir working(m_workingDirectoryPath);
    if(working.exists() && QFileInfo::exists(m_inputFile) && QFileInfo::exists(m_executablePath))
    {
        m_process.setProgram(m_executablePath);
        m_process.setWorkingDirectory(m_workingDirectoryPath);

        // backup original input.txt
        //
        if(!QFileInfo::exists(m_temporaryFile))
        {
            if(!QFile::copy(m_inputFile, m_temporaryFile))
            {
                QMessageBox::critical(nullptr, "Error", "Failed to backup data. Please contact support");
            }
        }

        // generate input.txt file content
        // exclude interview barcode and language
        //
        QStringList list;
        foreach(const auto key, m_inputKeyList)
        {
            if("barcode" == key || "language" == key)
            {
                continue;
            }

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

            if (m_debug)
            {
                qDebug() << "populated input.txt file " << m_inputFile;
                qDebug() << "content = " << line;
            }
        }
        else
        {
            QMessageBox::critical(nullptr, "Error", "Failed writing to input file");
        }
    }
    else
    {
        QMessageBox::critical(nullptr, "Error", "Failed to configure FRAX process");
    }
}

bool FraxManager::clearData()
{
    if (m_debug)
    {
        qDebug() << "FraxManager::clearData";
    }

    m_test->reset();
    return true;
}

void FraxManager::finish()
{
    if (m_debug)
    {
        qDebug() << "FraxManager::finish";
    }

    QJsonObject responseJson{};

    int answer_id = m_session->getAnswerId();

    QJsonObject testJson = m_test->toJsonObject();
    testJson.insert("session", m_session->getJsonObject());

    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    QString answerUrl = CypressSettings::getAnswerUrl(answer_id);
    sendHTTPSRequest("PATCH", answerUrl, "application/json", serializedData);

    emit success("sent");

    cleanUp();
}

// Set up device
bool FraxManager::setUp()
{
    if (m_debug)
    {
        qDebug() << "FraxManager::setUp";
    }

    configureProcess();

    return true;
}

// Clean up the device for next time
bool FraxManager::cleanUp()
{
    if (m_debug)
    {
        qDebug() << "FraxManager::cleanUp";
    }

    m_test.reset();

    if(QProcess::NotRunning != m_process.state())
    {
        m_process.close();
    }

    // remove blackbox.exe generated output.txt file
    //
    if(QFileInfo::exists(m_outputFile))
    {
        QFile outputFile(m_outputFile);
        if (!outputFile.remove())
        {
            QMessageBox::critical(nullptr, "Error", "Could not remove the FRAX output file");
        }
    }
    else
    {
        QMessageBox::warning(nullptr, "Warning", "Could not find the output file to be removed");
    }

    // remove the default input.txt file
    //
    if(!QFileInfo::exists(m_temporaryFile))
    {
        // remove the input file containing participant data
        QFile inputFile(m_inputFile);
        if (!inputFile.exists())
        {
            QMessageBox::critical(nullptr, "Error", "Could not find the input file");
        }

        if (!inputFile.remove())
        {
            QMessageBox::critical(nullptr, "Error", "Could not remove the input file");
        }

        // restore backup, default input.txt
        if (!QFile::copy(m_temporaryFile, m_inputFile))
        {
            QMessageBox::critical(nullptr, "Error", "Could not restore backup");
        }

        // remove the temporary file
        QFile tempFile(m_temporaryFile);
        if (!tempFile.remove())
        {
            QMessageBox::critical(nullptr, "Error", "Could not remove the temporary file");
        }
    }
    else
    {
        QMessageBox::critical(nullptr, "Error", "Could not find the FRAX backup. Please contact support.");
    }

    return true;
}
