#ifndef SPIROMETER_MANAGER_H
#define SPIROMETER_MANAGER_H

#include "data/spirometer/tests/spirometer_test.h"
#include "managers/manager_base.h"
#include "server/sessions/spirometer_session.h"

#include <QProcess>

class SpirometerManager : public ManagerBase
{
    enum FileType {
        EasyWareExe,
        EMRDataPath
    };
    Q_OBJECT

public:
    explicit SpirometerManager(QSharedPointer<SpirometerSession> session);
    ~SpirometerManager() = default;

    static bool isInstalled();

public slots:

    // what the manager does in response to the main application
    // window invoking its run method
    //
    bool start() override;

    // retrieve a measurement from the device
    //
    void measure() override;


    void readOutput();

private:
    QString m_runnableName; // full pathspec to EasyWarePro.exe
    QString m_runnablePath; // path to EasyWarePro.exe directory
    QString m_dataPath;     // Path to the EMR plugin data transfer directory
    QString m_inFileName;   // OnyxIn.xml
    QString m_outFileName;  // OnyxOut.xml

    QString getEMRInXmlName() const { return QString("%1/%2").arg(m_dataPath, m_inFileName); }
    QString getEMROutXmlName() const { return QString("%1/%2").arg(m_dataPath, m_outFileName); }
    QString getEWPDbName() const { return QString("%1/%2").arg(m_dataPath,"EasyWarePro.mdb"); }
    QString getEWPDbCopyName() const { return QString("%1/%2").arg(m_dataPath,"EasyWareProCopy.mdb"); }
    QString getEWPOptionsDbName() const { return QString("%1/%2").arg(m_dataPath,"EwpOptions.mdb"); }
    QString getEWPOptionsDbCopyName() const
    {
        return QString("%1/%2").arg(m_dataPath, "EwpOptionsCopy.mdb");
    }

    QSharedPointer<SpirometerTest> m_test;

    QProcess m_process;

    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;

    // create a copy of the two databases in the EMR transfer directory
    void backupDatabases() const;

    // restore databases from copies
    void restoreDatabases() const;
    void removeXmlFiles() const;
    void configureProcess();

    QString getOutputPdfPath() const;
    bool outputPdfExists() const;
};

#endif // SPIROMETER_MANAGER_H
