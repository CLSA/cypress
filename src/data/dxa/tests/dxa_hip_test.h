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

    void simulate() override;

    virtual QJsonObject toJsonObject() const override;
    virtual QString toString() const override;

    void fromDicomFiles(QList<DicomFile> files, const DxaHipSession &session);

    bool hasAllNeededFiles() const;

    QJsonObject extractScanAnalysis(const QSqlDatabase &db,
                                    const QString &barcode,
                                    const QString &scanType);

    QJsonObject extractScanData(const QSqlDatabase &db,
                                const QString &barcode,
                                const QString &scanType);

    QSharedPointer<HipMeasurement> leftHipMeasurement;
    QSharedPointer<HipMeasurement> rightHipMeasurement;

    void retrieveResults(const QSqlDatabase &db, const QString &barcode);

    void computeTandZScores();
};

#endif // DXAHIPTEST_H
