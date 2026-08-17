from typing import override

from pathlib import Path

from view import View

from devices.dxa.session import DXASession
from devices.dxa.config import DXAConfig
from devices.dxa.dxa_scans_widget import DXAScansWidget


class DXAView(View):
    def __init__(
        self,
        session: DXASession,
        config: DXAConfig,
        detached: bool = False,
        parent=None,
    ):
        super().__init__(
            parent=parent, session=session, config=config, detached=detached
        )

        self.session_widget.deviceStatusValue.setText("DXA")

        self.measure_button.setVisible(True)
        self.start_button.setVisible(False)

    @override
    def _get_button_references(self):
        self.measure_button = self.dxa_scans_widget.measure_button
        self.submit_button = self.dxa_scans_widget.submit_button
        self.manual_entry_button = self.dxa_scans_widget.manual_entry_button
        self.start_button = self.session_widget.startButton

    @override
    def _add_measurement_table_widget(self):
        self.dxa_scans_widget = DXAScansWidget(
            log_path=self.config.log_path, session=self.session, parent=self
        )
        self.dxa_scans_widget.set_includes()
        self.layout().addWidget(self.dxa_scans_widget)

    @override
    def on_ready_to_measure(self):
        super().on_ready_to_measure()
        self.session_widget.statusValue.setText(
            "Press measure once scans are transferred"
        )

    @override
    def on_measured(self, status: dict):
        self.dxa_scans_widget.update_status(status)
        super().on_measured()
        self.measure_button.setEnabled(False)

    def on_files_received(self, status: dict):
        self.dxa_scans_widget.update_status(status)
