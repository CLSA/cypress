#ifndef DICOM_WIDGET_H
#define DICOM_WIDGET_H

#include <QWidget>

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

private:
    Ui::DicomWidget *ui;
};

#endif // DICOM_WIDGET_H
