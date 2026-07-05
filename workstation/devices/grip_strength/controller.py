from controller import Controller

from devices.grip_strength.config import GripStrengthConfig
from devices.grip_strength.session import GripStrengthSession
from devices.grip_strength.model import GripStrengthModel
from devices.grip_strength.view import GripStrengthView


class GripStrengthController(Controller):
    def __init__(
        self,
        session: GripStrengthSession,
        config: GripStrengthConfig,
        model: GripStrengthModel,
        view: GripStrengthView,
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
