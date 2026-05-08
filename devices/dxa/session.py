from pydantic.types import PositiveFloat, PastDate

from session import Session, SexEnum


class DXASession(Session):
    dob: PastDate
    height: PositiveFloat
    weight: PositiveFloat
    sex: SexEnum

    include_lhip: bool = False
    include_rhip: bool = False
    include_apspine: bool = False
    include_lfa: bool = False
    include_rfa: bool = False
    include_spine: bool = False
    include_whole_body: bool = False