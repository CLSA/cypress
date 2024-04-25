#include "managers/dxa/apex_database_helper.h"

#include <QDate>
#include <QDateTime>
#include <QtTest>
#include <QSqlDatabase>
#include <QSqlError>

// add necessary includes here

class dxa : public QObject
{
    Q_OBJECT

public:
    dxa();
    ~dxa();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();
};

dxa::dxa() {}

dxa::~dxa() {}

void dxa::initTestCase() {
    //QSqlDatabase patscanDb = QSqlDatabase::addDatabase("QODBC", "patscan");
    //QString patscanFilePath { "C:\\work\\clsa\\Test Data\\dxa\\PatScan.mdb" };

    //patscanDb.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ=" + patscanFilePath);
    //QVERIFY(patscanDb.open());
    //QVERIFY(!patscanDb.isOpenError());

    //QSqlDatabase referenceDb = QSqlDatabase::addDatabase("QODBC", "reference");
    //QString referenceFilePath { "C:\\work\\clsa\\Test Data\\dxa\\reference.mdb" };

    //referenceDb.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ=" + referenceFilePath);
    //QVERIFY(referenceDb.open());
    //QVERIFY(!referenceDb.isOpenError());
}

double computeYearsDifference(const QString& first, const QString& second)
{
    QDateTime firstDt = QDateTime::fromString(first, Qt::ISODateWithMs);
    QDateTime secondDt = QDateTime::fromString(second, Qt::ISODateWithMs);

    if (!firstDt.isValid()) {
        qWarning() << "first datetime is not valid";
        return 0.0;
    }

    if (!secondDt.isValid()) {
        qWarning() << "second datetime is not valid";
        return 9.9;
    }

    const qint64 diff = firstDt.toSecsSinceEpoch() - secondDt.toSecsSinceEpoch();
    const double yearsDiff = diff / (60.0 * 60.0 * 24.0 * 365.25);

    return yearsDiff;
}

void dxa::cleanupTestCase() {}

void dxa::test_case1() {
    qDebug() << computeYearsDifference("2024-02-29T11:15:49.000", "1934-06-16T00:00:00.000");
    //QSqlDatabase patscanDb = QSqlDatabase::database("patscan");

    //QString participantId { "20009109" };

    //QSharedPointer<QJsonObject> patientInfo = QSharedPointer<QJsonObject>::create();

    //ApexDatabaseHelper::patientScanQuery(patscanDb, participantId, *patientInfo);

    //QString patientKey = patientInfo->value("PATIENT_KEY").toString();
    //QDateTime birthdate = QDateTime::fromString(patientInfo->value("BIRTHDATE").toString(), Qt::DateFormat::ISODate);

    //int birthYear = birthdate.date().year();
    //int currentYear = QDate::currentDate().year();

    //qDebug() << patientKey << birthdate.date() << currentYear - birthYear;

    //QSharedPointer<QJsonObject> leftForearmOutput = QSharedPointer<QJsonObject>::create();
    //ApexDatabaseHelper::scanAnalysisQuery(patscanDb, patientKey, 6, *leftForearmOutput);
    //ApexDatabaseHelper::forearmQuery(patscanDb, patientKey, leftForearmOutput->value("SCANID").toString(), *leftForearmOutput);
    //qDebug() << "LEFT FA: " << *leftForearmOutput;

    //QSharedPointer<QJsonObject> rightForearmOutput = QSharedPointer<QJsonObject>::create();
    //ApexDatabaseHelper::scanAnalysisQuery(patscanDb, patientKey, 7, *rightForearmOutput);
    //ApexDatabaseHelper::forearmQuery(patscanDb, patientKey, rightForearmOutput->value("SCANID").toString(), *rightForearmOutput);
    //qDebug() << "RIGHT FA: " << *rightForearmOutput;

    ////ApexDatabaseHelper::forearmQuery(patscanDb, patientKey, forearmScanId, *forearmOutput);

    //QSharedPointer<QJsonObject> spineOutput = QSharedPointer<QJsonObject>::create();
    //ApexDatabaseHelper::scanAnalysisQuery(patscanDb, patientKey, 1, *spineOutput);
    //ApexDatabaseHelper::spineScanQuery(patscanDb, patientKey, spineOutput->value("SCANID").toString(), *spineOutput);
    //qDebug() << "SPINE: " << *spineOutput;

    //QSharedPointer<QJsonObject> leftHipOutput = QSharedPointer<QJsonObject>::create();
    //ApexDatabaseHelper::scanAnalysisQuery(patscanDb, patientKey, 2, *leftHipOutput);
    //ApexDatabaseHelper::hipScanQuery(patscanDb, patientKey, leftHipOutput->value("SCANID").toString(), *leftHipOutput);
    //ApexDatabaseHelper::hipHsaQuery(patscanDb, patientKey, leftHipOutput->value("SCANID").toString(), *leftHipOutput);
    //qDebug() << "Left HIP: " << *leftHipOutput;

    //QSharedPointer<QJsonObject> rightHipOutput = QSharedPointer<QJsonObject>::create();
    //ApexDatabaseHelper::scanAnalysisQuery(patscanDb, patientKey, 3, *rightHipOutput);
    //ApexDatabaseHelper::hipScanQuery(patscanDb, patientKey, rightHipOutput->value("SCANID").toString(), *rightHipOutput);
    //ApexDatabaseHelper::hipHsaQuery(patscanDb, patientKey, rightHipOutput->value("SCANID").toString(), *rightHipOutput);
    //qDebug() << "Right HIP: " << *rightHipOutput;

    //QSharedPointer<QJsonObject> wBodyOutput = QSharedPointer<QJsonObject>::create();
    //ApexDatabaseHelper::scanAnalysisQuery(patscanDb, patientKey, 5, *wBodyOutput);
    //ApexDatabaseHelper::wBodyQuery(patscanDb, patientKey, wBodyOutput->value("SCANID").toString(), *wBodyOutput);
    //ApexDatabaseHelper::wBodyCompositionQuery(patscanDb, patientKey, wBodyOutput->value("SCANID").toString(), *wBodyOutput);
    //ApexDatabaseHelper::wBodySubRegionBoneQuery(patscanDb, patientKey, wBodyOutput->value("SCANID").toString(), *wBodyOutput);
    //ApexDatabaseHelper::wBodySubRegionCompositionQuery(patscanDb, patientKey, wBodyOutput->value("SCANID").toString(), *wBodyOutput);
    //qDebug() << "WBODY: " << *wBodyOutput;
}

QTEST_APPLESS_MAIN(dxa)

#include "tst_dxa.moc"
