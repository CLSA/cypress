import sys

from PySide6.QtWidgets import QApplication

from session import SessionDialog

from instruments.crt.config import CRTConfig
from instruments.crt.session import CRTSession
from instruments.crt.controller import CRTController
from instruments.crt.view import CRTView
from instruments.crt.model import CRTModel


class CRTSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.setWindowTitle("CRT")

        self.setFixedWidth(350)
        self.setFixedHeight(350)


def run_crt(session: CRTSession | None):
    app = QApplication()

    try:
        config = CRTConfig.from_ini()
    except Exception as e:
        print(e)
        sys.exit(-1)

    standalone = not session
    if standalone:
        session = CRTSession(answer_id=1, **CRTSessionDialog().prompt())

    model = CRTModel(session=session)
    controller = CRTController(config=config, session=session, model=model, standalone=standalone)
    view = CRTView(controller=controller, session=session)

    view.show()

    sys.exit(app.exec())


if __name__ == "__main__":
    run_crt(session=None)
