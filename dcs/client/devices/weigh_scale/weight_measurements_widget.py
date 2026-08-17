import datetime
import logging

from PySide6.QtCore import Qt, Signal

from PySide6.QtWidgets import (
    QWidget,
    QHeaderView,
    QAbstractItemView,
    QTableWidgetItem,
    QPushButton,
)

MAX_ROWS = 6

from ui.ui_weight_measurements import Ui_WeightMeasurements

from utils import DoubleSpinBoxDelegate

from devices.weigh_scale.config import WeighScaleConfig

logger = logging.getLogger("weigh_scale")

class WeightMeasurementsWidget(QWidget, Ui_WeightMeasurements):
    values_changed = Signal(list)
    delete_measure = Signal(int)

    def __init__(self, config: WeighScaleConfig, parent=None):
        super().__init__(parent=parent)
        self.setupUi(self)

        self.config = config

        self.table.clear()
        self.columns = ["Weight", "Unit", "Time"]

        self._set_table()

        self.table.setEnabled(True)
        self.deleteRow.setEnabled(False)

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

        self.zeroButton.setVisible(True)
        self.zeroButton.setEnabled(True)

        self.addRow.setEnabled(False)
        self.addRow.setVisible(False)

        self.set_enabled(False)

        #self.deleteRow.setEnabled(False)
        #self.deleteRow.setVisible(False)

    def set_enabled(self, enabled: bool) -> None:
        self.table.clearSelection()

        self.save_manual_entry.setVisible(enabled)
        self.save_manual_entry.setEnabled(enabled)
        self.addRow.setEnabled(enabled)
        self.addRow.setVisible(enabled)
        self.zeroButton.setEnabled(not enabled)

        if enabled:
            self.table.setEditTriggers(QAbstractItemView.DoubleClicked | QAbstractItemView.SelectedClicked)
        else:
            self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)

    def on_save_manual_entry(self):
        self.set_enabled(False)
        self.manualEntryButton.setEnabled(True)
        self._handle_values_changed()

    def _handle_values_changed(self):
        measures = []
        for row in range(self.table.rowCount()):
            measures.append((float(self.table.item(row, 0).text()), str(self.table.item(row, 2))))

        self.values_changed.emit(measures)

    def on_measured(self, output: dict):
        try:
            logger.debug(f"widget: {output}")
            self.table.clear()
            self._set_table()

            self.table.setRowCount(len(output["value"]["results"]))

            logger.debug(len(output["value"]["results"]))

            for index, result in enumerate(output["value"]["results"]):
                logger.debug(index, result)
                weight = QTableWidgetItem(str(result["weight"].get("value")))
                unit = QTableWidgetItem(str(result["weight"].get("units")))
                time = QTableWidgetItem(str(result["timestamp"]))

                self.table.setItem(index, 0, weight)
                self.table.setItem(index, 1, unit)
                self.table.setItem(index, 2, time)
        except Exception as e:
            logger.error(e)

    def add_row(self):
        self.table.clearSelection()
        self.deleteRow.setEnabled(False)

        row_count = self.table.rowCount()
        if row_count > MAX_ROWS - 1:
            return False

        weight_item = QTableWidgetItem(str(0.00))
        unit_item = QTableWidgetItem("kg")
        unit_item.setFlags(unit_item.flags() & ~Qt.ItemFlag.ItemIsEditable)
        time_item = QTableWidgetItem("---")
        time_item.setFlags(time_item.flags() & ~Qt.ItemFlag.ItemIsEditable)

        self.table.insertRow(row_count)
        self.table.setItem(row_count, 0, weight_item)
        self.table.setItem(row_count, 1, unit_item)
        self.table.setItem(row_count, 2, time_item)

    def _on_row_selected(self):
        self.row_selected = self.table.currentRow()
        self.deleteRow.setEnabled(True)

    def delete_row(self):
        if self.row_selected is None:
            return

        # Only signal if deleted outside of manual entry
        if self.manualEntryButton.isEnabled():
            self.delete_measure.emit(self.row_selected)
        else:
            self.table.removeRow(self.row_selected)

        self.table.clearSelection()
        self.row_selected = None
        self.deleteRow.setEnabled(False)


    def _set_table(self):
        self.table.horizontalHeader().setDefaultAlignment(Qt.AlignmentFlag.AlignLeft)
        self.table.setColumnCount(len(self.columns))
        self.table.setHorizontalHeaderLabels(self.columns)

        self.header = self.table.horizontalHeader()

        for i in range(0, self.table.columnCount()):
            self.header.setSectionResizeMode(i, QHeaderView.Stretch)
            column = self.table.horizontalHeaderItem(i)
            column.setTextAlignment(Qt.AlignmentFlag.AlignLeft)
            self.table.setColumnWidth(i, 175)

        self.table.setItemDelegateForColumn(
            0,
            DoubleSpinBoxDelegate(
                min_val=1, max_val=999, decimals=1, locale=self.config.locale
            ),
        )

        self.table.setSelectionMode(QAbstractItemView.SingleSelection)
        self.table.setSelectionBehavior(QAbstractItemView.SelectionBehavior.SelectItems)
