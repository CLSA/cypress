import sys

from PySide6.QtWidgets import QApplication

from session import SessionDialog

from instruments.cdtt.config import CDTTConfig
from instruments.cdtt.session import CDTTSession
from instruments.cdtt.model import CDTTModel
from instruments.cdtt.controller import CDTTController
from instruments.cdtt.view import CDTTView


class CDTTSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.setWindowTitle("CDTT")

        self.setFixedWidth(350)
        self.setFixedHeight(350)


def run_cdtt(session: CDTTSession | None):
    app = QApplication()

    config = CDTTConfig.from_ini()

    standalone = not session
    if not standalone:
        session = CDTTSession(answer_id=1, **CDTTSessionDialog().prompt())

    model = CDTTModel(session=session)
    controller = CDTTController(
        config=config, session=session, model=model, standalone=standalone
    )
    view = CDTTView(controller=controller, session=session)
    view.show()

    return app.exec()


if __name__ == "__main__":
    try:
        sys.exit(run_cdtt(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)
