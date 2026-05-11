import sys

from PySide6.QtWidgets import (
    QApplication,
)

from session import SessionDialog

from devices.weigh_scale.session import WeighScaleSession
from devices.weigh_scale.config import WeighScaleConfig
from devices.weigh_scale.model import WeighScaleModel
from devices.weigh_scale.view import WeighScaleView
from devices.weigh_scale.controller import WeighScaleController


class WeighScaleSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.setWindowTitle("Weight setup")
        self.setMinimumSize(350, 350)


def run_weigh_scale(session: WeighScaleSession | None):
    app = QApplication()

    config = WeighScaleConfig.from_ini()

    detached = not session
    if detached:
        session = WeighScaleSession(answer_id=1, **WeighScaleSessionDialog().prompt())

    controller = WeighScaleController(
        config=config,
        session=session,
        model=WeighScaleModel(session),
        detached=detached,
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
