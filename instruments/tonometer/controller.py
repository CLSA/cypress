from instruments.controller import Controller
from instruments.tonometer.session import TonometerSession
from instruments.tonometer.config import TonometerConfig
from instruments.tonometer.model import TonometerModel

class TonometerController(Controller):
    def __init__(
        self,
        session: TonometerSession,
        config: TonometerConfig,
        model: TonometerModel,
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
