#ifndef USB_PORT_PICKER_H
#define USB_PORT_PICKER_H

#include <QtUsb/QtUsb>
#include <QtUsb/QHidDevice>

#include <QWidget>
#include <QMap>

namespace Ui {
class UsbPortPicker;
}

class UsbPortPicker : public QWidget
{
    Q_OBJECT

public:
    explicit UsbPortPicker(QWidget *parent = nullptr);
    ~UsbPortPicker();

signals:
    void deviceSelected(const QUsb::Id &info);

    void connectDevice();
    void disconnectDevice();

public slots:
    void scanningForDevices();
    void devicesDiscovered(const QMap<QString, QUsb::Id> &devices);

    void devicesCanBeSelected();
    void canConnectDevice();
    void canDisconnectDevice();

    void defaultDeviceSelected(const QUsb::Id& portInfo);

    void deviceConnected();
    void deviceDisconnected();

    void clear();

private slots:
    void comboBoxChanged(const QString& portName);

private:
    Ui::UsbPortPicker *ui;
    QMap<QString, QUsb::Id> m_usbPorts;
};

#endif // USB_PORT_PICKER_H
