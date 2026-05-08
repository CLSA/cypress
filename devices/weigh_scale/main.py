import sys

from PySide6.QtWidgets import (
    QApplication,
)

from session import SessionDialog

from instruments.weigh_scale.session import WeighScaleSession
from instruments.weigh_scale.config import WeighScaleConfig
from instruments.weigh_scale.model import WeighScaleModel
from instruments.weigh_scale.view import WeighScaleView
from instruments.weigh_scale.controller import WeighScaleController


class WeighScaleSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.setWindowTitle("Weight setup")
        self.setMinimumSize(350, 350)


def run_weigh_scale(session: WeighScaleSession | None):
    app = QApplication()

    config = WeighScaleConfig.from_ini()

    standalone = not session
    if standalone:
        session = WeighScaleSession(answer_id=1, **WeighScaleSessionDialog().prompt())

    controller = WeighScaleController(
        config=config,
        session=session,
        model=WeighScaleModel(session),
        standalone=standalone,
    )

    view = WeighScaleView(controller=controller, session=session)

    view.show()

    return app.exec()


if __name__ == "__main__":
    try:
        sys.exit(run_weigh_scale(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)
