#ifndef DIALOG_BASE_H
#define DIALOG_BASE_H

#include <QDialog>
#include <QStandardItemModel>

#include "../managers/manager_base.h"
#include "../auxiliary/Constants.h"

class CypressSession;

class DialogBase : public QDialog
{
    Q_OBJECT

public:
    DialogBase(QWidget *parent, QSharedPointer<CypressSession> session);
    virtual ~DialogBase();

    void initialize();
    void run();

    QSharedPointer<ManagerBase> m_manager;

public slots:
    void error(const QString& errorMsg);
    void success(const QString& successMsg);
    void cancel(const QString& cancelMsg);

protected:
    bool m_debug;
    bool m_sim;

    void closeEvent(QCloseEvent *event) override;

    QSharedPointer<CypressSession> m_session;

    bool isManualEntryMode { false };

    QVariantMap m_inputData;

    QDateTime startTime;

    virtual void initializeConnections();
    virtual void initializeModel();
};

#endif // DIALOG_BASE_H
