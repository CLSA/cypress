#include "CDTTTest.h"
#include "QueryHelper.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>

// the minimum output data keys required from a successful a test
//
CDTTTest::CDTTTest()
{
    // test paramaters from Main sheet
    //
    m_outputKeyList << "subject_id";
    m_outputKeyList << "datetime";
    m_outputKeyList << "language";
    m_outputKeyList << "talker";
    m_outputKeyList << "mode";
    m_outputKeyList << "digits";
    m_outputKeyList << "list_number";
    m_outputKeyList << "msk_signal";
    m_outputKeyList << "test_ear";
    m_outputKeyList << "sp_level";
    m_outputKeyList << "msk_level";

    // adaptive test results summary from Main sheet
    //
    m_outputKeyList << "speech_reception_threshold";
    m_outputKeyList << "standard_deviation";
    m_outputKeyList << "reversal_count";

    m_outputKeyList << "trial_count";
}

//TODO: implement void CDTTTest::simulate()
//

void CDTTTest::fromFile(const QString &fileName)
{
    if(QFileInfo::exists(fileName))
    {
        qDebug() << "OK, reading input file " << fileName;

        //TODO: impl for linux or insert ifdef OS blockers
        //
        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "xlsx_connection");
        db.setDatabaseName("DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DBQ=" + fileName);
        if(db.open())
        {
            reset();

            bool ok = readBarcode(db);
            if(ok)
            {
                ok = readMetaData(db);
            }
            if(ok)
            {
                ok = readSummary(db);
            }
            if(ok)
            {
                ok = readTrialData(db);
            }

            /*
            if(queryTestMetaData(db))
            {
              queryTestMeasurements(db);
              int n = getNumberOfMeasurements();
              addMetaDataCharacteristic("number of measurements", n);
            }
            */
        }
        db.close();
        db.removeDatabase("xlsx_connection");
    }
}

bool CDTTTest::readBarcode(const QSqlDatabase &db)
{
    // get the barcode <=> subject_id
    //
    qDebug() << "-----------getting barcode...";

    QStringList header;
    header << "Subject ID:";
    QueryHelper helper("A1","B1","Main");
    helper.setOrder(QueryHelper::Order::Row);
    helper.setHeader(header);
    bool ok = true; //helper.buildQuery(db);
    if((ok = helper.buildQuery(db)))
    {
      helper.processQuery();
      QJsonObject obj = helper.getOutput();
      if(obj.contains("header_valid") &&
         obj["header_valid"].toBool())
      {
         addMetaDataCharacteristic("subject_id",obj["Subject ID:"].toString());
      }
      else
          ok = false;

      qDebug() << helper.getOutput().toVariantMap();
    }
    return ok;
}

bool CDTTTest::readMetaData(const QSqlDatabase &db)
{
    qDebug() << "-----------getting meta data...";
    // get the meta data
    QueryHelper helper = QueryHelper("A4","J5","Main");
    bool ok = true;//helper.buildQuery(db);
    if((ok = helper.buildQuery(db)))
    {
      QStringList header;
      header << "Date & time"
             << "Language"
             << "Talker"
             << "Mode"
             << "Digits"
             << "List #"
             << "MSK signal"
             << "Test Ear"
             << "SP level"
             << "MSK level";
      helper.setHeader(header);
      helper.processQuery();
      QJsonObject obj = helper.getOutput();
      if(obj.contains("header_valid") &&
         obj["header_valid"].toBool())
      {
         QString s = obj["Date & time"].toString().simplified().replace(", "," ");
         addMetaDataCharacteristic("datetime",QDateTime::fromString(s, "yyyy-MM-dd hh:mm:ss"));
         addMetaDataCharacteristic("language",obj["Language"].toString());
         addMetaDataCharacteristic("mode",obj["Mode"].toString());
         addMetaDataCharacteristic("digits",obj["Digits"].toString());
         addMetaDataCharacteristic("list_number",obj["List #"].toInt());
         addMetaDataCharacteristic("msk_signal",obj["MSK Signal"].toString());
         addMetaDataCharacteristic("test_ear",obj["Test Ear"].toString());
         addMetaDataCharacteristic("sp_level",obj["SP level"].toDouble());
         addMetaDataCharacteristic("msk_level",obj["MSK level"].toDouble());
      }
      else
          ok = false;

      qDebug() << helper.getOutput().toVariantMap();
    }
    return ok;
}

