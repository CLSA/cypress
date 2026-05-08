import sys

from PySide6.QtWidgets import (
    QApplication,
)

from session import SessionDialog

from instruments.grip_strength.session import GripStrengthSession
from instruments.grip_strength.config import GripStrengthConfig
from instruments.grip_strength.model import GripStrengthModel
from instruments.grip_strength.view import GripStrengthView
from instruments.grip_strength.controller import GripStrengthController


class GripStrengthSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.setWindowTitle("Grip strength setup")
        self.setMinimumSize(350, 350)


def run_grip_strength(session: GripStrengthSession | None):
    app = QApplication()
    config = GripStrengthConfig.from_ini()
    standalone = not session

    if standalone:
        session = GripStrengthSession(
            answer_id=1, **GripStrengthSessionDialog().prompt()
        )

    controller = GripStrengthController(
        config=config,
        session=session,
        model=GripStrengthModel(session=session),
        standalone=standalone
    )

    view = GripStrengthView(controller=controller, session=session)
    view.show()

    return app.exec()


if __name__ == "__main__":
    try:
        sys.exit(run_grip_strength(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)

