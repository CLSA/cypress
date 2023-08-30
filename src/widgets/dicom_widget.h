#ifndef DICOM_WIDGET_H
#define DICOM_WIDGET_H

#include <QWidget>
#include <QMap>

namespace Ui {
class DicomWidget;
}

class DicomWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DicomWidget(QWidget *parent = nullptr);
    ~DicomWidget();

    void setDicomLabels(const QString& aeTitle, const QString& hostname, const QString& port);

signals:
    void updateDicomSettings(const QString& newAeTitle, const QString& newPortValue);

private slots:
    void onSaveButtonClicked();
    void on_aeTitleValue_textChanged(const QString &arg1);
    void on_portValue_textChanged(const QString &arg1);

private:
    Ui::DicomWidget *ui;

    QString m_aeTitle;
    QString m_hostname;
    QString m_port;
};

#endif // DICOM_WIDGET_H
