from typing import override

from view import View, State

from PySide6.QtCore import Signal

from devices.blood_pressure.session import BPSession
from devices.blood_pressure.config import BPConfig
from devices.blood_pressure.measurements_widget import BloodPressureMeasurementsWidget


class BPView(View):
    values_changed = Signal(list)

    def __init__(
        self,
        session: BPSession,
        config: BPConfig,
        detached: bool = False,
        parent=None
    ):
        self.measurement_form = BloodPressureMeasurementsWidget()

        super().__init__(
            parent=parent, session=session, config=config, detached=detached
        )

        self.session_widget.deviceStatusValue.setText("Blood Pressure")
        self.measurement_table_widget.deleteLater()

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
        self.measurement_form.values_changed.connect(self._on_values_changed)

    @override
    def on_started(self):
        super().on_started()
        self.manual_entry_button.setEnabled(False)

    @override
    def _on_manual_entry_clicked(self):
        self.manual_entry.emit([])

    def _on_values_changed(self, data):
        self.values_changed.emit(data)
        self.manual_entry_button.setEnabled(True)

    @override
    def on_measured(self, output: dict):
        if self.state != State.MANUAL_ENTRY:
            super().on_measured()
            self.session_widget.statusValue.setText("Ready to submit")

        self.measurement_form.on_measured(output)

    @override
    def on_manual_entry(self):
        self.state = State.MANUAL_ENTRY
        self.session_widget.statusValue.setText(f"Manual entry")
        self.session_widget.startButton.setEnabled(False)
        self.start_button.setEnabled(False)
        self.manual_entry_button.setEnabled(False)
        self.measurement_form.set_enabled(True)
        self.measure_button.setEnabled(False)
        self.submit_button.setEnabled(False)

    @override
    def on_submitted(self):
        super().on_submitted()
        self.measurement_form.setEnabled(False)
        self.measurement_form.deleteRow.setEnabled(False)
