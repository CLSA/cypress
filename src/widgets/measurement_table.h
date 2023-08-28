#ifndef MEASUREMENT_TABLE_H
#define MEASUREMENT_TABLE_H

#include <QWidget>
#include <QTableWidget>

namespace Ui {
class MeasurementTable;
}

class MeasurementTable : public QWidget
{
    Q_OBJECT

public:
    explicit MeasurementTable(QWidget *parent = nullptr);
    ~MeasurementTable();

    QTableWidget* getTable();

    void enableMeasureButton();
    void disableMeasureButton();

private:
    Ui::MeasurementTable *ui;
};

#endif // MEASUREMENT_TABLE_H
