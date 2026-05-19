from PySide6.QtWidgets import QDialog

from devices.audiometer.ui_audiometer_manual_entry import Ui_AudiometerManualEntryDialog


class AudiometerManualEntryDialog(QDialog, Ui_AudiometerManualEntryDialog):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        self.values = {
            "Left500HzSpinBox": 0,
            "Left1000HzSpinBox": 0,
            "Left2000HzSpinBox": 0,
            "Left3000HzSpinBox": 0,
            "Left4000HzSpinBox": 0,
            "Left6000HzSpinBox": 0,
            "Left8000HzSpinBox": 0,
            "Right500HzSpinBox": 0,
            "Right1000HzSpinBox": 0,
            "Right2000HzSpinBox": 0,
            "Right3000HzSpinBox": 0,
            "Right4000HzSpinBox": 0,
            "Right6000HzSpinBox": 0,
            "Right8000HzSpinBox": 0,
        }

        self.Left500HzSpinBox.valueChanged.connect(self._value_changed)
        self.Left1000HzSpinBox.valueChanged.connect(self._value_changed)
        self.Left2000HzSpinBox.valueChanged.connect(self._value_changed)
        self.Left3000HzSpinBox.valueChanged.connect(self._value_changed)
        self.Left4000HzSpinBox.valueChanged.connect(self._value_changed)
        self.Left6000HzSpinBox.valueChanged.connect(self._value_changed)
        self.Left8000HzSpinBox.valueChanged.connect(self._value_changed)

        self.Right500HzSpinBox.valueChanged.connect(self._value_changed)
        self.Right1000HzSpinBox.valueChanged.connect(self._value_changed)
        self.Right2000HzSpinBox.valueChanged.connect(self._value_changed)
        self.Right3000HzSpinBox.valueChanged.connect(self._value_changed)
        self.Right4000HzSpinBox.valueChanged.connect(self._value_changed)
        self.Right6000HzSpinBox.valueChanged.connect(self._value_changed)
        self.Right8000HzSpinBox.valueChanged.connect(self._value_changed)

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
