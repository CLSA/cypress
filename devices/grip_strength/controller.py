from devices.controller import Controller
from devices.grip_strength.session import GripStrengthSession
from devices.grip_strength.model import GripStrengthModel
from devices.grip_strength.config import GripStrengthConfig


class GripStrengthController(Controller):
    def __init__(
        self,
        session: GripStrengthSession,
        config: GripStrengthConfig,
        model: GripStrengthModel,
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
