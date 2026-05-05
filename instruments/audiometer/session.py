from session import Session, SexEnum

from pydantic.types import PastDate

class AudiometerSession(Session):
    dob: PastDate
    sex: SexEnum