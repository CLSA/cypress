#include <QtTest>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSql>
#include <QVariant>
#include <QMap>
#include <QString>
#include <QDate>

#include "data/tonometer/tests/tonometer_test.h"

// add necessary includes here

class TonometerTests : public QObject
{
    Q_OBJECT

public:
    TonometerTests();
    ~TonometerTests();

private slots:
    void test_case1();
};


TonometerTests::TonometerTests() {


}

TonometerTests::~TonometerTests() {


}


QVariantMap createFakeTestData(const QString& eye) {
    QVariantMap data;

    // Assuming integers for MeasureID and MeasureNumber
    data["PatientID"] = 12345678;
    data["MeasureID"] = 123;
    data["MeasureNumber"] = 456;

    data["MeasureDate"] = QDate::currentDate();
    data["SessionDate"] = QDate::currentDate();

    data["Eye"] = eye;
    data["ORASerialNumber"] = "ORA123456";
    data["ORASoftware"] = "v1.0.0";
    data["PCSoftware"] = "v2.0.0";
    data["Pressure"] = "Normal";
    data["Applanation"] = "TypeA";
    data["TimeIn"] = "";
    data["TimeOut"] = "";
    data["Meds"] = "MedicationX";
    data["Conditions"] = "ConditionY";
    data["Notes1"] = "Note1";
    data["Notes2"] = "Note2";
    data["Notes3"] = "Note3";
    data["Indexes"] = "Index1, Index2";

    data["IOPG"] = 15.2;
    data["IOPCC"] = 14.8;
    data["CRF"] = 10.5;
    data["CCTAvg"] = 550.0;
    data["CCTLowest"] = 540.0;
    data["CCTSD"] = 5.5;
    data["CH"] = 11.0;
    data["TearFilmValue"] = 0.5;

    data["m_G2"] = 3.5;
    data["b_G2"] = 4.5;
    data["m_G3"] = 5.5;
    data["b_G3"] = 6.5;
    data["iop_cc_coef"] = 7.5;
    data["crf_coef"] = 8.5;
    data["m_ABC"] = 9.5;
    data["b_ABC"] = 10.5;
    data["b_PP"] = 11.5;
    data["QualityIndex"] = 12.5;

    data["BestWeighted"] = true;

    return data;
}

bool deleteAllDataFromTables(QSqlDatabase& db) {
    QSqlQuery query(db);

    query.prepare("DELETE FROM Patients WHERE PatientID = ?");
    query.addBindValue(12345678);

    if (!query.exec()) {
        qWarning() << "Error deleting data from table:" << query.lastError().text();
        return false;
    }

    query.prepare("DELETE FROM Measures WHERE PatientID = ?");
    query.addBindValue(12345678);

    if (!query.exec()) {
        qWarning() << "Error deleting data from table:" << query.lastError().text();
        return false;
    }

    return true;
}

bool insertMeasure(QSqlDatabase& db, const QVariantMap& measureData) {
    QSqlQuery query(db);

    // Prepare the SQL INSERT statement with named placeholders
    query.prepare("INSERT INTO Measures ("
                  "PatientID, MeasureID, MeasureNumber, MeasureDate, SessionDate, Eye, "
                  "ORASerialNumber, ORASoftware, PCSoftware, IOPG, IOPCC, CRF, "
                  "CCTAvg, CCTLowest, CCTSD, CH, TearFilmValue, Pressure, Applanation, "
                  "TimeIn, TimeOut, Meds, Conditions, Notes1, Notes2, Notes3, "
                  "m_G2, b_G2, m_G3, b_G3, iop_cc_coef, crf_coef, m_ABC, b_ABC, b_PP, "
                  "BestWeighted, QualityIndex, Indexes) "
                  "VALUES (:PatientID, :MeasureID, :MeasureNumber, :MeasureDate, :SessionDate, :Eye, "
                  ":ORASerialNumber, :ORASoftware, :PCSoftware, :IOPG, :IOPCC, :CRF, "
                  ":CCTAvg, :CCTLowest, :CCTSD, :CH, :TearFilmValue, :Pressure, :Applanation, "
                  ":TimeIn, :TimeOut, :Meds, :Conditions, :Notes1, :Notes2, :Notes3, "
                  ":m_G2, :b_G2, :m_G3, :b_G3, :iop_cc_coef, :crf_coef, :m_ABC, :b_ABC, :b_PP, "
                  ":BestWeighted, :QualityIndex, :Indexes)");

    // Loop through the QVariantMap and bind values
    for (auto it = measureData.cbegin(); it != measureData.cend(); ++it) {
        query.bindValue(":" + it.key(), it.value());
    }

    // Execute the query
    for(int i = 0; i < query.boundValues().size(); ++i) {
        qDebug() << "Bound value" << i << ":" << query.boundValue(i);
    }

    if (!query.exec()) {
        qWarning() << "Error inserting data into the database:" << query.lastError().text();
        return false;
    }

    return true;
}

