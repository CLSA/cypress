from pathlib import Path

from PySide6.QtCore import Qt
from PySide6.QtWidgets import QTableWidgetItem, QHeaderView, QAbstractItemView

from view import View

from devices.ecg.session import ECGSession
from devices.ecg.config import ECGConfig


class ECGView(View):
    def __init__(
        self, session: ECGSession, config: ECGConfig, detached: bool = False, parent=None
    ):
        super().__init__(
            parent=parent, session=session, config=config, detached=detached
        )

        self.session_widget.deviceStatusValue.setText("ECG")
        self.table = self.measurement_table_widget.measurementTable
        self.table.setVisible(False)
        self.start_button.setVisible(False)
        self.setFixedSize(400, 250)

    def _on_files_received(self, files: list[Path]):
        self.table.clear()

        columns = ["Name", "Size"]

        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.table.horizontalHeader().setDefaultAlignment(Qt.AlignmentFlag.AlignLeft)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setRowCount(len(files))
        self.table.setColumnCount(len(columns))
        self.table.setHorizontalHeaderLabels(columns)

        for index, file_info in enumerate(files):
            name = QTableWidgetItem(file_info.name)
            size = QTableWidgetItem(file_info.readable_size)

            self.table.setItem(index, 0, name)
            self.table.setItem(index, 1, size)
