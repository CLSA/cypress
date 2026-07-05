from datetime import datetime, timezone

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
from PySide6.QtCore import QUuid

from pydantic import BaseModel, ConfigDict, computed_field, Field, HttpUrl, PrivateAttr
from pydantic.types import StringConstraints, PositiveInt, PositiveFloat, PastDate

from settings import CYPRESS_VERSION


class LanguageOptions(str, Enum):
    english = "en"
    french = "fr"


class SexEnum(Enum):
    MALE = "male"
    FEMALE = "female"


class Session(BaseModel):
    model_config = ConfigDict(use_enum_values=True)

    session_id: str = Field(
        default_factory=lambda: QUuid.createUuid().toString(
            QUuid.StringFormat.WithoutBraces
        ),
        frozen=True,
    )

    answer_id: Annotated[PositiveInt, Field(frozen=True)]
    barcode: Annotated[
        str, StringConstraints(min_length=1, max_length=99), Field(frozen=True)
    ]
    uid: Annotated[
        str, StringConstraints(min_length=1, max_length=99), Field(frozen=True)
    ]
    language: Annotated[LanguageOptions, Field(frozen=True)]
    interviewer: Annotated[
        str, StringConstraints(min_length=1, max_length=99), Field(frozen=True)
    ]
    start_time: str = Field(
        default_factory=lambda: str(
            datetime.now(timezone.utc)
            .isoformat(timespec="milliseconds")
            .replace("+00:00", "Z")
        ),
        frozen=True,
    )

    # optional
    interviewer_name: str | None = None
    date: str | None = None

    origin: str | None = None

    @computed_field
    @property
    def cypress_version(self) -> str:
        return CYPRESS_VERSION


class TonometerSession(Session):
    dob: PastDate
    sex: SexEnum


class SpirometerSession(Session):
    dob: PastDate
    smoker: bool
    sex: SexEnum
    height: PositiveFloat
    weight: PositiveFloat


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
        self.barcode.setPlaceholderText("00000000")
        self.barcode.textChanged.connect(self.can_submit)

        self.uid = QLineEdit(self)
        self.uid.setValidator(
            QRegularExpressionValidator(r"^[A-Za-z0-9]{8}$")
        )  # accept 8 numeric values
        self.uid.setMaxLength(8)
        self.uid.setPlaceholderText("00000000")
        self.uid.textChanged.connect(self.can_submit)

        self.language = QComboBox(self)
        self.language.addItems(["en", "fr"])
        self.language.setCurrentIndex(0)

        self.interviewer = QLineEdit(self)
        self.interviewer.setText("")
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
            if label == "uid":
                self.form_layout.addRow("Participant ID", widgets)
            elif label == "barcode":
                self.form_layout.addRow("Interview ID", widgets)
            else:
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
