import sys

from PySide6.QtWidgets import (
    QApplication,
    QFormLayout,
    QComboBox,
    QGroupBox,
    QDoubleSpinBox,
    QCheckBox,
    QDateEdit
)

from PySide6.QtCore import QDate

from session import SessionDialog
from devices.dxa.config import DXAConfig
from devices.dxa.session import DXASession
from devices.dxa.controller import DXAController
from devices.dxa.view import DXAView
from devices.dxa.model import DXAModel


class DXASessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.setWindowTitle("DXA Setup")
        self.setMinimumSize(350, 350)

        self.dxa_group = QGroupBox()
        self.dxa_group.setTitle("DXA")

        self.dob = QDateEdit()
        self.dob.setMaximumDate(QDate.currentDate())
        self.dob.dateChanged.connect(self.can_submit)

        self.sex = QComboBox()
        self.sex.addItems({'male', 'female'})

        # weight
        self.weight = QDoubleSpinBox()
        self.weight.setRange(1.0, 999.0)
        self.weight.setValue(70.0)
        self.weight.setSuffix(" kg")
        self.form_layout.addRow("Weight", self.weight)

        # height
        self.height = QDoubleSpinBox()
        self.height.setRange(1.0, 999.0)
        self.height.setValue(170.0)
        self.height.setSuffix(" cm")


        self.include_lhip = QCheckBox()
        self.include_rhip = QCheckBox()
        self.include_apspine = QCheckBox()
        self.include_lfa = QCheckBox()
        self.include_rfa = QCheckBox()
        self.include_spine = QCheckBox()
        self.include_whole_body = QCheckBox()


        self.form_layout = QFormLayout()
        self.form_layout.addRow("Date of Birth", self.dob)
        self.form_layout.addRow("Sex", self.sex)
        self.form_layout.addRow("Weight", self.weight)
        self.form_layout.addRow("Height", self.height)

        self.form_layout.addRow("Include left hip", self.include_lhip)
        self.form_layout.addRow("Include right hip", self.include_rhip)
        self.form_layout.addRow("Include AP spine", self.include_apspine)
        self.form_layout.addRow("Include left forearm", self.include_lfa)
        self.form_layout.addRow("Include right forearm", self.include_rfa)
        self.form_layout.addRow("Include spine", self.include_spine)
        self.form_layout.addRow("Include whole body", self.include_whole_body)

        self.dxa_group.setLayout(self.form_layout)
        self.layout.insertWidget(1, self.dxa_group)

    def can_submit(self):
        has_acceptable_input = all([
            self.dob.hasAcceptableInput(),
            self.height.hasAcceptableInput(),
            self.weight.hasAcceptableInput()
        ])
        return super().can_submit() and has_acceptable_input

    def get_values(self):
        session_values = super().get_values()
        dxa_values = {
            "dob": self.dob.date().toPython(),
            "sex": self.sex.currentText(),
            "weight": self.weight.value(),
            "height": self.weight.value(),

            "include_lhip": self.include_lhip.isChecked(),
            "include_rhip": self.include_rhip.isChecked(),
            "include_apspine": self.include_apspine.isChecked(),
            "include_lfa": self.include_lfa.isChecked(),
            "include_rfa": self.include_rfa.isChecked(),
            "include_spine": self.include_spine.isChecked(),
            "include_whole_body": self.include_whole_body.isChecked(),
        }
        return session_values | dxa_values

def run_dxa(session: DXASession | None):
    app = QApplication()

    config = DXAConfig.from_ini()

    standalone = not session
    if standalone:
        session = DXASession(answer_id=1, **DXASessionDialog().prompt())

    model = DXAModel(session=session)
    controller = DXAController(
        config=config, session=session, model=model, standalone=standalone
    )
    view = DXAView(controller=controller, session=session)

    view.show()

    return app.exec()

if __name__ == '__main__':
    try:
        sys.exit(run_dxa(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)