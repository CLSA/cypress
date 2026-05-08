from typing import override
from PySide6.QtWidgets import QTableWidgetItem, QHeaderView, QAbstractItemView

from devices.view import View

from devices.retinal_camera.controller import RetinalCameraController
from devices.retinal_camera.session import RetinalCameraSession


class RetinalCameraView(View):
    def __init__(
        self,
        controller: RetinalCameraController,
        session: RetinalCameraSession,
        parent=None,
    ):
        super().__init__(parent=parent, controller=controller, session=session)

        self.resize(800, 600)
        self.table = self.measurement_table_widget.measurementTable
        self.test_info_widget.deviceStatusValue.setText("OCT")


    @override
    def _on_measured(self, output: dict):
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

        super()._on_measured(output)

