import sys

from PySide6.QtWidgets import (
    QApplication,
    QFormLayout,
    QComboBox,
    QGroupBox,
    QSpinBox,
    QDoubleSpinBox,
    QCheckBox,
    QLineEdit,
)

from session import SessionDialog

from devices.frax.config import FRAXConfig
from devices.frax.session import FRAXSession
from devices.frax.model import FRAXModel
from devices.frax.controller import FRAXController
from devices.frax.view import FRAXView


class FRAXSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.setWindowTitle("FRAX setup")

        self.setMinimumWidth(350)
        self.setMinimumHeight(700)

        self.frax_group = QGroupBox()
        self.frax_group.setTitle("FRAX inputs")

        self.form_layout = QFormLayout()

        # age
        self.age = QSpinBox()
        self.age.setRange(40, 90)
        self.form_layout.addRow("Age", self.age)

        # alcohol
        self.alcohol = QCheckBox()
        self.form_layout.addRow("Alcohol", self.alcohol)

        # current_smoker
        self.current_smoker = QCheckBox()
        self.form_layout.addRow("Current smoker", self.current_smoker)

        # sex
        self.sex = QComboBox()
        self.sex.addItems({"male", "female"})
        self.form_layout.addRow("Sex", self.sex)

        # father_hip_fracture
        self.father_hip_fracture = QCheckBox()
        self.form_layout.addRow("Father hip fracture", self.father_hip_fracture)

        # mother_hip_fracture
        self.mother_hip_fracture = QCheckBox()
        self.form_layout.addRow("Mother hip fracture", self.mother_hip_fracture)

        # previous_fracture
        self.previous_fracture = QCheckBox()
        self.form_layout.addRow("Previous fracture", self.previous_fracture)

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
        self.form_layout.addRow("Height", self.height)

        self.ra_medications = QLineEdit()
        self.form_layout.addRow("RA meds", self.ra_medications)

        # femoral_neck_bmd
        self.femoral_neck_bmd = QDoubleSpinBox()
        self.femoral_neck_bmd.setRange(0.0, 1.0)
        self.form_layout.addRow("Femoral neck BMD", self.femoral_neck_bmd)

        # glucocorticoid_age
        self.gluco_age = QSpinBox()
        self.gluco_age.setRange(0, 999)
        self.form_layout.addRow("Glucocorticoid age", self.gluco_age)

        # glucocorticoid_number
        self.gluco_number = QSpinBox()
        self.gluco_age.setRange(0, 999)
        self.form_layout.addRow("Glucocorticoid number", self.gluco_number)

        # glucocorticoid_year
        self.gluco_year = QSpinBox()
        self.gluco_age.setRange(0, 999)
        self.form_layout.addRow("Glucocorticoid year", self.gluco_year)

        self.frax_group.setLayout(self.form_layout)

        self.layout.insertWidget(1, self.frax_group)

    def can_submit(self):
        has_acceptable_input = all(
            [
                self.age.hasAcceptableInput(),
                self.weight.hasAcceptableInput(),
                self.height.hasAcceptableInput(),
                self.femoral_neck_bmd.hasAcceptableInput(),
                self.gluco_age.hasAcceptableInput(),
                self.gluco_year.hasAcceptableInput(),
                self.gluco_number.hasAcceptableInput(),
            ]
        )

        return super().can_submit() and has_acceptable_input

    def get_values(self):
        session_values = super().get_values()

        return session_values | {
            "age": self.age.value(),
            "alcohol": self.alcohol.isChecked(),
            "current_smoker": self.current_smoker.isChecked(),
            "sex": self.sex.currentText(),
            "father_hip_fracture": self.father_hip_fracture.isChecked(),
            "mother_hip_fracture": self.mother_hip_fracture.isChecked(),
            "previous_fracture": self.previous_fracture.isChecked(),
            "weight": self.weight.value(),
            "height": self.height.value(),
            "ra_medications": self.ra_medications.text(),
            "femoral_neck_bmd": self.femoral_neck_bmd.value(),
            "glucocorticoid_age": self.gluco_age.value(),
            "glucocorticoid_year": self.gluco_year.value(),
            "glucocorticoid_number": self.gluco_number.value(),
        }


def run_frax(session: FRAXSession | None):
    app = QApplication()

    config = FRAXConfig.from_ini()

    standalone = not session
    if standalone:
        session = FRAXSession(answer_id=1, **FRAXSessionDialog().prompt())

    model = FRAXModel(session=session)
    controller = FRAXController(
        config=config, session=session, model=model, standalone=standalone
    )
    view = FRAXView(controller=controller, session=session)
    view.show()

    return app.exec()


if __name__ == "__main__":
    try:
        sys.exit(run_frax(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)
