#include "submission_buttons.h"
#include "ui_submission_buttons.h"

SubmissionButtons::SubmissionButtons(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SubmissionButtons)
{
    ui->setupUi(this);
}

SubmissionButtons::~SubmissionButtons()
{
    delete ui;
}

void SubmissionButtons::enableFinishButton()
{
    ui->finishButton->setEnabled(true);
}

void SubmissionButtons::disableFinishButton()
{
    ui->finishButton->setEnabled(false);
}
