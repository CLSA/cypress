from typing import override

from PySide6.QtWidgets import QTableWidgetItem, QHeaderView, QAbstractItemView
from PySide6.QtCore import Qt

from view import View
from devices.frax.session import FRAXSession
from devices.frax.config import FRAXConfig


class FRAXView(View):
    def __init__(
        self,
        session: FRAXSession,
        config: FRAXConfig,
        detached: bool = False,
        parent=None
    ):
        super().__init__(
            parent=parent, session=session, config=config, detached=detached
        )

        self.session_widget.deviceStatusValue.setText("FRAX")
        self.table = self.measurement_table_widget.measurementTable

    @override
    def on_measured(self, output: dict):
        self.table.clear()

        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setRowCount(4)
        self.table.setColumnCount(2)
        self.table.setHorizontalHeaderLabels(["Type", "Probability"])

        for index, result in enumerate(output["value"]["results"]):
            risk = QTableWidgetItem(" ".join(result["type"].split("_")).capitalize())
            risk.setTextAlignment(
                Qt.AlignmentFlag.AlignVCenter | Qt.AlignmentFlag.AlignLeft
            )

            probability = QTableWidgetItem(
                f"{result['probability']['value']}{result['probability']['unit']}"
            )
            probability.setTextAlignment(
                Qt.AlignmentFlag.AlignVCenter | Qt.AlignmentFlag.AlignLeft
            )

            self.table.setItem(index, 0, risk)
            self.table.setItem(index, 1, probability)

        super().on_measured()
