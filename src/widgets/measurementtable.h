#ifndef MEASUREMENTTABLE_H
#define MEASUREMENTTABLE_H

#include <QWidget>

namespace Ui {
class MeasurementTable;
}

class MeasurementTable : public QWidget
{
    Q_OBJECT

public:
    explicit MeasurementTable(QWidget *parent = nullptr);
    ~MeasurementTable();

private:
    Ui::MeasurementTable *ui;
};

#endif // MEASUREMENTTABLE_H
