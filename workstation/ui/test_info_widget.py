
from PySide6 import QtWidgets

from .test_info_widget_ui import Ui_TestInfoWidget

class TestInfoWidget(QtWidgets.QWidget, Ui_TestInfoWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setupUi(self)