#ifndef BLOODPRESSUREMANAGER_H
#define BLOODPRESSUREMANAGER_H

#include "ManagerBase.h"
//#include "../data/BloodPressureTest.h"

class BloodPressureManager : public ManagerBase
{
public:
    explicit BloodPressureManager(QObject* parent = Q_NULLPTR);

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
        void setInputData(const QMap<QString, QVariant>&);

public slots:

    // retrieve a measurement from the device
    //
    void measure() override;

    // implementation of final clean up of device after disconnecting and all
    // data has been retrieved and processed by any upstream classes
    //
    void finish() override;

    void readOutput();

    void start() override;

private:
    QString m_outputPath;     // path to output .csv files
    QString m_outputFile;     // full pathspec to working output .csv file

    //BloodPressureTest m_test;

    void clearData() override;

    //QMap<QString, QVariant> m_inputData;
    QList<QString> m_inputKeyList;
};

#endif // BLOODPRESSUREMANAGER_H