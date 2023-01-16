#ifndef DIALOGBASE_H
#define DIALOGBASE_H

#include <QDialog>
#include <QStandardItemModel>

#include "../managers/ManagerBase.h"
#include "../auxiliary/Constants.h"

class DialogBase : public QDialog
{
    Q_OBJECT

public:
    DialogBase(QWidget *parent = Q_NULLPTR);
    ~DialogBase() = default;

    void initialize();
    void run();

    void setRunMode(const Constants::RunMode& mode) { m_mode = mode; }
    Constants::RunMode runMode() { return m_mode; }

    QSharedPointer<ManagerBase> m_manager;

signals:
    void sendResults(QJsonObject& results);

public slots:
    void complete(QJsonObject& results);

protected:
    void closeEvent(QCloseEvent *event) override;

    QStandardItemModel m_model;
    QString m_inputFileName;
    QString m_outputFileName;
    Constants::RunMode m_mode;
    QVariantMap m_inputData;

    bool m_verbose;

    QString getDefaultOutputFileName();
    QString getOutputFile();

    virtual void initializeConnections() = 0;
    virtual void initializeModel();
};

#endif // DIALOGBASE_H
