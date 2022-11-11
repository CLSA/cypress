#ifndef RETINALCAMERAMANAGER_H
#define RETINALCAMERAMANAGER_H

#include <QObject>
#include <QProcess>

#include "ManagerBase.h"


class RetinalCameraManager : public ManagerBase
{
    Q_OBJECT
public:
    explicit RetinalCameraManager(QObject *parent = nullptr);

    // load and save device, paths and other constant settings to .ini
    //
    void loadSettings(const QSettings &) override;
    void saveSettings(QSettings*) const override;

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
};

#endif // RETINALCAMERAMANAGER_H
