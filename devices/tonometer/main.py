import sys

from PySide6.QtWidgets import (
    QApplication,
)

from session import SessionDialog

from devices.tonometer.session import TonometerSession
from devices.tonometer.config import TonometerConfig
from devices.tonometer.model import TonometerModel
from devices.tonometer.view import TonometerView
from devices.tonometer.controller import TonometerController


class TonometerSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.setWindowTitle("Tonometer setup")
        self.setMinimumSize(350, 350)


def run_tonometer(session: TonometerSession | None):
    app = QApplication()

    config = TonometerConfig.from_ini()

    detached = not session
    if detached:
        session = TonometerSession(answer_id=1, **TonometerSessionDialog().prompt())

    controller = TonometerController(
        config=config,
        session=session,
        model=TonometerModel(session),
        detached=detached,
    )

    view = TonometerView(controller=controller, session=session)

    view.show()

    return app.exec()


if __name__ == "__main__":
    try:
        sys.exit(run_tonometer(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)
