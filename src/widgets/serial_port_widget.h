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
    void deviceSelected(const QSerialPortInfo& portInfo);

    void connectDevice();
    void disconnectDevice();

public slots:
    void scanningForDevices();
    void devicesDiscovered(const QMap<QString, QSerialPortInfo> &devices);

    void devicesCanBeSelected();
    void canConnectDevice();
    void canDisconnectDevice();

    void defaultDeviceSelected(const QSerialPortInfo& portInfo);

    void deviceConnected();
    void deviceDisconnected();



    void clear();

private slots:
    void comboBoxChanged(const QString& portName);

private:
    Ui::SerialPortWidget *ui;
    QSerialPortInfo m_portInfo;

    QMap<QString, QSerialPortInfo> m_serialPorts;
};

#endif // SERIAL_PORT_WIDGET_H
