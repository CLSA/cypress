import sys

from session import SessionDialog
from device import Device

from devices.echo.settings import LOG_CONFIG
from devices.echo.config import ECHOConfig
from devices.echo.session import ECHOSession
from devices.echo.controller import ECHOController
from devices.echo.view import ECHOView
from devices.echo.model import ECHOModel


class ECHOSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.setWindowTitle("ECHO setup")
        self.setMinimumSize(350, 350)


class ECHO(Device):
    logging_config = LOG_CONFIG
    config = ECHOConfig
    model = ECHOModel
    view = ECHOView
    controller = ECHOController
    session = ECHOSession
    session_dialog = ECHOSessionDialog


if __name__ == "__main__":
    sys.exit(ECHO.run(session=None))
