import sys

from session import SessionDialog
from device import Device

from devices.ecg.settings import LOG_CONFIG
from devices.ecg.config import ECGConfig
from devices.ecg.session import ECGSession
from devices.ecg.controller import ECGController
from devices.ecg.view import ECGView
from devices.ecg.model import ECGModel


class ECGSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.setWindowTitle("ECG setup")
        self.setMinimumSize(350, 350)


class ECG(Device):
    logging_config = LOG_CONFIG
    config = ECGConfig
    model = ECGModel
    view = ECGView
    controller = ECGController
    session = ECGSession
    session_dialog = ECGSessionDialog


if __name__ == "__main__":
    try:
        sys.exit(ECG.run(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)
