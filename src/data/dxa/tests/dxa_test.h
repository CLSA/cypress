#ifndef DXA_TEST_H
#define DXA_TEST_H

#include "data/test_base.h"

#include "dicom/dcm_recv.h"
#include "server/sessions/dxa/dxa_session.h"

#include "dcmtk/ofstd/ofstdinc.h"

#include <QSqlDatabase>

class DXATest : public TestBase
{

public:
    DXATest();

    static const QMap<QString, QString> ranges;

    bool isValid() const override;
    void reset() override;

    bool hasAllNeededFiles() const;

    int fromDicomFiles(QList<DicomFile> files, const DXASession &session);

    virtual QJsonObject toJsonObject() const override;
    virtual QString toString() const override;

    void getPatientScan(const QSqlDatabase &db, const QString &participantId);
    void getScanAnalysisData(const QSqlDatabase& patscanDb, const QSqlDatabase& referenceDb, const QJsonObject& patientData);
};

#endif // DXA_TEST_H
