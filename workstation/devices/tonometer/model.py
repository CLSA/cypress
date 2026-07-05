from model import Model

from devices.tonometer.session import TonometerSession
from devices.tonometer.config import TonometerConfig


class TonometerModel(Model):
    def __init__(self, session: TonometerSession, config: TonometerConfig):
        super().__init__(session=session, config=config)
