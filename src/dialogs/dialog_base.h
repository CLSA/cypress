#ifndef DIALOG_BASE_H
#define DIALOG_BASE_H

#include <QDialog>
#include <QStandardItemModel>

#include "managers/manager_base.h"
#include "auxiliary/Constants.h"

class CypressSession;

class DialogBase : public QDialog
{
    Q_OBJECT

public:
    DialogBase(QWidget *parent, const CypressSession& session);
    ~DialogBase();

    void initialize();
    void run();

    //virtual void userClose();

    void setRunMode(const Constants::RunMode& mode) { m_mode = mode; }
    Constants::RunMode runMode() { return m_mode; }

    QSharedPointer<ManagerBase> m_manager;

signals:
    void sendResults(QJsonObject& results);

public slots:
    void complete(QJsonObject& results);
    void error(const QString& errorMsg);
    void success(const QString& successMsg);

protected:
    void closeEvent(QCloseEvent *event) override;

    const CypressSession& m_session;

    bool m_user_close = false;

    QString m_uuid;
    QStandardItemModel m_model;
    Constants::RunMode m_mode;
    QVariantMap m_inputData;

    QString getDefaultOutputFileName();
    QString getOutputFile();

    virtual void initializeConnections();
    virtual void initializeModel();
};

#endif // DIALOG_BASE_H
