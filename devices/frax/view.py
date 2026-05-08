from typing import override

from PySide6.QtWidgets import QTableWidgetItem, QHeaderView, QAbstractItemView
from PySide6.QtCore import Qt

from devices.view import View
from devices.frax.controller import FRAXController
from devices.frax.session import FRAXSession


class FRAXView(View):
    def __init__(
        self,
        controller: FRAXController,
        session: FRAXSession,
        parent=None
    ):
        super().__init__(
            parent=parent, controller=controller, session=session
        )

        self.resize(800, 600)
        self.table = self.measurement_table_widget.measurementTable

    @override
    def _on_measured(self, output: dict):
        self.table.clear()

        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setRowCount(4)
        self.table.setColumnCount(2)
        self.table.setHorizontalHeaderLabels(["Type", "Probability"])

        for index, result in enumerate(output["results"]):
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

        super()._on_measured(output)
