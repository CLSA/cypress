from typing import override

from PySide6.QtCore import Qt
from PySide6.QtWidgets import QTableWidgetItem, QHeaderView, QAbstractItemView

from view import View

from devices.tonometer.session import TonometerSession
from devices.tonometer.config import TonometerConfig


class TonometerView(View):
    def __init__(
        self,
        session: TonometerSession,
        config: TonometerConfig,
        detached: bool = False,
        parent=None,
    ):
        super().__init__(
            parent=parent, session=session, config=config, detached=detached
        )

        self.table = self.measurement_table_widget.measurementTable

        self.session_widget.deviceStatusValue.setText("ORA")
        self.measure_button.setVisible(True)

    @override
    def _on_start_button_clicked(self):
        self.logger.info("start requested")
        self.start.emit()

    @override
    def on_measured(self, output: dict):
        self.table.clear()

        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.table.horizontalHeader().setDefaultAlignment(Qt.AlignmentFlag.AlignLeft)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setRowCount(len(output["value"]["results"]))
        self.table.setColumnCount(7)
        self.table.setHorizontalHeaderLabels(
            [
                "Measure ID",
                "Eye",
                "IOPcc",
                "IOPg",
                "CH",
                "CRF",
                "WS",
            ]
        )

        for index, result in enumerate(output["value"]["results"]):
            measure_id = QTableWidgetItem(str(result.get("measure_id", "---")))

            eye = QTableWidgetItem(str(result.get("eye", "---")))

            iopcc = QTableWidgetItem(
                str(round(result["iopcc"]["value"], 1))
                if result.get("iopcc", None) is not None
                else "---"
            )

            iopg = QTableWidgetItem(
                str(round(result["iopg"]["value"], 1))
                if result.get("iopg", None) is not None
                else "---"
            )
            ch = QTableWidgetItem(
                str(round(result["ch"], 1))
                if result.get("ch", None) is not None
                else "---"
            )
            crf = QTableWidgetItem(
                str(round(result["crf"]["value"], 1))
                if result.get("crf", None) is not None
                else "---"
            )
            ws = QTableWidgetItem(
                str(round(result["sa_ws"], 1))
                if result.get("sa_ws", None) is not None
                else "---"
            )

            self.table.setItem(index, 0, measure_id)
            self.table.setItem(index, 1, eye)
            self.table.setItem(index, 2, iopcc)
            self.table.setItem(index, 3, iopg)
            self.table.setItem(index, 4, ch)
            self.table.setItem(index, 5, crf)
            self.table.setItem(index, 6, ws)

        super().on_measured()
