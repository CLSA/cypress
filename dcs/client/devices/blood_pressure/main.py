import sys


from device import Device

from devices.blood_pressure.config import BPConfig
from devices.blood_pressure.session import BPSession, BPSessionDialog
from devices.blood_pressure.model import BPModel
from devices.blood_pressure.controller import BPController
from devices.blood_pressure.view import BPView
from devices.blood_pressure.settings import LOG_CONFIG


class BloodPressure(Device):
    logging_config = LOG_CONFIG
    config = BPConfig
    model = BPModel
    view = BPView
    controller = BPController
    session = BPSession
    session_dialog = BPSessionDialog


if __name__ == "__main__":
    try:
        sys.exit(BloodPressure.run(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)
