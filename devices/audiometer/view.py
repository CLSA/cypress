from typing import override

from PySide6.QtWidgets import QTableWidgetItem, QHeaderView, QAbstractItemView, QDialog
from PySide6.QtCore import Qt, Signal


from devices.view import View
from devices.audiometer.hearing_measurements import HearingMeasurementsWidget
from devices.audiometer.session import AudiometerSession
from devices.audiometer.config import AudiometerConfig
from devices.audiometer.manual_entry import AudiometerManualEntryDialog


class AudiometerView(View):
    manual_entry = Signal(dict)

    def __init__(
        self,
        session: AudiometerSession,
        config: AudiometerConfig,
        detached: bool = False,
        parent=None,
    ):
        self.measurement_form = HearingMeasurementsWidget()

        super().__init__(
            parent=parent, session=session, config=config, detached=detached
        )

        self.resize(600, 500)

        # Deleting default measure table to replace with widget below
        self.measurement_table_widget.deleteLater()
        self.measurement_form.set_enabled(False)

        self.measure_button.setVisible(True)
        self.manual_entry_button.setVisible(True)
        self.manual_entry_button.setEnabled(True)

        self.layout().addWidget(self.measurement_form)


    @override
    def _get_button_references(self):
        super()._get_button_references()

        self.measure_button = self.measurement_form.measureButton
        self.submit_button = self.measurement_form.submitButton
        self.manual_entry_button = self.measurement_form.manualEntryButton

    @override
    def _connect_signals(self):
        self.session_widget.startButton.clicked.connect(self._on_start_button_clicked)
        self.submit_button.clicked.connect(self._on_submit_button_clicked)
        self.measure_button.clicked.connect(self._on_measure_button_clicked)
        self.manual_entry_button.clicked.connect(self._on_manual_entry_clicked)

    @override
    def _on_manual_entry_clicked(self):
        super()._on_manual_entry_clicked()
        self.measurement_form.set_enabled(True)

    @override
    def on_measured(self, output: dict):
        super().on_measured(output)
        # self.measurement_form.set_values(output)
