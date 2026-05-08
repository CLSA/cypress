from PySide6.QtCore import Qt
from PySide6.QtWidgets import QTableWidgetItem, QHeaderView, QAbstractItemView

from dicom.receiver import FileInfo
from instruments.view import View

from instruments.dxa.controller import DXAController
from instruments.dxa.session import DXASession


class DXAView(View):
    def __init__(
        self, controller: DXAController, session: DXASession, parent=None
    ):
        super().__init__(
            parent=parent, controller=controller, session=session
        )

        self.resize(800, 600)

        self.table = self.measurement_table_widget.measurementTable
        self.controller.files_received.connect(self._on_files_received)

    def _on_files_received(self, files: list[FileInfo]):
        self.table.clear()

        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.table.horizontalHeader().setDefaultAlignment(Qt.AlignmentFlag.AlignLeft)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setRowCount(len(files))
        self.table.setColumnCount(3)
        self.table.setHorizontalHeaderLabels(["Name", "Size", "Path"])

        for index, file_info in enumerate(files):
            name = QTableWidgetItem(file_info.name)
            size = QTableWidgetItem(file_info.size)
            file_path = QTableWidgetItem(str(file_info.file_path.resolve()))

            self.table.setItem(index, 0, name)
            self.table.setItem(index, 1, size)
            self.table.setItem(index, 2, file_path)
