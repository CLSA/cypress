from enum import Enum
from typing import Annotated

from PySide6.QtWidgets import (
    QDialog,
    QFormLayout,
    QLineEdit,
    QDialogButtonBox,
    QVBoxLayout,
    QSpinBox,
    QComboBox,
    QGroupBox,
    QDateEdit,
)

from PySide6.QtGui import QRegularExpressionValidator

from pydantic import BaseModel
from pydantic.types import StringConstraints, PositiveInt, PositiveFloat, PastDate


class SessionDialog(QDialog):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Cypress Session")

        # UI Setup
        self.layout = QVBoxLayout(self)

        self.session_group = QGroupBox()

        self.form_layout = QFormLayout()

        self.barcode = QLineEdit(self)
        self.barcode.setValidator(
            QRegularExpressionValidator(r"^[0-9]{8}$")
        )  # accept 8 numeric values
        self.barcode.setMaxLength(8)
        self.barcode.setPlaceholderText("50000000")
        self.barcode.textChanged.connect(self.can_submit)

        self.uid = QLineEdit(self)
        self.uid.setValidator(
            QRegularExpressionValidator(r"^[A-Za-z0-9]{8}$")
        )  # accept 8 numeric values
        self.uid.setMaxLength(8)
        self.uid.setPlaceholderText("A0000000")
        self.uid.textChanged.connect(self.can_submit)

        self.language = QComboBox(self)
        self.language.addItems({"en", "fr"})

        self.interviewer = QLineEdit(self)
        self.interviewer.setText("CLSA")
        self.interviewer.setValidator(
            QRegularExpressionValidator(r"^.+$")
        )  # accept 8 numeric values
        self.interviewer.textChanged.connect(self.can_submit)


        self.inputs = {
            "barcode": self.barcode,
            "uid": self.uid,
            "language": self.language,
            "interviewer": self.interviewer,
        }

        for label, widgets in self.inputs.items():
            self.form_layout.addRow(label.replace("_", " ").title(), widgets)

        self.session_group.setLayout(self.form_layout)
        self.session_group.setTitle("Session")

        self.layout.addWidget(self.session_group)

        # OK and Cancel buttons
        self.buttons = QDialogButtonBox(
            QDialogButtonBox.StandardButton.Ok | QDialogButtonBox.StandardButton.Cancel,
            self,
        )
        self.layout.addWidget(self.buttons)

        self.buttons.accepted.connect(self.accept)
        self.buttons.rejected.connect(self.reject)

        self.buttons.button(QDialogButtonBox.Ok).setEnabled(False)

    def get_values(self):
        return {
            "barcode": self.barcode.text(),
            "uid": self.uid.text(),
            "language": self.language.currentText(),
            "interviewer": self.interviewer.text(),
        }

    def can_submit(self):
        has_acceptable_input = all(
            [
                self.uid.hasAcceptableInput(),
                self.barcode.hasAcceptableInput(),
                self.interviewer.hasAcceptableInput(),
            ]
        )
        self.buttons.button(QDialogButtonBox.Ok).setEnabled(has_acceptable_input)

    def prompt(self, parent=None):
        dialog = type(self)(parent)
        result = dialog.exec()

        if result == QDialog.DialogCode.Accepted:
            return dialog.get_values()
        return None


class LanguageOptions(str, Enum):
    english = "en"
    french = "fr"


class Session(BaseModel):
    barcode: Annotated[str, StringConstraints(min_length=8, max_length=8)]
    uid: Annotated[str, StringConstraints(min_length=7, max_length=8)]
    language: LanguageOptions
    interviewer: Annotated[str, StringConstraints(min_length=1, max_length=50)]
    answer_id: PositiveInt


class TonometerSession(Session):
    dob: PastDate
    sex: SexEnum


class SpirometerSession(Session):
    dob: PastDate
    smoker: bool
    sex: SexEnum
    height: PositiveFloat
    weight: PositiveFloat


class SexEnum(Enum):
    MALE = "male"
    FEMALE = "female"
