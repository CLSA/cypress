from model import Model

from devices.weigh_scale.session import WeighScaleSession
from devices.weigh_scale.config import WeighScaleConfig

class WeighScaleModel(Model):
    def __init__(self, session: WeighScaleSession, config: WeighScaleConfig):
        super().__init__(session=session, config=config)
