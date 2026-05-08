from devices.controller import Controller
from devices.weigh_scale.session import WeighScaleSession
from devices.weigh_scale.config import WeighScaleConfig
from devices.weigh_scale.model import WeighScaleModel

class WeighScaleController(Controller):
    def __init__(
        self,
        session: WeighScaleSession,
        config: WeighScaleConfig,
        model: WeighScaleModel,
        standalone: bool = False,
        parent=None,
    ):
        super().__init__(
            session=session,
            config=config,
            model=model,
            standalone=standalone,
            parent=parent,
        )
