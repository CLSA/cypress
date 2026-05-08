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
