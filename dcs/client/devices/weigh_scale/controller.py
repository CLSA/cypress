from controller import Controller

from devices.weigh_scale.config import WeighScaleConfig
from devices.weigh_scale.session import WeighScaleSession
from devices.weigh_scale.model import WeighScaleModel
from devices.weigh_scale.view import WeighScaleView


class WeighScaleController(Controller):
    def __init__(
        self,
        session: WeighScaleSession,
        config: WeighScaleConfig,
        model: WeighScaleModel,
        view: WeighScaleView,
        detached: bool = False,
        parent=None,
    ):
        super().__init__(
            parent=parent,
            session=session,
            config=config,
            model=model,
            view=view,
            detached=detached,
        )
