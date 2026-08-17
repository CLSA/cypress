from typing import override

from PySide6.QtWidgets import QTableWidgetItem, QHeaderView, QAbstractItemView
from PySide6.QtCore import Qt

from view import View
from devices.frax.session import FRAXSession
from devices.frax.config import FRAXConfig


class FRAXView(View):
    def __init__(
        self,
        session: FRAXSession,
        config: FRAXConfig,
        detached: bool = False,
        parent=None,
    ):
        super().__init__(
            parent=parent, session=session, config=config, detached=detached
        )

        self.session_widget.deviceStatusValue.setText("FRAX")
        self.table = self.measurement_table_widget.measurementTable

        self.start_button.setVisible(True)
        self.start_button.setEnabled(True)

        self.table.setEditTriggers(QAbstractItemView.EditTrigger.NoEditTriggers)
        self.table.setFocusPolicy(Qt.FocusPolicy.NoFocus)
        self.table.setSelectionMode(QAbstractItemView.SelectionMode.NoSelection)

    @override
    def on_measured(self, output: dict):
        self.table.clear()

        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setRowCount(1)
        self.table.setColumnCount(2)
        self.table.setHorizontalHeaderLabels(["Type", "Probability"])
        self.table.verticalHeader().setVisible(False)

        result = output["value"]["results"][2]
        risk = QTableWidgetItem("Fracture Risk")
        probability = QTableWidgetItem(
            f"{result['probability']['value']}{result['probability']['unit']}"
        )

        self.table.setItem(0, 0, risk)
        self.table.setItem(0, 1, probability)

        super().on_measured()
