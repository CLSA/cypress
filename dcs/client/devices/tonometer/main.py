import sys

from PySide6.QtWidgets import (
    QApplication,
)

from session import SessionDialog
from device import Device

from devices.tonometer.settings import LOG_CONFIG
from devices.tonometer.session import TonometerSession
from devices.tonometer.config import TonometerConfig
from devices.tonometer.model import TonometerModel
from devices.tonometer.view import TonometerView
from devices.tonometer.controller import TonometerController


class TonometerSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.setWindowTitle("Tonometer setup")
        self.setMinimumSize(350, 350)


class Tonometer(Device):
    logging_config = LOG_CONFIG
    config = TonometerConfig
    model = TonometerModel
    view = TonometerView
    controller = TonometerController
    session = TonometerSession
    session_dialog = TonometerSessionDialog



if __name__ == "__main__":
    try:
        sys.exit(Tonometer.run(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)
