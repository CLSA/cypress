from devices.controller import Controller
from devices.tonometer.session import TonometerSession
from devices.tonometer.config import TonometerConfig
from devices.tonometer.model import TonometerModel

class TonometerController(Controller):
    def __init__(
        self,
        session: TonometerSession,
        config: TonometerConfig,
        model: TonometerModel,
        detached: bool = False,
        parent=None,
    ):
        super().__init__(
            session=session,
            config=config,
            model=model,
            detached=detached,
            parent=parent,
        )
