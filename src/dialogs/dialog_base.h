#ifndef DIALOG_BASE_H
#define DIALOG_BASE_H

#include "managers/manager_base.h"

#include <QDialog>
#include <QStandardItemModel>


class CypressSession;

class DialogBase : public QDialog
{
    Q_OBJECT

public:
    DialogBase(QWidget *parent, QSharedPointer<CypressSession> session);
    virtual ~DialogBase();

    bool run();

public slots:
    void error(const QString& errorMsg);
    void success(const QString& successMsg);
    void cancel(const QString& cancelMsg);

protected:
    bool isManualEntryMode{false};

    QSharedPointer<CypressSession> m_session;
    QSharedPointer<ManagerBase> m_manager;

    QVariantMap m_inputData;
    QDateTime startTime;

    void closeEvent(QCloseEvent *event) override;
};

#endif // DIALOG_BASE_H
