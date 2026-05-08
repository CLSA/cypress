import sys

from PySide6.QtWidgets import QApplication, QFormLayout, QComboBox, QGroupBox, QDateEdit
from PySide6.QtCore import QDate

from session import SessionDialog

from devices.blood_pressure.config import BPConfig
from devices.blood_pressure.session import BPSession
from devices.blood_pressure.model import BPModel
from devices.blood_pressure.controller import BPController
from devices.blood_pressure.view import BPView


class BPSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.setWindowTitle("BP setup")
        self.setMinimumSize(350, 350)

        self.bp_group = QGroupBox()

        self.dob = QDateEdit()
        self.dob.setMaximumDate(QDate.currentDate())
        self.dob.dateChanged.connect(self.can_submit)

        self.sex = QComboBox()
        self.sex.addItems({"male", "female"})

        self.form_layout = QFormLayout()
        self.form_layout.addRow("Date of Birth", self.dob)
        self.form_layout.addRow("Sex", self.sex)

        self.bp_group.setLayout(self.form_layout)

        self.layout.insertWidget(1, self.bp_group)

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


def run_blood_pressure(session: BPSession | None):
    app = QApplication()

    config = BPConfig.from_ini()

    standalone = not session
    if standalone:
        session = BPSession(answer_id=1, **BPSessionDialog().prompt())

    model = BPModel(session=session)
    controller = BPController(
        config=config, session=session, model=model, standalone=standalone
    )
    view = BPView(controller=controller, session=session)
    view.show()

    return app.exec()


if __name__ == "__main__":
    try:
        sys.exit(run_blood_pressure(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)
