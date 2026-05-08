from PySide6.QtCore import Qt
from PySide6.QtWidgets import QTableWidgetItem, QHeaderView, QAbstractItemView

from dicom.receiver import FileInfo

from instruments.view import View

from instruments.ecg.controller import ECGController
from instruments.ecg.session import ECGSession


class ECGView(View):
    def __init__(
        self, controller: ECGController, session: ECGSession, parent=None
    ):
        super().__init__(
            parent=parent, controller=controller, session=session
        )

        self.resize(800, 600)
        self.table = self.measurement_table_widget.measurementTable
        self.controller.files_received.connect(self._on_files_received)
        self.test_info_widget.startButton.setVisible(False)
        self.test_info_widget.statusValue.setText("Waiting for transfer")

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
            size = QTableWidgetItem(file_info.size)

            self.table.setItem(index, 0, name)
            self.table.setItem(index, 1, size)
