#ifndef RETINALCAMERAMANAGER_H
#define RETINALCAMERAMANAGER_H

#include <QObject>
#include <QProcess>
#include <QSettings>

#include "../ManagerBase.h"


class RetinalCameraManager : public ManagerBase
{
    Q_OBJECT
public:
    explicit RetinalCameraManager(QObject *parent = nullptr);

    // load and save device, paths and other constant settings to .ini
    //
    void loadSettings(const QSettings &) ;
    void saveSettings(QSettings*) const ;

    // collate test results and device and other meta data
    // for the main application to write to .json
    //
    QJsonObject toJsonObject() const override;

    // build a model from test and measurement data for UI display
    // of the results
    //
    void initializeModel() override;

    // update the model whenever the test data changes
    //
    void updateModel() override;

    void setInputData(const QVariantMap &) override;

public slots:
    // subclasses call methods after main initialization just prior
    // to running (eg., emit dataChanged signal)
    //
    void start() override;

    // actual measure will only execute if the barcode has been
    // verified.  Subclasses must reimplement accordingly.
    //
    void measure() override;

    // subclasses call methods just prior to main close event
    //
    void finish() override;

protected:
    // Context dependent clear test data and possibly device data (eg., serial port info)
    void clearData() override;

private:
    QProcess m_process;

    QString m_runnableName;
    QString m_defaultPatientUid;

    QString m_database;
    QString m_databaseVersion;
    QString m_databaseDriver;
    QString m_databaseHost;
    QString m_databasePort;
    QString m_databaseUser;
    QString m_databasePassword;
};

#endif // RETINALCAMERAMANAGER_H
