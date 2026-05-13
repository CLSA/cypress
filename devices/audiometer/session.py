from PySide6.QtWidgets import QFormLayout, QComboBox, QGroupBox, QDateEdit
from PySide6.QtCore import QDate

from session import Session, SessionDialog, SexEnum

from pydantic.types import PastDate


class AudiometerSession(Session):
    dob: PastDate
    sex: SexEnum


class AudiometerSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.setWindowTitle("Audiometer setup")

        self.setFixedWidth(350)
        self.setFixedHeight(350)

        self.audiometer_group = QGroupBox()
        self.audiometer_group.setTitle("Audiometer inputs")

        self.dob = QDateEdit()
        self.dob.setMaximumDate(QDate.currentDate())
        self.dob.dateChanged.connect(self.can_submit)

        self.sex = QComboBox()
        self.sex.addItems({"male", "female"})

        self.form_layout = QFormLayout()
        self.form_layout.addRow("Date of Birth", self.dob)
        self.form_layout.addRow("Sex", self.sex)

        self.audiometer_group.setLayout(self.form_layout)

        self.layout.insertWidget(1, self.audiometer_group)

    def can_submit(self):
        has_acceptable_input = self.dob.hasAcceptableInput()
        return super().can_submit() and has_acceptable_input

    def get_values(self):
        session_values = super().get_values()
        audiometer_values = {
            "dob": self.dob.date().toPython(),
            "sex": self.sex.currentText(),
        }
        return session_values | audiometer_values
