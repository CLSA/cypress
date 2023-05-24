#ifndef DXAMANAGER_H
#define DXAMANAGER_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QString>

#include "dcmtk/dcmdata/dcfilefo.h"

#include "dicom/dicomdirectorywatcher.h"
#include "dicom/dcmrecv.h"

#include "../ManagerBase.h"
#include "./dicom/DicomSCP.h"

#include "data/dxa/tests/ApSpineTest.h"
#include "data/dxa/tests/ForearmTest.h"
#include "data/dxa/tests/HipTest.h"
#include "data/dxa/tests/IVAImagingTest.h"
#include "data/dxa/tests/WholeBodyScanTest.h"

/*
 * Static ivar needed for computing T- and Z-scores. Map distinct BMD variable name(s) (eg., HTOT_BMD) for a given
 * PatScanDb table (eg., Hip) and the corresponding bonerange code in the RefScanDb ReferenceCurve table (eg., 123.).
 * Additional BMD variables and codes should be added here for other tables (ie., Spine).
 */


class DXAManager : public ManagerBase
{
    Q_OBJECT
public:
    explicit DXAManager(QJsonObject inputData);
    ~DXAManager();

    //DicomSCP* m_dicomSCP;
    bool isAvailable();

    QJsonObject scanAnalysisJson;
    QJsonObject scoresJson;

    QList<DcmFileFormat> validatedDicomFiles;
    QList<DcmFileFormat> getValidatedFiles(QStringList filePaths);

    QVariantMap getParticipantData();

    QMap<QString, QVariant> retrieveDeviceData();
    QMap<QString, QVariant> extractScanAnalysisData();
    QMap<QString, QVariant> computeTandZScores();

    void dicomFilesReceived(QStringList paths);
    bool validateDicomFile(DcmFileFormat &loadedFileFormat);
    bool startDicomServer();
    bool endDicomServer();

    bool isCorrectDicom(DcmFileFormat &file);
    bool isCompleteDicom(DcmFileFormat &file);

public slots:
    void start() override;
    void measure() override;
    void finish() override;

protected slots:
    void dicomServerExitNormal();
    void dicomServerExitCrash();

private:
    ApSpineTest m_apSpineTest;
    ForearmTest m_forearmTest;
    HipTest m_hipScanTest;
    IVAImagingTest m_ivaImagingTest;
    WholeBodyScanTest m_wholeBodyTest;

    //DicomDirectoryWatcher m_dicomWatcher;
    //DcmRecv m_dcmRecv;

    // set input parameters for the test
    void setInputData(const QVariantMap& inputData) override;

    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;
};

#endif // DXAMANAGER_H
