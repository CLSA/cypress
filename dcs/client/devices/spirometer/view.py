from typing import override
from view import View

from devices.spirometer.session import SpirometerSession
from devices.spirometer.config import SpirometerConfig


class SpirometerView(View):
    def __init__(
        self,
        session: SpirometerSession,
        config: SpirometerConfig,
        detached: bool = False,
        parent=None,
    ):
        super().__init__(
            parent=parent, session=session, config=config, detached=detached
        )

        self.session_widget.deviceStatusValue.setText("Spirometer")
        self.table = self.measurement_table_widget.measurementTable

    @override
    def _on_measured(self, output: dict):
        self.table.clear()
