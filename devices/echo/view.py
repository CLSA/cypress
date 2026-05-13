from PySide6.QtCore import Qt
from PySide6.QtWidgets import QTableWidgetItem, QHeaderView, QAbstractItemView


from files.receiver import FileInfo

from devices.view import View

from devices.echo.controller import ECHOController
from devices.echo.session import ECHOSession


class ECHOView(View):
    def __init__(self, controller: ECHOController, session: ECHOSession, parent=None):
        super().__init__(parent=parent, controller=controller, session=session)

        self.resize(800, 600)
        self.table = self.measurement_table_widget.measurementTable
        self.controller.files_received.connect(self._on_files_received)

    def _on_files_received(self, files: list[FileInfo]):
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
