from PySide6.QtWidgets import QWidget, QSizePolicy, QHBoxLayout, QLabel
from PySide6.QtCore import Qt

from pathlib import Path

from log import LogWidget

from ui.ui_dxa_scans import Ui_Form

from devices.dxa.session import DXASession




class DXAScansWidget(QWidget, Ui_Form):
    def __init__(self, log_path, session: DXASession, parent=None):
        super().__init__(parent=parent)

        self.session = session

        self.setupUi(self)
        self.setSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Minimum)

        self.log_widget = LogWidget(parent=self, log_path=log_path)

        self.tab_2.layout().insertWidget(0, self.log_widget)

    def update_total(self, files: list[Path]):
        self.scans_received_value.setText(str(len(files)))

    def update_status(self, status: dict):
        if "left_forearm" in status and self.session.include_lfa:
            self.left_forearm_value.setText(f"{str(status["left_forearm"])} / 1")

        if "right_forearm" in status and self.session.include_rfa:
            self.right_forearm_value.setText(f"{str(status["right_forearm"])} / 1")

        if "left_hip" in status and self.session.include_lhip:
            self.left_hip_value.setText(f"{str(status["left_hip"])} / 1")

        if "right_hip" in status and self.session.include_rhip:
            self.right_hip_value.setText(f"{str(status["right_hip"])} / 1")

        if "spine" in status and self.session.include_apspine:
            self.spine_value.setText(f"{str(status["spine"])} / 1")

        if "lateral_spine" in status and self.session.include_spine:
            self.lateral_spine_value.setText(f"{str(status["lateral_spine"])} / 3")

        if "whole_body" in status and self.session.include_wholebody:
            self.whole_body_value.setText(f"{str(status["whole_body"])} / 2")


    def set_includes(self):
        if not self.session.include_lhip:
            self.left_hip_value.setText("Not required")

        if not self.session.include_rhip:
            self.right_hip_value.setText("Not required")

        if not self.session.include_apspine:
            self.spine_value.setText("Not required")

        if not self.session.include_spine:
            self.lateral_spine_value.setText("Not required")

        if not self.session.include_lfa:
            self.left_forearm_value.setText("Not required")

        if not self.session.include_rfa:
            self.right_forearm_value.setText("Not required")

        if not self.session.include_wholebody:
            self.whole_body_value.setText("Not required")


