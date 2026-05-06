from instruments.controller import Controller
from instruments.grip_strength.session import GripStrengthSession
from instruments.grip_strength.model import GripStrengthModel
from instruments.grip_strength.config import GripStrengthConfig


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
