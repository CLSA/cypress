
from PySide6.QtWidgets import QDialog


class BloodPressureManualEntryDialog(QDialog):
    def __init__(self):
        super().__init__()
        #self.setupUi(self)

        self.values = {
            'systolic': None,
            'diastolic': None,
            'pulse': None
        }

    def set_data(self, values: dict):
        for key, value in values.items():
            self.values[key] = value

    def get_data(self):
        return self.values

    def _value_changed(self):
        widget = self.sender()
        name = widget.objectName()
        value = widget.value()

        self.values[name] = value
        print(name, value)