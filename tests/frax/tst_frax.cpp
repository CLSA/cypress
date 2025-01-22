#include <QCoreApplication>
#include <QtTest>

#include "fixtures.cpp"

#include "auxiliary/json_settings.h"

#include "data/frax/tests/frax_test.h"
#include "server/sessions/frax_session.h"
#include "managers/frax/frax_manager.h"

// add necessary includes here

class Frax : public QObject
{
    Q_OBJECT

public:
    Frax();
    ~Frax();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_frax();

private:
    QList<QJsonObject> inputDataList;
};



Frax::Frax() {}

Frax::~Frax() {}

void Frax::initTestCase()
{
}

void Frax::cleanupTestCase()
{

}

void Frax::test_frax()
{
    QJsonObject inputData {
        // CypressSession
        { "origin", "localhost:9000"},
        { "uid", "A000000"},
        { "barcode", "5000000"},
        { "answer_id", 0 },
        { "language", "en" },
        { "interviewer", "anthony" },

        // FraxSession
        { "age", 86 },
        { "alcohol", false },
        { "current_smoker", false },
        { "father_hip_fracture", false },
        { "mother_hip_fracture", true },
        { "weight", 100.00 },
        { "height", 180.00 },
        { "femoral_neck_bmd", 1.00 },
        { "glucocorticoid_age", 4},
        { "glucocorticoid_number", 86 },
        { "glucocorticoid_year", 2024 },
        { "previous_fracture", true },
        { "ra_medications", "" },
        { "sex", "male" },
    };

    QSharedPointer<FraxSession> session(
        new FraxSession(nullptr, inputData, "localhost:9000")
    );

    try {
        session->calculateInputs();
    }
    catch (ValidationError e) {
        qDebug() << "error" << e.what();
    }
    catch (...) {
        qDebug() << "Unknown exception";
    }

    QSharedPointer<FraxManager> manager(new FraxManager(session));

    qDebug() << "expected:" << manager->m_test->getExpectedMeasurementCount();

    try {
        manager->start();
        qDebug() << manager->m_test->isValid();
        if (!manager->m_test->isValid()) {
            qDebug() << "results are not valid";
        }
        qDebug().noquote() << JsonSettings::prettyPrintJson(manager->m_test->toJsonObject());
    }
    catch (...) {
        qDebug() << "Exception happened";
    }
}


QTEST_MAIN(Frax)

#include "tst_frax.moc"
