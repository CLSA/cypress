from PySide6.QtCore import Signal
from PySide6.QtSerialPort import QSerialPort, QSerialPortInfo

from controller import Controller

from devices.weigh_scale.config import WeighScaleConfig
from devices.weigh_scale.session import WeighScaleSession
from devices.weigh_scale.model import WeighScaleModel
from devices.weigh_scale.view import WeighScaleView


class WeighScaleController(Controller):
    devices_scanned = Signal()

    def __init__(
        self,
        session: WeighScaleSession,
        config: WeighScaleConfig,
        model: WeighScaleModel,
        view: WeighScaleView,
        detached: bool = False,
        parent=None,
    ):
        super().__init__(
            parent=parent,
            session=session,
            config=config,
            model=model,
            view=view,
            detached=detached,
        )

        self.device_list = []
        self.device_selected: QSerialPortInfo = None


    def _scan_device(self):
        self.device_list.clear()
        for port_info in QSerialPortInfo.availablePorts():
            pass
            #print(port_info.portName(), port_info.)

    def _device_selected(self, port_info: QSerialPortInfo):
        self.device_selected = port_info

    def _connect_device(self):
        pass

    def _disconnect_device(self):
        pass

    def _read_device(self):
        pass

    def _write_device(self):
        pass








