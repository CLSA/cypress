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

    QSharedPointer<HipMeasurement> leftHipMeasurement;
    QSharedPointer<HipMeasurement> rightHipMeasurement;

    void getPatientScan(const QSqlDatabase &db, const QString &participantId);
};

#endif // DXAHIPTEST_H
