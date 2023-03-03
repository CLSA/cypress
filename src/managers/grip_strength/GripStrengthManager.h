#ifndef GRIPSTRENGTHMANAGER_H
#define GRIPSTRENGTHMANAGER_H

#include "managers/ManagerBase.h"
#include "data/grip_strength/tests/GripStrengthTest.h"

#include <QProcess>

class GripStrengthManager : public ManagerBase
{
    enum FileType {
        Tracker5Exe,
        GripTestDbPath,
        GripTestDataDbPath
    };
    Q_OBJECT
public:
    explicit GripStrengthManager();
    bool isAvailable();

public slots:
    void start() override;
    void measure() override;
    void finish() override;

signals:
    void processInitialized();
    void measureStart();

private:
    QProcess m_process;
    GripStrengthTest m_test;

    QString m_workingDir;
    QString m_runnableName;
    QString m_databaseDir;
    QString m_databaseName;
    QString m_databaseHost;
    QString m_databasePort;
    QString m_databaseUser;
    QString m_databasePassword;

    bool validateInputData(const QVariantMap& inputData);

    bool initializeConnections();
    bool configureProcess();
    bool processStart();
    bool readOutput();

    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;

    // set input parameters for the test
    void setInputData(const QVariantMap& inputData) override;

    bool isDefined(const QString&, const GripStrengthManager::FileType&) const;
};

#endif // GRIPSTRENGTHMANAGER_H
