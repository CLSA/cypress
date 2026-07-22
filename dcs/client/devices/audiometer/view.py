from typing import override

from PySide6.QtCore import Signal


from view import View, State

from devices.audiometer.hearing_measurements import HearingMeasurementsWidget
from devices.audiometer.session import AudiometerSession
from devices.audiometer.config import AudiometerConfig


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

        # Deleting default measure table to replace with widget below
        self.session_widget.deviceStatusValue.setText("Hearcon")

        self.measurement_table_widget.deleteLater()
        self.measurement_form.set_enabled(False)

        self.measure_button.setVisible(True)
        self.manual_entry_button.setVisible(True)
        self.manual_entry_button.setEnabled(True)

        self.layout().addWidget(self.measurement_form)
        self.setFixedSize(560, 581)

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
        super().on_measured()
        self.measurement_form.set_values(output)

    @override
    def on_ready_to_measure(self):
        self.session_widget.statusValue.setText("Waiting...")
        self.measure_button.setEnabled(True)
        self.state = State.READY_TO_MEASURE

    @override
    def on_submitted(self):
        super().on_submitted()
        self.measurement_form.setEnabled(False)
