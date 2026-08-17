from typing import override

from view import View, State

from devices.echo.session import ECHOSession
from devices.echo.config import ECHOConfig
from devices.echo.echo_scans_widget import ECHOScansWidget


class ECHOView(View):
    def __init__(
        self,
        session: ECHOSession,
        config: ECHOConfig,
        detached: bool = False,
        parent=None,
    ):
        super().__init__(
            parent=parent, session=session, config=config, detached=detached
        )

        self.session_widget.deviceStatusValue.setText("ECHO")

    @override
    def _get_button_references(self):
        self.measure_button = self.echo_scans_widget.measure_button
        self.submit_button = self.echo_scans_widget.submit_button
        self.manual_entry_button = self.echo_scans_widget.manual_entry_button
        self.start_button = self.session_widget.startButton

    @override
    def _add_measurement_table_widget(self):
        self.echo_scans_widget = ECHOScansWidget(
            log_path=self.config.log_path, parent=self
        )
        self.layout().addWidget(self.echo_scans_widget)

    def on_files_received(self, totals: dict):
        self.echo_scans_widget.set_totals(totals)

    @override
    def on_submitting(self):
        self.logger.info("submitting")
        if self.detached:
            self.session_widget.statusValue.setText("Saving..")

        self.measure_button.setEnabled(False)

        if not self.detached:
            self.submit_button.setEnabled(False)

        self.manual_entry_button.setEnabled(False)

    @override
    def on_ready_to_measure(self):
        super().on_ready_to_measure()
        self.session_widget.statusValue.setText("Press measure once all files received")

    @override
    def on_submitted(self):
        super().on_submitted()
        # self.submit_button.setEnabled(True)
        # self.session_widget.statusValue.setText("Complete")
        # self.state = State.SUBMITTED

        # self.manual_entry_button.setEnabled(False)
        # self.start_button.setEnabled(False)
        # self.submit_button.setEnabled(False)
