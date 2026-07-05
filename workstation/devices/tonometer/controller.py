from controller import Controller

from devices.tonometer.session import TonometerSession
from devices.tonometer.config import TonometerConfig
from devices.tonometer.model import TonometerModel
from devices.tonometer.view import TonometerView

class TonometerController(Controller):
    def __init__(
        self,
        session: TonometerSession,
        config: TonometerConfig,
        model: TonometerModel,
        view: TonometerView,
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
