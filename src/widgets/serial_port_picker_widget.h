#ifndef SERIAL_PORT_PICKER_WIDGET_H
#define SERIAL_PORT_PICKER_WIDGET_H

#include <QWidget>
#include <QMap>
#include <QSerialPort>
#include <QSerialPortInfo>

namespace Ui {
class SerialPortPickerWidget;
}

class SerialPortPickerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SerialPortPickerWidget(QWidget *parent = nullptr);
    ~SerialPortPickerWidget();

    // select a device (serial port) by name
    // checks of the named port is in the list of scanned devices
    // and calls setDevice
    //
    const QSerialPortInfo& selectDevice(const QString& label);

    void setBaudRate(QSerialPort::BaudRate baudRate);
    void setParity(QSerialPort::Parity parity);
    void setDataBits(QSerialPort::DataBits dataBits);

signals:
    // a port was selected from the list of discovered ports
    //
    void deviceSelected(const QSerialPortInfo& info);

    void baudRateChanged(QSerialPort::BaudRate baudRate);
    void parityChanged(QSerialPort::Parity parity);
    void dataBitsChanged(QSerialPort::DataBits dataBits);

private:
    // Scan available serial ports
    void scanDevices();

    const QSerialPortInfo& setDevice(const QSerialPortInfo&);

    Ui::SerialPortPickerWidget *ui;

    QMap<QString, QSerialPortInfo> m_deviceList;

    QSerialPort m_serialPort;
    QString m_deviceName;
};

#endif // SERIAL_PORT_PICKER_WIDGET_H
