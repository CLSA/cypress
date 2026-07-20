from PySide6.QtWidgets import (
    QFormLayout,
    QComboBox,
    QGroupBox,
    QDateEdit,
    QDoubleSpinBox,
    QCheckBox,
)
from PySide6.QtCore import QDate

from session import Session, SessionDialog, SexEnum

from pydantic.types import PastDate, PositiveFloat


class SpirometerSession(Session):
    dob: PastDate
    sex: SexEnum
    height: PositiveFloat
    weight: PositiveFloat
    smoker: bool


class SpirometerSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.setWindowTitle("Spirometer setup")

        self.setFixedWidth(350)
        self.setFixedHeight(450)

        self.audiometer_group = QGroupBox()
        self.audiometer_group.setTitle("Participant")

        self.dob = QDateEdit()
        self.dob.setMaximumDate(QDate.currentDate())
        self.dob.dateChanged.connect(self.can_submit)

        self.sex = QComboBox()
        self.sex.addItems({"male", "female"})

        # weight
        self.weight = QDoubleSpinBox()
        self.weight.setRange(1.0, 999.0)
        self.weight.setValue(70.0)
        self.weight.setSuffix(" kg")

        # height
        self.height = QDoubleSpinBox()
        self.height.setRange(1.0, 999.0)
        self.height.setValue(170.0)
        self.height.setSuffix(" cm")

        self.smoker = QCheckBox()

        self.form_layout = QFormLayout()
        self.form_layout.addRow("Sex", self.sex)
        self.form_layout.addRow("Date of Birth", self.dob)
        self.form_layout.addRow("Height", self.height)
        self.form_layout.addRow("Weight", self.weight)
        self.form_layout.addRow("Smoker", self.smoker)

        self.audiometer_group.setLayout(self.form_layout)

        self.layout.insertWidget(1, self.audiometer_group)

    def can_submit(self):
        has_acceptable_input = all([
            self.height.hasAcceptableInput(),
            self.weight.hasAcceptableInput(),
            self.dob.hasAcceptableInput()
        ])
        return super().can_submit() and has_acceptable_input

    def get_values(self):
        session_values = super().get_values()
        device_values = {
            "dob": self.dob.date().toPython(),
            "sex": self.sex.currentText(),
            "height": self.height.value(),
            "weight": self.weight.value(),
            "smoker": self.smoker.isChecked()
        }
        return session_values | device_values
