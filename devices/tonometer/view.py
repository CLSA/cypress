from typing import override
from devices.view import View

from devices.tonometer.controller import TonometerController
from devices.tonometer.session import TonometerSession
from devices.tonometer.config import TonometerConfig


class TonometerView(View):
    def __init__(
        self, controller: TonometerController, session: TonometerSession, parent=None
    ):
        super().__init__(controller=controller, session=session, parent=parent)

        self.resize(800, 600)
        self.table = self.measurement_table_widget.measurementTable
        self.test_info_widget.deviceStatusValue.setText("Tonometer")

    @override
    def _on_measured(self, output: dict):
        self.table.clear()
