import sys

from PySide6.QtWidgets import QApplication

from session import SessionDialog

from devices.crt.config import CRTConfig
from devices.crt.session import CRTSession
from devices.crt.controller import CRTController
from devices.crt.view import CRTView
from devices.crt.model import CRTModel


class CRTSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.setWindowTitle("CRT")

        self.setFixedWidth(350)
        self.setFixedHeight(350)


def run_crt(session: CRTSession | None):
    app = QApplication()

    config = CRTConfig.from_ini()

    standalone = not session
    if standalone:
        session = CRTSession(answer_id=1, **CRTSessionDialog().prompt())

    model = CRTModel(session=session)
    controller = CRTController(config=config, session=session, model=model, standalone=standalone)
    view = CRTView(controller=controller, session=session)

    view.show()

    return app.exec()


if __name__ == "__main__":
    try:
        sys.exit(run_crt(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)
