import json

from PySide6.QtWidgets import QWidget, QSizePolicy
from PySide6.QtCore import Signal
from ui.ui_hearing_measurements import Ui_HearingMeasurements


class HearingMeasurementsWidget(QWidget, Ui_HearingMeasurements):
    values_changed = Signal(dict)

    def __init__(self, parent=None):
        super().__init__(parent)

        self.setupUi(self)

        self.inputs_enabled = False

        self.inputs = {
            "left": {
                "500Hz": self.Left500HzSpinBox,
                "1000Hz": self.Left1000HzSpinBox,
                "2000Hz": self.Left2000HzSpinBox,
                "3000Hz": self.Left3000HzSpinBox,
                "4000Hz": self.Left4000HzSpinBox,
                "6000Hz": self.Left6000HzSpinBox,
                "8000Hz": self.Left8000HzSpinBox,
            },
            "right": {
                "500Hz": self.Right500HzSpinBox,
                "1000Hz": self.Right1000HzSpinBox,
                "2000Hz": self.Right2000HzSpinBox,
                "3000Hz": self.Right3000HzSpinBox,
                "4000Hz": self.Right4000HzSpinBox,
                "6000Hz": self.Right6000HzSpinBox,
                "8000Hz": self.Right8000HzSpinBox,
            },
        }

        self.save_button = self.saveButton
        self.save_button.pressed.connect(self.on_save_manual_entry)

    def get_data(self):
        return {
            "left": {
                name: spin_box.value() for name, spin_box in self.inputs["left"].items()
            },
            "right": {
                name: spin_box.value()
                for name, spin_box in self.inputs["right"].items()
            },
        }

    def set_values(self, response_json):
        if self.inputs_enabled:
            self.set_enabled(False)

        for left_result in response_json["value"]["results"]["left"]:
            test = left_result["test"]
            test_key = "".join(test.split(" "))

            self.inputs["left"][test_key].setValue(left_result["level"]["value"])

        for right_result in response_json["value"]["results"]["right"]:
            test = right_result["test"]
            test_key = "".join(test.split(" "))

            self.inputs["right"][test_key].setValue(right_result["level"]["value"])

    def set_enabled(self, enabled: bool):
        self.inputs_enabled = enabled
        self.save_button.setEnabled(enabled)
        self.save_button.setVisible(enabled)

        self.enable_inputs("left", enabled)
        self.enable_inputs("right", enabled)

    def enable_inputs(self, side: str, enabled: bool):
        if side == "left":
            self.leftGroup.setEnabled(enabled)

        if side == "right":
            self.rightGroup.setEnabled(enabled)

        # for spin_box in self.inputs[side].values():
        #     spin_box.setEnabled(enabled)

    def on_save_manual_entry(self):
        self.values_changed.emit(self.get_data())
        self.set_enabled(False)
