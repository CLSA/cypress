from typing import override
from devices.view import View

from devices.spirometer.controller import SpirometerController
from devices.spirometer.session import SpirometerSession
from devices.spirometer.config import SpirometerConfig


class SpirometerView(View):
    def __init__(
        self, controller: SpirometerController, session: SpirometerSession, parent=None
    ):
        super().__init__(controller=controller, session=session, parent=parent)

        self.resize(800, 600)
        self.table = self.measurement_table_widget.measurementTable
        self.test_info_widget.deviceStatusValue.setText("Spirometer")

    @override
    def _on_measured(self, output: dict):
        self.table.clear()
