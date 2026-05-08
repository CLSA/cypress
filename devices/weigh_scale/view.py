from typing import override
from devices.view import View

from devices.weigh_scale.controller import WeighScaleController
from devices.weigh_scale.session import WeighScaleSession
from devices.weigh_scale.config import WeighScaleConfig


class WeighScaleView(View):
    def __init__(
        self, controller: WeighScaleController, session: WeighScaleSession, parent=None
    ):
        super().__init__(controller=controller, session=session, parent=parent)

        self.resize(800, 600)
        self.table = self.measurement_table_widget.measurementTable
        self.test_info_widget.deviceStatusValue.setText("Weight")

    @override
    def _on_measured(self, output: dict):
        self.table.clear()
