from typing import override

from PySide6.QtWidgets import QTableWidgetItem, QHeaderView, QAbstractItemView
from PySide6.QtCore import Qt

from view import View
from devices.cdtt.session import CDTTSession
from devices.cdtt.config import CDTTConfig


class CDTTView(View):
    def __init__(
        self,
        config: CDTTConfig,
        session: CDTTSession,
        detached: bool = False,
        parent=None,
    ):
        super().__init__(
            parent=parent, session=session, config=config, detached=detached
        )

        self.session_widget.deviceStatusValue.setText("CDTT")

        self.table = self.measurement_table_widget.measurementTable
        self.measure_button.setVisible(True)

    @override
    def on_measured(self, output: dict):
        self.table.clear()

        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.table.horizontalHeader().setDefaultAlignment(Qt.AlignmentFlag.AlignLeft)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setRowCount(len(output["value"]["results"]))
        self.table.setColumnCount(2)
        self.table.setHorizontalHeaderLabels(
            [
                "Stimulus digits",
                "Response digits",
            ]
        )

        for index, result in enumerate(output["value"]["results"]):
            stimulus_digits = QTableWidgetItem(
                f"{','.join(str(digit) for digit in result['stimulus_digits'])}"
            )
            response_digits = QTableWidgetItem(
                f"{','.join(str(digit) for digit in result['response_digits'])}"
            )

            self.table.setItem(index, 0, stimulus_digits)
            self.table.setItem(index, 1, response_digits)

        super().on_measured()
