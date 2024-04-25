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
    void setReadOnly(bool value);

signals:
    void portChanged(QString port);
    void aeTitleChanged(QString aeTitle);

private slots:
    void on_aeTitleValue_textChanged(const QString &arg1);

private:
    Ui::DicomWidget *ui;

    QString m_aeTitle;
    QString m_hostname;
    QString m_port;
};

#endif // DICOM_WIDGET_H
