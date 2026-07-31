import sys

from PySide6.QtWidgets import QComboBox, QGroupBox, QFormLayout

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
        self.setFixedSize(350, 350)

        self.camera_group = QGroupBox()
        self.camera_group.setTitle("Camera")

        self.side = QComboBox(self)
        self.side.addItems(["Left", "Right"])
        self.side.setCurrentIndex(0)

        self.form_layout = QFormLayout()
        self.form_layout.addRow("Eye", self.side)

        self.camera_group.setLayout(self.form_layout)

        self.layout.insertWidget(1, self.camera_group)

    def get_values(self):
        session_values = super().get_values()
        camera_values = {
            "side": self.side.currentText()[0],
        }

        return session_values | camera_values


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
