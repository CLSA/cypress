from pathlib import Path

from PySide6.QtWidgets import QWidget, QSizePolicy, QHBoxLayout, QLabel

from log import LogWidget

from ui.ui_echo_scans import Ui_ECHOScansWidget

class ECHOScansWidget(QWidget, Ui_ECHOScansWidget):
    def __init__(self, log_path, parent=None):
        super().__init__(parent=parent)

        self.setupUi(self)

        self.setSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Minimum)

        self.log_widget = LogWidget(self, log_path)
        self.tab_2.layout().insertWidget(0, self.log_widget)

    def set_totals(self, totals: dict):
        self.scans_received_value.setText(str(totals["total"]))
        self.usm_value.setText(str(totals["usm"]))
        self.us_value.setText(str(totals["us"]))
        self.src_value.setText(str(totals["src"]))