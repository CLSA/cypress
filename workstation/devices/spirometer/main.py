import sys

from session import SessionDialog

from device import Device

from devices.spirometer.settings import LOG_CONFIG
from devices.spirometer.session import SpirometerSession
from devices.spirometer.config import SpirometerConfig
from devices.spirometer.model import SpirometerModel
from devices.spirometer.view import SpirometerView
from devices.spirometer.controller import SpirometerController


class SpirometerSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.setWindowTitle("Spirometer setup")
        self.setMinimumSize(350, 350)


class Spirometer(Device):
    logging_config = LOG_CONFIG
    config = SpirometerConfig
    model = SpirometerModel
    view = SpirometerView
    controller = SpirometerController
    session = SpirometerSession
    session_dialog = SpirometerSessionDialog


if __name__ == "__main__":
    try:
        sys.exit(Spirometer.run(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)