bool CDTTTest::readSummary(const QSqlDatabase &db)
{
    qDebug() << "-----------getting summary...";
    // get the summary results
    QueryHelper helper = QueryHelper("K4","M5","Main");
    bool ok = true;//helper.buildQuery(db);
    if((ok = helper.buildQuery(db)))
    {
      QStringList header;
      header << "SRT" << "St# Dev#" << "Reversals";
      helper.setHeader(header);
      helper.processQuery();
      QJsonObject obj = helper.getOutput();
      if(obj.contains("header_valid") &&
         obj["header_valid"].toBool())
      {
        CDTTMeasurement m;
        m.setCharacteristic("name","speech_reception_threshold");
        m.setCharacteristic("value",obj["SRT"].toDouble());
        addMeasurement(m);
        m.reset();
        m.setCharacteristic("name","standard_deviation");
        m.setCharacteristic("value",obj["St# Dev#"].toDouble());
        addMeasurement(m);
        m.reset();
        m.setCharacteristic("name","reversal_count");
        m.setCharacteristic("value",obj["Reversals"].toInt());
        addMeasurement(m);
      }
      else
        ok = false;

      qDebug() << helper.getOutput().toVariantMap();
    }
    return ok;
}

bool CDTTTest::readTrialData(const QSqlDatabase &db)
{
    qDebug() << "-----------getting number of responses...";
    // get the number of stimulus and response digits
    // by querying the first column of the sheet
    //
    QString sheet = QString("%1-%2").arg(
      getMetaDataCharacteristic("language").toString(),
      getMetaDataCharacteristic("talker").toString());

    QueryHelper helper = QueryHelper("A13","A60",sheet);
    bool ok = true;//helper.buildQuery(db);
    if((ok = helper.buildQuery(db)))
    {
      helper.processQuery();
      qDebug() << helper.getOutput().toVariantMap();
    }
    else
      return ok;

    QJsonObject obj = helper.getOutput();
    QJsonArray arr = obj["column_0"].toArray();
    int num_row = arr.last().toInt();

    CDTTMeasurement m;
    m.setCharacteristic("name","trial_count");
    m.setCharacteristic("value",num_row);

    QString cell_suffix = QString::number(13+num_row-1);
    QString endCell = "D" + cell_suffix;

    qDebug() << "-----------getting stimulus digits...";
    // get the stimulus digits
    helper = QueryHelper("B13",endCell,sheet);
    if((ok = helper.buildQuery(db)))
    {
      helper.setOrder(QueryHelper::Order::Row);
      helper.setPrefix("stimulus_");
      helper.processQuery();
      obj = helper.getOutput();
      for(auto it = obj.constBegin(), end=obj.constEnd(); it!=end; it++)
      {
         CDTTMeasurement m;
         m.setCharacteristic("name",it.key());
         m.setCharacteristic("value",it.value().toVariant().toStringList().join(","));
      }
    }
    else
      return ok;

    qDebug() << "-----------getting response digits...";
    // get the response digits
    endCell = "G" + cell_suffix;
    helper = QueryHelper("E13",endCell,sheet);
    if((ok = helper.buildQuery(db)))
    {
      helper.setOrder(QueryHelper::Order::Row);
      helper.setPrefix("response_");
      helper.processQuery();
      obj = helper.getOutput();
      for(auto it = obj.constBegin(), end=obj.constEnd(); it!=end; it++)
      {
         CDTTMeasurement m;
         m.setCharacteristic("name",it.key());
         m.setCharacteristic("value",it.value().toVariant().toStringList().join(","));
      }
    }

    return ok;
}

// String representation for debug and GUI display purposes
//
QString CDTTTest::toString() const
{
    QString s;
    if(isValid())
    {
        QStringList l;
        for(auto&& x : m_measurementList)
        {
            l << x.toString();
        }
        s = l.join("\n");
    }
    return s;
}

bool CDTTTest::isValid() const
{
    bool okMeta = true;
    for(auto&& key : m_outputKeyList)
    {
      if(!hasMetaDataCharacteristic(key))
      {
         okMeta = false;
         qDebug() << "ERROR: missing test meta data " << key;
         break;
       }
    }
    bool okTest = 0 < getNumberOfMeasurements();
    if(okTest)
    {
      for(auto&& x : m_measurementList)
      {
        if(!x.isValid())
        {
          okTest = false;
          qDebug() << "ERROR: invalid test measurement";
          break;
        }
      }
    }

    return okMeta && okTest;
}

// String keys are converted to snake_case
//
QJsonObject CDTTTest::toJsonObject() const
{
    QJsonArray jsonArr;
    for(auto&& x : m_measurementList)
    {
        jsonArr.append(x.toJsonObject());
    }
    QJsonObject json;
    json.insert("test_meta_data",m_metaData.toJsonObject());
    json.insert("test_results",jsonArr);
    return json;
}
