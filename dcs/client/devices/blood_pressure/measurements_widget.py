from PySide6.QtCore import Qt, Signal

from PySide6.QtWidgets import (
    QWidget,
    QHeaderView,
    QAbstractItemView,
    QTableWidgetItem,
    QStyledItemDelegate,
    QSpinBox,
    QDoubleSpinBox,
)

from ui.ui_blood_pressure_measurements import Ui_BloodPressureMeasurements


class IntegerSpinBoxDelegate(QStyledItemDelegate):

    def __init__(self, min_val, max_val, parent=None):
        super().__init__(parent)
        self.min_val = min_val
        self.max_val = max_val

    def createEditor(self, parent, option, index):
        editor = QSpinBox(parent)
        editor.setRange(self.min_val, self.max_val)
        return editor

    def setEditorData(self, editor, index):
        value = index.model().data(index, Qt.EditRole)
        editor.setValue(int(value) if value else self.min_val)

    def setModelData(self, editor, model, index):
        editor.interpretText()
        value = editor.value()
        model.setData(index, value, Qt.EditRole)

    def updateEditorGeometry(self, editor, option, index):
        editor.setGeometry(option.rect)


class DoubleSpinBoxDelegate(QStyledItemDelegate):

    def __init__(self, min_val, max_val, decimals=2, parent=None):
        super().__init__(parent)
        self.min_val = min_val
        self.max_val = max_val
        self.decimals = decimals

    def createEditor(self, parent, option, index):
        editor = QDoubleSpinBox(parent)
        editor.setRange(self.min_val, self.max_val)
        editor.setDecimals(self.decimals)
        return editor

    def setEditorData(self, editor, index):
        value = index.model().data(index, Qt.EditRole)
        editor.setValue(float(value) if value else self.min_val)

    def setModelData(self, editor, model, index):
        editor.interpretText()
        value = editor.value()
        model.setData(index, value, Qt.EditRole)

    def updateEditorGeometry(self, editor, option, index):
        editor.setGeometry(option.rect)


class BloodPressureMeasurementsWidget(QWidget, Ui_BloodPressureMeasurements):
    values_changed = Signal(list)

    def __init__(self, parent=None):
        super().__init__(parent=parent)
        self.setupUi(self)

        self.table.clear()
        self.columns = ["#", "Systolic (mmHg)", "Diastolic (mmHg)", "Pulse (bpm)"]

        self._set_table()

        self.table.setEnabled(False)

    def set_enabled(self, enabled: bool) -> None:
        self.table.cellChanged.connect(self.on_cell_changed)

        self.measures = []
        for row in range(self.table.rowCount()):
            self.measures.append(
                {
                    "Spare7": int(self.table.item(row, 0).text()),
                    "SYS": int(self.table.item(row, 1).text()),
                    "DIA": int(self.table.item(row, 2).text()),
                    "HR": int(self.table.item(row, 3).text()),
                }
            )

        self.table.setEnabled(enabled)

    def on_cell_changed(self, row, column):
        self.measures[row] = {
            "Spare7": int(self.table.item(row, 0).text()),
            "SYS": int(self.table.item(row, 1).text()),
            "DIA": int(self.table.item(row, 2).text()),
            "HR": int(self.table.item(row, 3).text()),
        }
        self.values_changed.emit(self.measures)

    def on_measured(self, output: dict):
        self.table.clear()
        self._set_table()

        self.table.setRowCount(len(output["value"]["results"]))

        for index, result in enumerate(output["value"]["results"]):
            reading_number = QTableWidgetItem(str(result["reading_number"]))
            reading_number.setFlags(
                reading_number.flags()
                & ~Qt.ItemFlag.ItemIsEditable
                & ~Qt.ItemFlag.ItemIsSelectable
            )

            systolic = QTableWidgetItem(str(result["systolic"].get("value")))
            diastolic = QTableWidgetItem(str(result["diastolic"].get("value")))
            pulse = QTableWidgetItem(str(result["pulse"].get("value")))

            self.table.setItem(index, 0, reading_number)
            self.table.setItem(index, 1, systolic)
            self.table.setItem(index, 2, diastolic)
            self.table.setItem(index, 3, pulse)

    def _set_table(self):
        # self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        # self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)

        self.table.setStyleSheet(
            """
            QTableWidget { outline: 0; }
            QTableWidget::item:focus, QTableWidget::item:selected {
                background-color: #2a82da;
                color: white;
            }
            """
        )

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

        self.table.setItemDelegateForColumn(1, IntegerSpinBoxDelegate(1, 999))
        self.table.setItemDelegateForColumn(2, IntegerSpinBoxDelegate(1, 999))
        self.table.setItemDelegateForColumn(3, IntegerSpinBoxDelegate(1, 999))

        self.table.setSelectionMode(QAbstractItemView.SingleSelection)
        self.table.setSelectionBehavior(QAbstractItemView.SelectRows)
