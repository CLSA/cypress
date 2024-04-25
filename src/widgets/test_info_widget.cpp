#include "test_info_widget.h"
#include "ui_test_info_widget.h"

TestInfoWidget::TestInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestInfoWidget) {
    ui->setupUi(this);
}

TestInfoWidget::~TestInfoWidget() {
    delete ui;
}

void TestInfoWidget::setSessionInformation(const CypressSession &session, const QString& deviceName) {
    ui->deviceStatusValue->setText(deviceName);
    ui->barcodeValue->setText(session.getBarcode());
    ui->interviewerValue->setText(session.getInterviewer());
    ui->startedValue->setText(session.getStartTime().toLocalTime().toString("hh:mm ap"));
}

void TestInfoWidget::setDeviceStatus(QString status) {
    ui->deviceStatusValue->setText(status);
}


void TestInfoWidget::setStatus(const QString status) {
    ui->statusValue->setText(status);
}
