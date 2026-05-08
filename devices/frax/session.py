from pydantic import computed_field
from pydantic.types import PositiveInt, PositiveFloat

from session import Session, SexEnum


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
