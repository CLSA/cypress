import sys

from devices.device import Device

from devices.audiometer.settings import LOG_CONFIG
from devices.audiometer.config import AudiometerConfig
from devices.audiometer.session import AudiometerSession, AudiometerSessionDialog
from devices.audiometer.model import AudiometerModel
from devices.audiometer.view import AudiometerView
from devices.audiometer.controller import AudiometerController


class Audiometer(Device):
    logging_config = LOG_CONFIG
    config = AudiometerConfig
    model = AudiometerModel
    view = AudiometerView
    controller = AudiometerController
    session = AudiometerSession
    session_dialog = AudiometerSessionDialog


if __name__ == "__main__":
    try:
        sys.exit(Audiometer.run(session=None))
    except Exception as e:
        print(e)
        print("Press enter to continue...")
        sys.exit(-1)
