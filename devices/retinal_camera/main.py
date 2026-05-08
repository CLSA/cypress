import sys

from PySide6.QtWidgets import (
    QApplication,
)

from session import SessionDialog

from instruments.retinal_camera.session import RetinalCameraSession
from instruments.retinal_camera.config import RetinalCameraConfig
from instruments.retinal_camera.model import RetinalCameraModel
from instruments.retinal_camera.view import RetinalCameraView
from instruments.retinal_camera.controller import RetinalCameraController

class RetinalCameraSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.setWindowTitle("Retinal camera setup")
        self.setMinimumSize(350, 350)


def run_retinal_camera(session: RetinalCameraSession | None):
    app = QApplication()

    config = RetinalCameraConfig.from_ini()

    standalone = not session
    if standalone:
        session = RetinalCameraSession(
            answer_id=1, **RetinalCameraSessionDialog().prompt()
        )

    controller = RetinalCameraController(
        config=config,
        session=session,
        model=RetinalCameraModel(session),
        standalone=standalone
    )

    view = RetinalCameraView(controller=controller, session=session)

    view.show()

    return app.exec()


if __name__ == '__main__':
    try:
        sys.exit(run_retinal_camera(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)

