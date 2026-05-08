from PySide6.QtWidgets import QTableWidgetItem, QHeaderView, QAbstractItemView, QDialog
from PySide6.QtCore import Qt

from devices.view import View

from typing import override

from .controller import AudiometerController
from .session import AudiometerSession

from .manual_entry import AudiometerManualEntryDialog


class AudiometerView(View):
    def __init__(
        self,
        controller: AudiometerController,
        session: AudiometerSession,
        parent=None
    ):
        super().__init__(
            controller=controller, session=session, parent=parent
        )

        self.measurement_table_widget.manualEntryToggle.setEnabled(True)
        self.measurement_table_widget.manualEntryToggle.setVisible(True)

        self.table = self.measurement_table_widget.measurementTable

        self.resize(800, 600)

        self.manual_entry_form = AudiometerManualEntryDialog()

    @override
    def _on_manual_entry_clicked(self):
        result = self.manual_entry_form.exec()

        if result == QDialog.Accepted:
            data = self.manual_entry_form.get_data()
            self.controller.manual_entry(data)
        else:
            print("user cancelled the dialog.")


    @override
    def _on_measured(self, output: dict):
        # return super()._on_measured(results)
        self.results = output

        self.table.clear()

        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.table.horizontalHeader().setDefaultAlignment(Qt.AlignmentFlag.AlignLeft)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setRowCount(len(output["results"]))
        self.table.setColumnCount(4)
        self.table.setHorizontalHeaderLabels(
            [
                "Side",
                "Test",
                "Level",
                "Passed",
                "Error",
            ]
        )

        for index, result in enumerate(output["results"]):
            side = QTableWidgetItem(result["side"].capitalize())
            test = QTableWidgetItem(result["test"])
            level = QTableWidgetItem(f'{result['level']['value']} {result['level']['units']}')
            outcome = QTableWidgetItem('Yes' if result['pass'] else 'No')
            error = QTableWidgetItem(result['error'])

            self.table.setItem(index, 0, side)
            self.table.setItem(index, 1, test)
            self.table.setItem(index, 2, level)
            self.table.setItem(index, 3, outcome)
            self.table.setItem(index, 4, error)

        super()._on_measured(output)
