#ifndef APEX_TRANSFER_WIDGET_H
#define APEX_TRANSFER_WIDGET_H

#include <QWidget>
#include <QFileInfo>

namespace Ui {
class ApexTransferWidget;
}

class ApexTransferWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ApexTransferWidget(QWidget *parent = nullptr);
    ~ApexTransferWidget();

public slots:
    void transfer();

signals:
    void started();
    void status(const QString newStatus);
    void completed(QFileInfo patientScan, QFileInfo reference);
    void failed();

private:
    Ui::ApexTransferWidget *ui;

    QString m_patscanDbPath;
    QFileInfo m_patscanDbFileInfo;

    QString m_referenceDbPath;
    QFileInfo m_referenceDbFileInfo;

    bool copyPatScanDb();
    bool copyReferenceDb();
};

#endif // APEX_TRANSFER_WIDGET_H
