from typing import override

from view import View

from devices.blood_pressure.session import BPSession
from devices.blood_pressure.config import BPConfig
from devices.blood_pressure.measurements_widget import BloodPressureMeasurementsWidget


class BPView(View):
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
        self.manual_entry_button.setVisible(False)
        self.manual_entry_button.setEnabled(False)

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
        super().on_measured()
        self.manual_entry_button.setEnabled(True)
        self.measurement_form.on_measured(output)