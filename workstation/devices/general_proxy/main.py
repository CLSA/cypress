import sys

from session import SessionDialog
from device import Device

from devices.general_proxy.settings import LOG_CONFIG
from devices.general_proxy.session import GeneralProxySession
from devices.general_proxy.config import GeneralProxyConfig
from devices.general_proxy.model import GeneralProxyModel
from devices.general_proxy.view import GeneralProxyView
from devices.general_proxy.controller import GeneralProxyController


class GeneralProxySessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.setWindowTitle("Weight setup")
        self.setMinimumSize(350, 350)


class GeneralProxy(Device):
    logging_config = LOG_CONFIG
    config = GeneralProxyConfig
    model = GeneralProxyModel
    view = GeneralProxyView
    controller = GeneralProxyController
    session = GeneralProxySession
    session_dialog = GeneralProxySessionDialog


if __name__ == "__main__":
    try:
        sys.exit(GeneralProxy.run(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)
