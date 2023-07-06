#ifndef DXA_MANAGER_H
#define DXA_MANAGER_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QString>

#include "dcmtk/dcmdata/dcfilefo.h"

#include "dicom/dicom_directory_watcher.h"
#include "dicom/dcm_recv.h"

#include "../manager_base.h"
#include "./dicom/dicom_scp.h"

#include "data/dxa/tests/ap_spine_test.h"
#include "data/dxa/tests/forearm_test.h"
#include "data/dxa/tests/hip_test.h"
#include "data/dxa/tests/iva_imaging_test.h"
#include "data/dxa/tests/whole_body_test.h"

/*
 * Static ivar needed for computing T- and Z-scores. Map distinct BMD variable name(s) (eg., HTOT_BMD) for a given
 * PatScanDb table (eg., Hip) and the corresponding bonerange code in the RefScanDb ReferenceCurve table (eg., 123.).
 * Additional BMD variables and codes should be added here for other tables (ie., Spine).
 */


class DXAManager : public ManagerBase
{
    Q_OBJECT
public:
    explicit DXAManager(const CypressSession& session);
    ~DXAManager();

    //DicomSCP* m_dicomSCP;
    static bool isAvailable();
    static bool isInstalled();

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
    //ApSpineTest m_apSpineTest;
    //ForearmTest m_forearmTest;
    //HipTest m_hipScanTest;
    //IVAImagingTest m_ivaImagingTest;
    //WholeBodyScanTest m_wholeBodyTest;

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

#endif // DXA_MANAGER_H
