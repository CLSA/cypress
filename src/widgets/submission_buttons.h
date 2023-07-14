#ifndef SUBMISSION_BUTTONS_H
#define SUBMISSION_BUTTONS_H

#include <QWidget>

namespace Ui {
class SubmissionButtons;
}

class SubmissionButtons : public QWidget
{
    Q_OBJECT

public:
    explicit SubmissionButtons(QWidget *parent = nullptr);
    ~SubmissionButtons();

    void enableFinishButton();
    void disableFinishButton();

private:
    Ui::SubmissionButtons *ui;
};

#endif // SUBMISSION_BUTTONS_H
