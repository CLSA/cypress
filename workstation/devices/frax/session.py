from PySide6.QtWidgets import (
    QFormLayout,
    QComboBox,
    QGroupBox,
    QSpinBox,
    QDoubleSpinBox,
    QCheckBox,
    QLineEdit,
)

from pydantic import computed_field
from pydantic.types import PositiveInt, PositiveFloat

from session import Session, SessionDialog, SexEnum


def calculate_bmi(weight_kg: float, height_cm: float):
    height_m = height_cm / 100.0
    return weight_kg / (height_m * height_m)


def calculate_t_score(bmd: float):
    """
    Calculate the Femoral Neck BMD t-score using NHANES III reference values for women age 20-29 years

    """
    return (bmd - 0.858) / 0.120


def calculate_glucocorticoid(
    age: int,
    gluco_number: int | None,
    gluco_year: int | None,
    gluco_age: int | None,
) -> int:
    """
    age: in years
    gluco_number: Months used
    gluco_year: Year last used
    gluco_age: Age last used
    """
    if age and gluco_age:
        if age - gluco_age <= 1 and gluco_number >= 3:
            return True

    if age and gluco_year:
        if age - gluco_year <= 1 and gluco_number >= 3:
            return True

    return False


class FRAXSession(Session):
    age: PositiveInt
    alcohol: bool
    current_smoker: bool
    sex: SexEnum

    father_hip_fracture: bool
    mother_hip_fracture: bool
    previous_fracture: bool

    weight: PositiveFloat
    height: PositiveFloat

    femoral_neck_bmd: PositiveFloat

    glucocorticoid_age: int | float | None
    glucocorticoid_number: int | float | None
    glucocorticoid_year: int | float | None

    ra_medications: str | None

    test_type: str = "t"
    country_code: str = "19"

    @computed_field
    @property
    def body_mass_index(self) -> float:
        return calculate_bmi(self.weight, self.height)

    @computed_field
    @property
    def femoral_neck_tscore(self) -> float:
        return calculate_t_score(self.femoral_neck_bmd)

    @computed_field
    @property
    def glucocorticoid(self) -> int:
        return calculate_glucocorticoid(
            age=self.age,
            gluco_number=self.glucocorticoid_number,
            gluco_year=self.glucocorticoid_year,
            gluco_age=self.glucocorticoid_age,
        )

    @computed_field
    @property
    def parent_hip_fracture(self) -> int:
        return self.mother_hip_fracture or self.father_hip_fracture

    @computed_field
    @property
    def rheumatoid_arthritis(self) -> int:
        return self.ra_medications.lower() != "none" and len(self.ra_medications) > 0


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
