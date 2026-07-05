from session import Session, SessionDialog
from devices.crt.settings import DEVICE_NAME


class CRTSession(Session):
    pass


class CRTSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.setWindowTitle("CRT setup")

        self.setFixedWidth(350)
        self.setFixedHeight(350)
