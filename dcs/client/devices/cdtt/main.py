import sys

from device import Device
from devices.cdtt.config import CDTTConfig
from devices.cdtt.session import CDTTSession, CDTTSessionDialog
from devices.cdtt.model import CDTTModel
from devices.cdtt.controller import CDTTController
from devices.cdtt.view import CDTTView
from devices.cdtt.settings import LOG_CONFIG


class CDTT(Device):
    logging_config = LOG_CONFIG
    config = CDTTConfig
    model = CDTTModel
    view = CDTTView
    controller = CDTTController
    session = CDTTSession
    session_dialog = CDTTSessionDialog


if __name__ == "__main__":
    try:
        sys.exit(CDTT.run(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)
