#ifndef CYPRESS_MAIN_WINDOW_H
#define CYPRESS_MAIN_WINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QCloseEvent>

#include "dialogs/dialog_base.h"

namespace Ui {
class CypressMainWindow;
}

class CypressMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CypressMainWindow(QWidget *parent = nullptr);
    ~CypressMainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::CypressMainWindow *ui;
    QPushButton *m_button;

    QSharedPointer<DialogBase> m_device_dialog;
    QJsonObject getDebugInputData();

    void enableDevices(bool enabled);
    void enableDebugParticipant();
    void enableDebugDevices();
};

#endif // CYPRESS_MAIN_WINDOW_H
