import sys
import traceback

from session import SessionDialog

from device import Device

from devices.spirometer.settings import LOG_CONFIG
from devices.spirometer.session import SpirometerSession, SpirometerSessionDialog
from devices.spirometer.config import SpirometerConfig
from devices.spirometer.model import SpirometerModel
from devices.spirometer.view import SpirometerView
from devices.spirometer.controller import SpirometerController

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
        print(traceback.format_exc())
        input("Press enter to continue...")
        sys.exit(-1)