bool insertPatient(const QSqlDatabase& db, const QString& name, const QString& birthDate, const QString& sex, const int id) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO Patients (PatientID, Name, BirthDate, Sex, GroupID, ID, RaceID) VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(id);
    query.addBindValue(name);
    query.addBindValue(QDate::fromString(birthDate));
    query.addBindValue(sex.startsWith("m", Qt::CaseSensitivity::CaseInsensitive));
    query.addBindValue(2);
    query.addBindValue(id);
    query.addBindValue(1);

    if (!query.exec()) {
        qWarning() << "Database error:" << db.lastError().text();
        return false;
    }

    return true;
}


QVariantMap extractMeasures(const QSqlDatabase& db, const int patientId, const QString& eye) {
    QSqlQuery query(db);
    QVariantMap resultMap;

    query.prepare("SELECT * from Measures where PatientID = ? and Eye = ? ORDER BY MeasureDate desc");

    query.addBindValue(patientId);
    query.addBindValue(eye);

    if (!query.exec()) {
        qWarning() << "Database error:" << db.lastError().text();
    }

    while (query.next()) {
        for (int i = 0; i < query.record().count(); ++i) {
            resultMap.insert(query.record().fieldName(i), query.value(i));
        }
    }

    return resultMap;
}

void TonometerTests::test_case1() {
    TonometerTest test;

    QVariantMap leftEye = createFakeTestData("Left");
    QVariantMap rightEye = createFakeTestData("Right");

    // Set up the database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    QString mdbFilePath = "C:\\Users\\antho\\Documents\\Database11.mdb";
    db.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ=" + mdbFilePath);

    // Open the database
    if (!db.open()) {
        qWarning() << "Error: Unable to connect to database.";
        qWarning() << "Database error:" << db.lastError().text();
    }

    QVERIFY(deleteAllDataFromTables(db));
    QVERIFY(insertPatient(db, "John Doe", "1980-01-01", "M", 12345678));

    QVERIFY(insertMeasure(db, leftEye));
    QVERIFY(insertMeasure(db, rightEye));

    QVariantMap leftResults = extractMeasures(db, 12345678, "Left");
    QVariantMap rightResults = extractMeasures(db, 12345678, "Right");

    QVERIFY(!leftResults.isEmpty());
    QVERIFY(!rightResults.isEmpty());

    QList<QVariantMap> results { leftResults, rightResults };

    test.fromVariantMapList(results);

    QVERIFY(test.isValid());
    QVERIFY(deleteAllDataFromTables(db));

    // Print the tables and column names
    QStringList tables = db.tables(QSql::Tables);
    for (const QString &table : tables) {
        qDebug() << "Table:" << table;
        QSqlRecord record = db.record(table);
        for(int i = 0; i < record.count(); i++) {
            qDebug() << "    Column:" << record.fieldName(i);
        }
    }
}

QTEST_APPLESS_MAIN(TonometerTests)

#include "tst_tonometertests.moc"
