from typing import override

from PySide6.QtCore import Qt, Signal

from PySide6.QtWidgets import (
    QWidget,
    QHeaderView,
    QAbstractItemView,
    QTableWidgetItem,
    QPushButton,
)

from ui.ui_blood_pressure_measurements import Ui_BloodPressureMeasurements

from utils import IntegerSpinBoxDelegate

class BloodPressureMeasurementsWidget(QWidget, Ui_BloodPressureMeasurements):
    values_changed = Signal(list)

    def __init__(self, parent=None):
        super().__init__(parent=parent)
        self.setupUi(self)

        self.table.clear()
        self.columns = ["#", "Systolic (mmHg)", "Diastolic (mmHg)", "Pulse (bpm)"]

        self._set_table()

        self.table.setEnabled(False)
        self.table.itemSelectionChanged.connect(self._on_row_selected)

        self.addRow.pressed.connect(self.add_row)
        self.deleteRow.pressed.connect(self.delete_row)

        self.save_manual_entry = QPushButton()
        self.save_manual_entry.setText("Save")
        self.save_manual_entry.setObjectName("save_manual_entry")
        self.save_manual_entry.pressed.connect(self.on_save_manual_entry)
        self.testControls.insertWidget(1, self.save_manual_entry)

        self.save_manual_entry.setVisible(False)
        self.save_manual_entry.setEnabled(False)

        self.addRow.setEnabled(False)
        self.addRow.setVisible(False)

        self.deleteRow.setEnabled(False)
        self.deleteRow.setVisible(False)

    def set_enabled(self, enabled: bool) -> None:
        self.save_manual_entry.setVisible(enabled)
        self.save_manual_entry.setEnabled(enabled)
        self.addRow.setEnabled(enabled)
        self.addRow.setVisible(enabled)
        self.deleteRow.setVisible(enabled)
        self.submitButton.setEnabled(not enabled)

        self.table.setEnabled(enabled)

        if self.table.rowCount() < 1:
            self.add_row()

    def on_save_manual_entry(self):
        self.table.clearSelection()
        self.row_selected = None
        self.deleteRow.setEnabled(False)
        self.set_enabled(False)
        self.manualEntryButton.setEnabled(True)
        self.submitButton.setEnabled(False)

        measures = []
        for row in range(self.table.rowCount()):
            measures.append(
                {
                    "Spare7": int(self.table.item(row, 0).text()),
                    "SYS": int(self.table.item(row, 1).text()),
                    "DIA": int(self.table.item(row, 2).text()),
                    "HR": int(self.table.item(row, 3).text()),
                }
            )

        self.values_changed.emit(measures)

    def on_measured(self, output: dict):
        self.table.clear()
        self._set_table()

        self.table.setRowCount(len(output["value"]["results"]))

        for index, result in enumerate(output["value"]["results"]):
            reading_number = QTableWidgetItem(str(result["reading_number"]))
            reading_number.setFlags(
                reading_number.flags() & ~Qt.ItemFlag.ItemIsEditable
            )

            systolic = QTableWidgetItem(str(result["systolic"].get("value")))
            diastolic = QTableWidgetItem(str(result["diastolic"].get("value")))
            pulse = QTableWidgetItem(str(result["pulse"].get("value")))

            self.table.setItem(index, 0, reading_number)
            self.table.setItem(index, 1, systolic)
            self.table.setItem(index, 2, diastolic)
            self.table.setItem(index, 3, pulse)

    def add_row(self):
        self.table.clearSelection()
        self.deleteRow.setEnabled(False)

        row_count = self.table.rowCount()
        if row_count > 5:
            return False

        last_row_index = row_count - 1

        new_reading_number = 1
        if last_row_index > -1:
            new_reading_number = int(self.table.item(last_row_index, 0).text()) + 1

        reading_number_item = QTableWidgetItem(str(new_reading_number))
        reading_number_item.setFlags(
            reading_number_item.flags() & ~Qt.ItemFlag.ItemIsEditable
        )

        systolic_item = QTableWidgetItem()
        diastolic_item = QTableWidgetItem()
        pulse_item = QTableWidgetItem()

        self.table.insertRow(row_count)
        self.table.setItem(row_count, 0, reading_number_item)
        self.table.setItem(row_count, 1, systolic_item)
        self.table.setItem(row_count, 2, diastolic_item)
        self.table.setItem(row_count, 3, pulse_item)

    def _on_row_selected(self):
        self.row_selected = self.table.currentRow()
        self.deleteRow.setEnabled(True)

    def delete_row(self):
        if self.row_selected is None:
            return

        self.table.removeRow(self.row_selected)
        self.table.clearSelection()
        self.row_selected = None
        self.deleteRow.setEnabled(False)

        for i in range(self.table.rowCount()):
            self.table.item(i, 0).setText(str(i + 1))


    def _set_table(self):
        # self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        # self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)

        self.table.setStyleSheet("""
            QTableWidget { outline: 0; }
            QTableWidget::item:focus, QTableWidget::item:selected {
                background-color: #2a82da;
                color: white;
            }
            """)

        self.table.horizontalHeader().setDefaultAlignment(Qt.AlignmentFlag.AlignLeft)
        self.table.setColumnCount(len(self.columns))
        self.table.setHorizontalHeaderLabels(self.columns)

        self.header = self.table.horizontalHeader()

        self.table.setColumnWidth(0, 32)
        self.table.horizontalHeaderItem(0).setTextAlignment(
            Qt.AlignmentFlag.AlignHCenter
        )
        self.header.setSectionResizeMode(0, QHeaderView.Fixed)

        for i in range(1, self.table.columnCount() - 1):
            self.header.setSectionResizeMode(i, QHeaderView.Stretch)
            column = self.table.horizontalHeaderItem(i)
            column.setTextAlignment(Qt.AlignmentFlag.AlignLeft)
            self.table.setColumnWidth(i, 175)

        self.header.setSectionResizeMode(3, QHeaderView.Stretch)

        self.table.setItemDelegateForColumn(1, IntegerSpinBoxDelegate(0, 999))
        self.table.setItemDelegateForColumn(2, IntegerSpinBoxDelegate(0, 999))
        self.table.setItemDelegateForColumn(3, IntegerSpinBoxDelegate(0, 999))

        self.table.setSelectionMode(QAbstractItemView.SingleSelection)
        self.table.setSelectionBehavior(QAbstractItemView.SelectRows)
