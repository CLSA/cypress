from typing import override
from view import View

from devices.tonometer.session import TonometerSession
from devices.tonometer.config import TonometerConfig


class TonometerView(View):
    def __init__(
        self,
        session: TonometerSession,
        config: TonometerConfig,
        detached: bool = False,
        parent=None,
    ):
        super().__init__(
            parent=parent, session=session, config=config, detached=detached
        )

        self.session_widget.deviceStatusValue.setText("ORA")
        self.measure_button.setVisible(True)

    @override
    def on_measured(self):
        super().on_measured()
