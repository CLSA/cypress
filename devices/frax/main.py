import sys

from devices.device import Device
from devices.frax.config import FRAXConfig
from devices.frax.session import FRAXSession, FRAXSessionDialog
from devices.frax.model import FRAXModel
from devices.frax.controller import FRAXController
from devices.frax.view import FRAXView


class FRAX(Device):
    config = FRAXConfig
    model = FRAXModel
    view = FRAXView
    controller = FRAXController
    session = FRAXSession
    session_dialog = FRAXSessionDialog


if __name__ == "__main__":
    try:
        sys.exit(FRAX.run(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)
