import sys

from PySide6.QtWidgets import (
    QApplication,
)

from session import SessionDialog

from devices.spirometer.session import SpirometerSession
from devices.spirometer.config import SpirometerConfig
from devices.spirometer.model import SpirometerModel
from devices.spirometer.view import SpirometerView
from devices.spirometer.controller import SpirometerController


class SpirometerSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.setWindowTitle("Spirometer setup")
        self.setMinimumSize(350, 350)


def run_spirometer(session: SpirometerSession | None):
    app = QApplication()

    config = SpirometerConfig.from_ini()

    detached = not session
    if detached:
        session = SpirometerSession(answer_id=1, **SpirometerSessionDialog().prompt())

    controller = SpirometerController(
        config=config,
        session=session,
        model=SpirometerModel(session),
        detached=detached,
    )

    view = SpirometerView(controller=controller, session=session)

    view.show()

    return app.exec()


if __name__ == "__main__":
    try:
        sys.exit(run_spirometer(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)
