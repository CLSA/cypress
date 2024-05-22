#ifndef DXAHIPTEST_H
#define DXAHIPTEST_H

#include "server/sessions/dxa/dxa_hip_session.h"

#include "data/dxa/measurements/hip_measurement.h"
#include "data/test_base.h"

#include "dcmtk/ofstd/ofstdinc.h"

#include <QSqlDatabase>

class DxaHipTest: public TestBase
{
public:
    DxaHipTest();

    static const QMap<QString, QString> ranges;

    bool isValid() const override;
    void reset() override;

    virtual QJsonObject toJsonObject() const override;
    virtual QString toString() const override;

    int fromDicomFiles(QList<DicomFile> files, const DxaHipSession &session);

    bool hasAllNeededFiles() const;

    void getPatientScan(const QSqlDatabase &db, const QString &participantId);
    void getScanAnalysisData(const QSqlDatabase &patscanDb, const QSqlDatabase &referenceDb, const QJsonObject &patientData);
private:
    QSharedPointer<QJsonObject> leftHipScanAnalysis;
    QSharedPointer<QJsonObject> rightHipScanAnalysis;
};

#endif // DXAHIPTEST_H
