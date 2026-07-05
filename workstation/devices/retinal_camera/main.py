import sys

from session import SessionDialog
from device import Device

from devices.retinal_camera.settings import LOG_CONFIG
from devices.retinal_camera.session import RetinalCameraSession
from devices.retinal_camera.config import RetinalCameraConfig
from devices.retinal_camera.model import RetinalCameraModel
from devices.retinal_camera.view import RetinalCameraView
from devices.retinal_camera.controller import RetinalCameraController


class RetinalCameraSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.setWindowTitle("Retinal camera setup")
        self.setMinimumSize(350, 350)


class RetinalCamera(Device):
    logging_config = LOG_CONFIG
    config = RetinalCameraConfig
    model = RetinalCameraModel
    view = RetinalCameraView
    controller = RetinalCameraController
    session = RetinalCameraSession
    session_dialog = RetinalCameraSessionDialog


if __name__ == "__main__":
    try:
        sys.exit(RetinalCamera.run(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)
