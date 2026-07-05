import sys

from PySide6.QtWidgets import (
    QApplication,
)

from session import SessionDialog
from device import Device

from devices.weigh_scale.settings import LOG_CONFIG
from devices.weigh_scale.session import WeighScaleSession
from devices.weigh_scale.config import WeighScaleConfig
from devices.weigh_scale.model import WeighScaleModel
from devices.weigh_scale.view import WeighScaleView
from devices.weigh_scale.controller import WeighScaleController


class WeighScaleSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.setWindowTitle("Weight setup")
        self.setMinimumSize(350, 350)


class WeighScale(Device):
    logging_config = LOG_CONFIG
    config = WeighScaleConfig
    model = WeighScaleModel
    view = WeighScaleView
    controller = WeighScaleController
    session = WeighScaleSession
    session_dialog = WeighScaleSessionDialog


if __name__ == "__main__":
    try:
        sys.exit(WeighScale.run(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)
