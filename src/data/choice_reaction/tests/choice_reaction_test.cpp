#include "choice_reaction_test.h"
#include "../measurements/choice_reaction_measurement.h"

#include <QDateTime>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

// the minimum output data keys required from a successful a test
//
ChoiceReactionTest::ChoiceReactionTest()
{
    m_outputKeyList << "user_id";
    m_outputKeyList << "start_datetime";
    m_outputKeyList << "end_datetime";
    m_outputKeyList << "interviewer_id";
    m_outputKeyList << "number_of_measurements";
    m_outputKeyList << "version";
}

void ChoiceReactionTest::fromFile(const QString &fileName)
{
    QFile ifile(fileName);
    if(ifile.open(QIODevice::ReadOnly))
    {
        qDebug() << "OK, reading input file " << fileName;

        QTextStream stream(&ifile);
        int clinic_pos = -1;
        int version_pos = -1;
        int userid_pos = -1; // the participant's interview barcode
        int interviewerid_pos = -1;  // the interviewer id

        reset();

        qDebug() << "begin reading text stream";
        int n_line = 0;
        while(!stream.atEnd())
        {
            QString s = stream.readLine();
            QStringList list = s.split(",");
            qDebug() << "reading line "<< QString::number(++n_line) <<" with number of items = " << QString::number(list.size());
            if(!list.empty())
            {
                int code = list.at(0).toInt();
                if((ChoiceReactionMeasurement::TEST_CODE - 1) == code)
                {
                    // get the position of the meta data key:
                    // Version, InterviewerId, UserId, Clinic
                    //
                    clinic_pos = list.indexOf(QLatin1String("Clinic"));
                    version_pos = list.indexOf(QLatin1String("Version"));
                    userid_pos = list.indexOf(QLatin1String("UserId"));
                    interviewerid_pos = list.indexOf(QLatin1String("InterviewerId"));

                    qDebug() << "found header item positions at line " << QString::number(n_line);
                }
                else if(ChoiceReactionMeasurement::TEST_CODE == code)
                {
                    QSharedPointer<ChoiceReactionMeasurement> measure(new ChoiceReactionMeasurement);
                    measure->fromString(s);
                    addMeasurement(measure);
                    qDebug() << "found " << (measure->isValid() ? "VALID" : "INVALID")
                             << "measurement item positions at line " << QString::number(n_line);
                    qDebug() << measure;
                }
                else if((ChoiceReactionMeasurement::TEST_CODE+1) == code)
                {
                    qDebug() << "found last line " << QString::number(n_line);

                    if(-1 != version_pos)
                    {
                      addMetaData("version",list.at(version_pos));
                      qDebug() << "adding version meta info";
                    }
                    if(-1 != clinic_pos)
                    {
                      addMetaData("clinic",list.at(clinic_pos));
                      qDebug() << "adding clinic meta info";
                    }
                    if(-1 != userid_pos)
                    {
                      addMetaData("user_id",list.at(userid_pos));
                      qDebug() << "adding user id meta info";
                    }
                    if(-1 != interviewerid_pos)
                    {
                      addMetaData("interviewer_id",list.at(interviewerid_pos));
                      qDebug() << "adding interviewer id meta info";
                    }
                    // get the position of the following meta data keys
                    // that are within the last line of the file with the correct code:
                    // EndDateTime, StartDateTimes
                    //
                    int pos = list.indexOf(QLatin1String("EndDateTime"));
                    if(-1 != pos)
                    {
                        QString s = list.at(pos+1);
                        s = s.remove(QRegExp("\\s(AM|PM)$")).trimmed();
                        qDebug() << "end datetime string " << s;

                        QDateTime d = QDateTime::fromString(s, "MM/dd/yyyy HH:mm:ss");
                        addMetaData("end_datetime",d);
                        qDebug() << "adding end datetime meta info";
                    }
                    pos = list.indexOf(QLatin1String("StartDateTimes"));
                    if(-1 != pos)
                    {
                        QString s = list.at(pos+1);
                        s = s.remove(QRegExp("\\s(AM|PM)$")).trimmed();
                        qDebug() << "start datetime string " << s;

                        QDateTime d = QDateTime::fromString(s, "MM/dd/yyyy HH:mm:ss");
                        addMetaData("start_datetime",d);
                        qDebug() << "adding start datetime meta info";
                    }
                }
                else
                {
                    qDebug() << "ERROR: cannot parse line, bad code";
                }
            }
            // additional meta data: the number of measurements
            int n = getMeasurementCount();
            addMetaData("number_of_measurements",n);

            // we do not know a priori what the count will be, set it here
            //
            this->setExpectedMeasurementCount(n);
        }
        qDebug() << "closed stream";
        ifile.close();
    }
}

void ChoiceReactionTest::simulate()
{
    addMetaData("version", "CLSA_ELCV");
    addMetaData("clinic", "CYPRESS");
    addMetaData("user_id", 1);

    for (int i = 0; i < getExpectedMeasurementCount(); i++)
    {
        QSharedPointer<ChoiceReactionMeasurement> measurement(new ChoiceReactionMeasurement());
        measurement->simulate();
        addMeasurement(measurement);
    }
}

// String representation for debug and GUI display purposes
//
QString ChoiceReactionTest::toString() const
{
    QString str;
    if(isValid())
    {
        QStringList list;
        foreach (const auto m, m_measurementList) {
            list << m->toString();
        }
        str = list.join("\n");
    }
    return str;
}

bool ChoiceReactionTest::isValid() const
{
    bool okMeta = true;
    foreach(const auto key, m_outputKeyList)
    {
      if(!hasMetaData(key))
      {
         okMeta = false;
         break;
       }
    }
    bool okTest = 0 < getMeasurementCount();
    if (okTest) {
       foreach (const auto m, m_measurementList) {
         if (!m->isValid()) {
                okTest = false;
                break;
         }
       }
    }
    return okMeta && okTest;
}

// String keys are converted to snake_case
//
QJsonObject ChoiceReactionTest::toJsonObject() const
{
    QJsonObject value{};
    QJsonArray jsonArr{};

    foreach (const auto m, m_measurementList) {
       jsonArr.append(m->toJsonObject());
    }

    QJsonObject json{};
    value.insert("metadata", m_metaData.toJsonObject());
    value.insert("results", jsonArr);
    value.insert("manual_entry", getManualEntryMode());

    return value;
}
