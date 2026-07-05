from PySide6 import QtWidgets

from .measurement_table_ui import Ui_MeasurementTable

class MeasurementTableWidget(QtWidgets.QWidget, Ui_MeasurementTable):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setupUi(self)