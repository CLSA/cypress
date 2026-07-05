from typing import override

from PySide6.QtWidgets import QTableWidgetItem, QHeaderView, QAbstractItemView, QDialog
from PySide6.QtCore import Qt

from view import View

from devices.blood_pressure.session import BPSession
from devices.blood_pressure.config import BPConfig
from devices.blood_pressure.manual_entry import BPManualEntryDialog


class BPView(View):
    def __init__(
        self,
        session: BPSession,
        config: BPConfig,
        detached: bool = False,
        parent=None
    ):
        super().__init__(
            parent=parent, session=session, config=config, detached=detached
        )

        self.session_widget.deviceStatusValue.setText("Blood Pressure")

        self.table = self.measurement_table_widget.measurementTable

        self.measurement_table_widget.manualEntryToggle.setEnabled(True)
        self.measurement_table_widget.manualEntryToggle.setVisible(True)

        self.manual_entry_form = BPManualEntryDialog()

    @override
    def _on_manual_entry_clicked(self):
        result = self.manual_entry_form.exec()

        if result == QDialog.Accepted:
            data = self.manual_entry_form.get_data()
            self.controller.manual_entry(data)

    @override
    def _on_measured(self, output: dict):
        # return super()._on_measured(results)
        self.results = output
        self.table.clear()

        columns = ["#", "Systolic (mmHg)", "Diastolic (mmHg)", "Pulse (bpm)"]

        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.table.horizontalHeader().setDefaultAlignment(Qt.AlignmentFlag.AlignLeft)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setRowCount(len(output["results"]))
        self.table.setColumnCount(len(columns))
        self.table.setHorizontalHeaderLabels(columns)

        for index, result in enumerate(output["results"]):
            number = QTableWidgetItem(result["number"])
            systolic = QTableWidgetItem(result["systolic"]["value"])
            diastolic = QTableWidgetItem(result["diastolic"]["value"])
            pulse = QTableWidgetItem(result["pulse"]["value"])

            self.table.setItem(index, 0, number)
            self.table.setItem(index, 1, systolic)
            self.table.setItem(index, 2, diastolic)
            self.table.setItem(index, 3, pulse)

        super()._on_measured(output)
