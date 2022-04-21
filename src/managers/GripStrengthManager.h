#ifndef GRIPSTRENGTHMANAGER_H
#define GRIPSTRENGTHMANAGER_H

#include "ManagerBase.h"
#include "../data/GripStrengthTest.h"

class GripStrengthManager : public ManagerBase
{
public:
    explicit GripStrengthManager(QObject* parent = Q_NULLPTR);

    void loadSettings(const QSettings&) override;
    void saveSettings(QSettings*) const override;

    QJsonObject toJsonObject() const override;

    void buildModel(QStandardItemModel*) const override;

    // Set the input data.
    // The input data is read from the input
    // json file to the main application.  This method should be
    // used to filter the minimum inputs needed to run
    // a test.  Filtering keys are stored in member
    // m_inputKeyList.
    //
    void setInputData(const QVariantMap&) override;

public slots:

    // what the manager does in response to the main application
    // window invoking its run method
    //
    void start() override;

    // retrieve a measurement from the device
    //
    void measure() override;

    // implementation of final clean up of device after disconnecting and all
    // data has been retrieved and processed by any upstream classes
    //
    void finish() override;

private:
    GripStrengthTest m_test;

    void clearData() override;
};

#endif // GRIPSTRENGTHMANAGER_H
