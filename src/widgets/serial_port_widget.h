#ifndef SERIAL_PORT_WIDGET_H
#define SERIAL_PORT_WIDGET_H

#include <QWidget>
#include <QSerialPortInfo>
#include <QMap>

namespace Ui {
class SerialPortWidget;
}

class SerialPortWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SerialPortWidget(QWidget *parent = nullptr);
    ~SerialPortWidget();

signals:
    void deviceSelected(const QString& portNumber);

public slots:
    void devicesDiscovered(QMap<QString, QSerialPortInfo> devices);
    void clear();

private slots:
    void comboBoxChanged(QString portNumber);

private:
    Ui::SerialPortWidget *ui;
};

#endif // SERIAL_PORT_WIDGET_H
