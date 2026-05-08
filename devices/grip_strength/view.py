from typing import override
from PySide6.QtWidgets import QTableWidgetItem, QHeaderView, QAbstractItemView

from devices.view import View

from devices.grip_strength.controller import GripStrengthController
from devices.grip_strength.session import GripStrengthSession


class GripStrengthView(View):
    def __init__(
        self,
        controller: GripStrengthController,
        session: GripStrengthSession,
        parent=None,
    ):
        super().__init__(parent=parent, controller=controller, session=session)

        self.resize(800, 600)
        self.table = self.measurement_table_widget.measurementTable
        self.test_info_widget.deviceStatusValue.setText("Grip strength")

    @override
    def _on_measured(self, output: dict):
        self.table.clear()

        columns = ["Rep 1", "Rep 2", "Rep 3"]

        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setRowCount(4)
        self.table.setColumnCount(len(columns))
        self.table.setHorizontalHeaderLabels(columns)

        for index, result in enumerate(output["results"]):
            rep1 = QTableWidgetItem(result["rep1"])
            rep2 = QTableWidgetItem(result["rep2"])
            rep3 = QTableWidgetItem(result["rep3"])

            self.table.setItem(index, 0, rep1)
            self.table.setItem(index, 1, rep2)
            self.table.setItem(index, 2, rep3)

        super()._on_measured(output)
