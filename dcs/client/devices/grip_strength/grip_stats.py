import json

from PySide6.QtWidgets import QWidget, QSizePolicy
from PySide6.QtCore import Signal

from ui.ui_grip_stats import Ui_GripInfo


class GripStatsWidget(QWidget, Ui_GripInfo):
    def __init__(self, parent=None):
        super().__init__()

        self.setupUi(self)
        self.setSizePolicy(
            QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Minimum
        )