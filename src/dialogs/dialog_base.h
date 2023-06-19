#ifndef DIALOG_BASE_H
#define DIALOG_BASE_H

#include <QDialog>
#include <QStandardItemModel>

#include "../managers/manager_base.h"
#include "../auxiliary/Constants.h"

class DialogBase : public QDialog
{
    Q_OBJECT

public:
    DialogBase(QWidget *parent = Q_NULLPTR, QString uuid = "");
    ~DialogBase() = default;

    void initialize();
    void run();

    virtual void userClose();

    void setRunMode(const Constants::RunMode& mode) { m_mode = mode; }
    Constants::RunMode runMode() { return m_mode; }

    QSharedPointer<ManagerBase> m_manager;

signals:
    void sendResults(QJsonObject& results);

public slots:
    void complete(QJsonObject& results);

protected:
    void closeEvent(QCloseEvent *event) override;

    bool m_user_close = false;

    QString m_uuid;
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

#endif // DIALOG_BASE_H
