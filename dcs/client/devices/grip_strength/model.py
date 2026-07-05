from model import Model

from devices.grip_strength.session import GripStrengthSession
from devices.grip_strength.config import GripStrengthConfig
from devices.grip_strength.paradox import ParadoxDb


class GripStrengthModel(Model):
    def __init__(self, session: GripStrengthSession, config: GripStrengthConfig):
        super().__init__(session=session, config=config)


    def is_valid(self) -> bool:
        return False

    def parse_database(self):
        database = ParadoxDb(self.config.database_path)