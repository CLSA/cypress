from PySide6.QtSerialPort import QSerialPort, QSerialPortInfo
from instruments.controller import Controller

import configparser

config = configparser.ConfigParser()

config['weigh_scale'] = {
    'portName': 'COM1'
}

class WeighScaleController(Controller):

    def __init__(self, parent=None, session_data: dict = {}):
        super().__init__(parent, session_data)

        self.available_ports = self.scan_available_ports()
        self.port: QSerialPort | None = None

    def scan_available_ports(self):
        ports = []

        serial_port_infos = QSerialPortInfo.availablePorts()
        for port_info in serial_port_infos:
            ports.append({
                'port':             port_info.portName(),
                'location':         port_info.systemLocation(),
                'description':      port_info.description(),
                'manufacturer':     port_info.manufacturer(),
                'serial_number':    port_info.serialNumber(),
                'vendor_id':        port_info.vendorIdentifier(),
                'product_id':       port_info.productIdentifer(),
            })

        return ports

    def connect_port(self, port: dict):
        print("connect to port", port)
        if self.port and self.port.isOpen():
            self.port.close()

    def disconnect_port(self):
        if self.port and self.port.isOpen():
            self.port.close()
            self.port = None