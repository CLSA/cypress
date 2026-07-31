from typing import Literal
from session import Session


class RetinalCameraSession(Session):
    side: Literal['L', 'R'] | None = None