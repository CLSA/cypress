from typing import override

from PySide6.QtWidgets import QTableWidgetItem, QHeaderView, QAbstractItemView
from PySide6.QtCore import Qt

from devices.view import View
from devices.crt.session import CRTSession
from devices.crt.config import CRTConfig


class CRTView(View):
    def __init__(
        self,
        session: CRTSession,
        config: CRTConfig,
        detached: bool = False,
        parent=None,
    ):
        super().__init__(
            parent=parent, session=session, config=config, detached=detached
        )

        self.resize(800, 600)
        self.table = self.measurement_table_widget.measurementTable
        self.session_widget.deviceStatusValue.setText("Choice Reaction Test")

    @override
    def on_measured(self, output: dict):
        self.table.clear()

        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.table.horizontalHeader().setDefaultAlignment(Qt.AlignmentFlag.AlignLeft)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setRowCount(len(output["value"]["results"]))
        self.table.setColumnCount(5)
        self.table.setHorizontalHeaderLabels(
            [
                "ID",
                "Correct Position",
                "Response Correct",
                "Response Stimulus Interval",
                "Elapsed Time",
            ]
        )

        for index, result in enumerate(output["value"]["results"]):
            screen_id = QTableWidgetItem(result["screen_id"])
            correct_position = QTableWidgetItem(result["correct_position"].capitalize())
            response_correct = QTableWidgetItem(
                "Yes" if bool(result["response_correct"]) else "No"
            )
            response_stimulus_interval = QTableWidgetItem(
                f"{result['response_stimulus_interval']['value']}{result['response_stimulus_interval']['unit']}"
            )
            elapsed_time = QTableWidgetItem(
                f"{result['elapsed_time']['value']}{result['elapsed_time']['unit']}"
            )

            self.table.setItem(index, 0, screen_id)
            self.table.setItem(index, 1, correct_position)
            self.table.setItem(index, 2, response_correct)
            self.table.setItem(index, 3, response_stimulus_interval)
            self.table.setItem(index, 4, elapsed_time)

        super().on_measured(output)
