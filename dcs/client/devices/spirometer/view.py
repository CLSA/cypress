from typing import override

from PySide6.QtWidgets import (
    QDialog,
    QComboBox,
    QVBoxLayout,
    QHBoxLayout,
    QPushButton,
    QSpacerItem,
)
from PySide6.QtCore import Qt
from view import View

from devices.spirometer.session import SpirometerSession
from devices.spirometer.config import SpirometerConfig

ethnicities = {
    "Caucasian": "Caucasian",
    "Hispanic": "Hispanic",
    "Asian": "Asian",
    "African": "African",
    "Other": "Other_Ethnic",
    "North-East Asian": "NE_Asian",
    "South-East Asian": "SE_Asian",
}


class SelectEthnicityDialog(QDialog):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.setWindowTitle("Select ethnicity")
        self.setWindowFlags(
            self.windowFlags() & ~Qt.WindowType.WindowMaximizeButtonHint
        )
        self.setFixedSize(300, 100)

        self.ok_button = QPushButton(self)
        self.ok_button.setText("Ok")
        self.ok_button.clicked.connect(self.accept)

        self.cancel_button = QPushButton(self)
        self.cancel_button.setText("Cancel")
        self.cancel_button.clicked.connect(self.reject)

        self.combo_box = QComboBox(self)
        for key, value in ethnicities.items():
            self.combo_box.addItem(key, value)
        self.combo_box.setCurrentIndex(0)

        v_layout = QVBoxLayout()
        v_layout.addWidget(self.combo_box)

        h_layout = QHBoxLayout()
        h_layout.addSpacerItem(QSpacerItem(100, 0))
        h_layout.addWidget(self.ok_button)
        h_layout.addWidget(self.cancel_button)

        v_layout.addLayout(h_layout)
        self.setLayout(v_layout)


    def get_selected_value(self) -> str:
        current_index = self.combo_box.currentIndex()

        if current_index >= 0:
            return str(self.combo_box.itemData(current_index))

        return None


class SpirometerView(View):
    def __init__(
        self,
        session: SpirometerSession,
        config: SpirometerConfig,
        detached: bool = False,
        parent=None,
    ):
        super().__init__(
            parent=parent, session=session, config=config, detached=detached
        )

        self.session_widget.deviceStatusValue.setText("Spirometer")
        self.table = self.measurement_table_widget.measurementTable
        self.table.setVisible(False)

        self.measure_button.setVisible(True)
        self.start_button.setVisible(False)

        self.setFixedSize(400, 250)

    @override
    def _on_measured(self, output: dict):
        self.table.clear()
