from typing import override
from PySide6.QtWidgets import QTableWidgetItem, QHeaderView, QAbstractItemView

from view import View

from devices.retinal_camera.session import RetinalCameraSession
from devices.retinal_camera.config import RetinalCameraConfig


class RetinalCameraView(View):
    def __init__(
        self,
        session: RetinalCameraSession,
        config: RetinalCameraConfig,
        detached: bool = False,
        parent=None,
    ):
        super().__init__(
            parent=parent, session=session, config=config, detached=detached
        )


        self.table = self.measurement_table_widget.measurementTable

        self.session_widget.deviceStatusValue.setText(
            f"OCT {'(Left)' if self.session.side == "L" else '(Right)'}"
        )

        self.measure_button.setVisible(True)
        self.start_button.setVisible(True)

        self.columns = ["#", "Name", "Size"]

        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setColumnCount(len(self.columns))
        self.table.setHorizontalHeaderLabels(self.columns)

    @override
    def on_ready_to_measure(self):
        super().on_ready_to_measure()
        self.session_widget.statusValue.setText("Press measure once exported")

    @override
    def on_measured(self, output: dict):
        self.table.clear()

        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setRowCount(len(output["value"]["results"]))
        self.table.setColumnCount(len(self.columns))
        self.table.setHorizontalHeaderLabels(self.columns)

        for index, result in enumerate(output["value"]["results"]):
            sequence_item = QTableWidgetItem(str(result["sequence_number"]))
            file_item = QTableWidgetItem(result["name"])
            size_item = QTableWidgetItem(result["size"])

            self.table.setItem(index, 0, sequence_item)
            self.table.setItem(index, 1, file_item)
            self.table.setItem(index, 2, size_item)

        super().on_measured()
