from session import Session, SexEnum

from pydantic.types import PastDate

class BPSession(Session):
    dob: PastDate
    sex: SexEnum