#ifndef CYPRESS_MAIN_WINDOW_H
#define CYPRESS_MAIN_WINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QCloseEvent>

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
};

#endif // CYPRESS_MAIN_WINDOW_H
