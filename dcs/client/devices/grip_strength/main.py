import sys

from session import SessionDialog
from device import Device

from devices.grip_strength.settings import LOG_CONFIG
from devices.grip_strength.session import GripStrengthSession
from devices.grip_strength.config import GripStrengthConfig
from devices.grip_strength.model import GripStrengthModel
from devices.grip_strength.view import GripStrengthView
from devices.grip_strength.controller import GripStrengthController


class GripStrengthSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.setWindowTitle("Grip strength setup")
        self.setMinimumSize(350, 350)


class GripStrength(Device):
    logging_config = LOG_CONFIG
    config = GripStrengthConfig
    model = GripStrengthModel
    view = GripStrengthView
    controller = GripStrengthController
    session = GripStrengthSession
    session_dialog = GripStrengthSessionDialog


if __name__ == "__main__":
    try:
        sys.exit(GripStrength.run(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)
