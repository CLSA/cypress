import sys

from device import Device
from devices.crt.config import CRTConfig
from devices.crt.session import CRTSession, CRTSessionDialog
from devices.crt.controller import CRTController
from devices.crt.view import CRTView
from devices.crt.model import CRTModel
from devices.crt.settings import LOG_CONFIG


class CRT(Device):
    logging_config = LOG_CONFIG
    config = CRTConfig
    model = CRTModel
    view = CRTView
    controller = CRTController
    session = CRTSession
    session_dialog = CRTSessionDialog


if __name__ == "__main__":
    try:
        sys.exit(CRT.run(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)
