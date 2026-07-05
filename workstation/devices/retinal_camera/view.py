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

        self.session_widget.deviceStatusValue.setText("OCT")
        self.table = self.measurement_table_widget.measurementTable

    @override
    def on_measured(self, output: dict):
        self.table.clear()

        columns = ["File", "Size"]
        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setRowCount(4)
        self.table.setColumnCount(len(columns))
        self.table.setHorizontalHeaderLabels(columns)

        for index, result in enumerate(output["results"]):
            file_item = QTableWidgetItem(result["file"])
            size_item = QTableWidgetItem(result["size"])

            self.table.setItem(index, 0, file_item)
            self.table.setItem(index, 1, size_item)

        super().on_measured(output)
