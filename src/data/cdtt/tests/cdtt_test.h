#ifndef CDTT_TEST_H
#define CDTT_TEST_H

#include "data/test_base.h"

#include <QJsonObject>

QT_FORWARD_DECLARE_CLASS(QSqlDatabase)

class CDTTTest : public TestBase
{
public:
    CDTTTest();
    ~CDTTTest() = default;

    // read from a MS Excel file using ODBC sql
    //
    void fromDatabase(const QSqlDatabase&);

    QString toString() const override;

    bool isValid() const override;

    QJsonObject toJsonObject() const override;

private:
    QList<QString> m_outputKeyList;
    QJsonObject m_jsonObj;

    bool readBarcode(const QSqlDatabase &);
    bool readMetaData(const QSqlDatabase &);
    bool readSummary(const QSqlDatabase &);
    bool readTrialData(const QSqlDatabase &);

};

Q_DECLARE_METATYPE(CDTTTest);

#endif // CDTT_TEST_H
