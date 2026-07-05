from PySide6.QtCore import Qt
from PySide6.QtWidgets import QTableWidgetItem, QHeaderView, QAbstractItemView

from files.receiver import FileInfo
from view import View

from devices.dxa.session import DXASession
from devices.dxa.config import DXAConfig


class DXAView(View):
    def __init__(
        self,
        session: DXASession,
        config: DXAConfig,
        detached: bool = False,
        parent=None,
    ):
        super().__init__(
            parent=parent, session=session, config=config, detached=detached
        )

        self.session_widget.deviceStatusValue.setText("DEXA")
        self.table = self.measurement_table_widget.measurementTable


    def on_files_received(self, files: list[FileInfo]):
        self.table.clear()

        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.table.horizontalHeader().setDefaultAlignment(Qt.AlignmentFlag.AlignLeft)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setRowCount(len(files))
        self.table.setColumnCount(2)
        self.table.setHorizontalHeaderLabels(["Name", "Size"])

        for index, file_info in enumerate(files):
            name = QTableWidgetItem(file_info.name)
            size = QTableWidgetItem(file_info.readable_size)

            self.table.setItem(index, 0, name)
            self.table.setItem(index, 1, size)
