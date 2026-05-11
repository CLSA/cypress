import sys

from devices.device import Device

from devices.cdtt.config import CDTTConfig
from devices.cdtt.session import CDTTSession, CDTTSessionDialog
from devices.cdtt.model import CDTTModel
from devices.cdtt.controller import CDTTController
from devices.cdtt.view import CDTTView



class CDTT(Device):
    config = CDTTConfig
    session = CDTTSession
    session_dialog = CDTTSessionDialog
    model = CDTTModel
    view = CDTTView
    controller = CDTTController



if __name__ == "__main__":
    try:
        sys.exit(CDTT.run(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)
