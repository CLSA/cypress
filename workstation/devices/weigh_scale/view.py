from typing import override
from view import View

from devices.weigh_scale.session import WeighScaleSession
from devices.weigh_scale.config import WeighScaleConfig


class WeighScaleView(View):
    def __init__(
        self,
        session: WeighScaleSession,
        config: WeighScaleConfig,
        detached: bool = False,
        parent=None,
    ):
        super().__init__(
            parent=parent, session=session, config=config, detached=detached
        )
        self.session_widget.deviceStatusValue.setText("Weight")

    @override
    def on_measured(self, output: dict):
        super().on_measured(output)
