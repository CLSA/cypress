from typing import override
from PySide6.QtWidgets import QTableWidgetItem, QHeaderView, QAbstractItemView

from view import View


from devices.grip_strength.config import GripStrengthConfig
from devices.grip_strength.session import GripStrengthSession


class GripStrengthView(View):
    def __init__(
        self,
        session: GripStrengthSession,
        config: GripStrengthConfig,
        detached: bool = False,
        parent=None,
    ):
        super().__init__(parent=parent, session=session, detached=detached, config=config)

        self.session_widget.deviceStatusValue.setText("Grip Strength")

        self.table = self.measurement_table_widget.measurementTable

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
