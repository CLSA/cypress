from typing import override

from PySide6.QtWidgets import QTableWidgetItem, QHeaderView, QAbstractItemView
from PySide6.QtCore import Qt

from devices.view import View

from devices.cdtt.controller import CDTTController
from devices.cdtt.session import CDTTSession


class CDTTView(View):
    def __init__(
        self,
        controller: CDTTController,
        session: CDTTSession,
        parent=None,
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
        self.table.horizontalHeader().setDefaultAlignment(Qt.AlignmentFlag.AlignLeft)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setRowCount(len(output["results"]))
        self.table.setColumnCount(2)
        self.table.setHorizontalHeaderLabels(
            [
                "Stimulus digits",
                "Response digits",
            ]
        )

        for index, result in enumerate(output["results"]):
            stimulus_digits = QTableWidgetItem(
                f"{','.join(str(digit) for digit in result['stimulus_digits'])}"
            )
            response_digits = QTableWidgetItem(
                f"{','.join(str(digit) for digit in result['response_digits'])}"
            )

            self.table.setItem(index, 0, stimulus_digits)
            self.table.setItem(index, 1, response_digits)

        super()._on_measured(output)
