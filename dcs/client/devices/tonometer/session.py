from PySide6.QtWidgets import QFormLayout, QComboBox, QGroupBox, QDateEdit
from PySide6.QtCore import QDate

from session import Session, SessionDialog, SexEnum

from pydantic.types import PastDate

class TonometerSession(Session):
    dob: PastDate
    sex: SexEnum

class TonometerSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.setWindowTitle("Tonometer setup")

        self.setFixedWidth(350)
        self.setFixedHeight(350)

        self.group = QGroupBox()
        self.group.setTitle("Participant")

        self.dob = QDateEdit()
        self.dob.setMaximumDate(QDate.currentDate())
        self.dob.dateChanged.connect(self.can_submit)

        self.sex = QComboBox()
        self.sex.addItems({"male", "female"})

        self.form_layout = QFormLayout()
        self.form_layout.addRow("Date of Birth", self.dob)
        self.form_layout.addRow("Sex", self.sex)

        self.group.setLayout(self.form_layout)

        self.layout.insertWidget(1, self.group)

    def can_submit(self):
        has_acceptable_input = self.dob.hasAcceptableInput()
        return super().can_submit() and has_acceptable_input

    def get_values(self):
        session_values = super().get_values()
        extra_values = {
            "dob": self.dob.date().toPython(),
            "sex": self.sex.currentText(),
        }
        return session_values | extra_values
