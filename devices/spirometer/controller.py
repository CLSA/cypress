from devices.controller import Controller
from devices.spirometer.session import SpirometerSession
from devices.spirometer.config import SpirometerConfig
from devices.spirometer.model import SpirometerModel

class SpirometerController(Controller):
    def __init__(
        self,
        session: SpirometerSession,
        config: SpirometerConfig,
        model: SpirometerModel,
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
