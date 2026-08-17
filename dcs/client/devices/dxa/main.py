import sys


from device import Device
from devices.dxa.settings import LOG_CONFIG
from devices.dxa.config import DXAConfig
from devices.dxa.session import DXASession, DXASessionDialog
from devices.dxa.controller import DXAController
from devices.dxa.view import DXAView
from devices.dxa.model import DXAModel


class DXA(Device):
    logging_config = LOG_CONFIG
    config = DXAConfig
    model = DXAModel
    view = DXAView
    controller = DXAController
    session = DXASession
    session_dialog = DXASessionDialog


if __name__ == "__main__":
    sys.exit(DXA.run(session=None))

