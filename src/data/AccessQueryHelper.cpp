#include "AccessQueryHelper.h"

#include <QSqlDatabase>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

AccessQueryHelper::AccessQueryHelper():
    m_operation(Operation::Count)
{
}

void AccessQueryHelper::setOperation(const AccessQueryHelper::Operation& op)
{
    m_operation = op;
}

void AccessQueryHelper::setHeader(const QStringList& header)
{
    m_header = header;
}

QVariant AccessQueryHelper::processQuery(const QVariantMap& input, const QSqlDatabase& db)
{
    QVariant result = false;
    if(!db.isOpen())
    {
        qDebug() << "ERROR: db is not open" << db.isOpenError();
        return result;
    }
    QSqlQuery query = QSqlQuery(db);
    QString q_str;
    if(Operation::Count == m_operation ||
       Operation::CountMeasures == m_operation)
    {
      if(Operation::Count == m_operation)
        q_str = QString(
          "SELECT COUNT(*) As Num FROM Patients "
          "WHERE ID=%1 "
          "AND BirthDate=#%2# "
          "AND Sex=%3").arg(
          input["barcode"].toString(),
          input["date_of_birth"].toDate().toString("yyyy-MM-dd"),
          input["sex"].toString()
        );
      else
        q_str = QString(
          "SELECT COUNT(*) AS Num FROM Measures AS m "
          "INNER JOIN Patients AS p ON p.PatientID=m.PatientID "
          "WHERE ID=%1 "
          "AND BirthDate=#%2# "
          "AND Sex=%3").arg(
          input["barcode"].toString(),
          input["date_of_birth"].toDate().toString("yyyy-MM-dd"),
          input["sex"].toString()
        );

      int count = -1;
      if(query.exec(q_str) && query.first())
      {
        QSqlRecord r = query.record();
        if(r.contains("Num"))
        {
          count = r.value("Num").toInt();
        }
      }
      result = QVariant(count);
    }
    else if(Operation::Delete == m_operation ||
            Operation::DeleteMeasures == m_operation)
    {
      if(Operation::DeleteMeasures == m_operation)
      {
        q_str = QString(
          "DELETE * FROM Measures "
          "WHERE PatientID IN ("
          "SELECT PatientID FROM Patients "
          "WHERE ID=%1 "
          "AND BirthDate=#%2# "
          "AND Sex=%3)").arg(
          input["barcode"].toString(),
          input["date_of_birth"].toDate().toString("yyyy-MM-dd"),
          input["sex"].toString()
          );
        if(query.exec(q_str))
        {
          result = true;
        }
      }
      else
      {
        q_str = QString(
          "DELETE * FROM Patients "
          "WHERE ID=%1 "
          "AND BirthDate=#%2# "
          "AND Sex=%3").arg(
          input["barcode"].toString(),
          input["date_of_birth"].toDate().toString("yyyy-MM-dd"),
          input["sex"].toString()
        );
        if(query.exec(q_str))
        {
          result = true;
        }
      }
    }
    else if(Operation::Insert == m_operation)
    {
      q_str = QString(
        "INSERT INTO Patients (BirthDate, Sex, GroupID, ID, RaceID)"
        "VALUES (#%1#,%2,%3,%4,%5)").arg(
        input["date_of_birth"].toDate().toString("yyyy-MM-dd"),
        input["sex"].toString(),
        QString::number(2),
        input["barcode"].toString(),
        QString::number(1)
      );
      if(query.exec(q_str))
      {
        result = true;
      }
    }
    else
    {
      q_str = QString(
        "SELECT DISTINCT * FROM Measures AS m "
        "INNER JOIN Patients AS p ON p.PatientID=m.PatientID "
        "WHERE ID=%1 "
        "AND SEX=%2 "
        "AND BirthDate=#%3#").arg(
        input["barcode"].toString(),
        input["sex"].toString(),
        input["date_of_birth"].toDate().toString("yyyy-MM-dd")
      );
      if(query.exec(q_str))
      {
          QList<QVariant> arr;
          while(query.next())
          {
            QSqlRecord r = query.record();
            QVariantMap obj;
            for(int i=0;i<r.count();i++)
            {
               QSqlField f = r.field(i);
               obj.insert(f.name(),f.value());
            }
            arr.push_back(obj);
          }
          result = arr.isEmpty() ? QVariant() : arr;
      }
    }

    query.finish();
    return result;
}
