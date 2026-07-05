from controller import Controller

from devices.spirometer.session import SpirometerSession
from devices.spirometer.config import SpirometerConfig
from devices.spirometer.model import SpirometerModel
from devices.spirometer.view import SpirometerView

class SpirometerController(Controller):
    def __init__(
        self,
        session: SpirometerSession,
        config: SpirometerConfig,
        model: SpirometerModel,
        view: SpirometerView,
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
