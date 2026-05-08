from PySide6 import QtWidgets
from PySide6.QtWidgets import QMessageBox

from ui.measurement_table import MeasurementTableWidget
from ui.test_info_widget import TestInfoWidget

from devices.controller import Controller

from session import Session

from datetime import datetime


class View(QtWidgets.QDialog):
    def __init__(
        self, controller: Controller, session: Session, parent=None, title="Cypress"
    ):
        super().__init__(parent)

        self.setWindowTitle(title)

        self.controller = controller
        self.controller.started.connect(self._on_started)
        self.controller.finished.connect(self._on_finished)
        self.controller.measured.connect(self._on_measured)
        self.controller.submitted.connect(self._on_submitted)
        self.controller.error.connect(self._on_error)

        self.test_info_widget = TestInfoWidget(self)
        self.test_info_widget.startButton.clicked.connect(self._on_start_button_clicked)

        self.measurement_table_widget = MeasurementTableWidget(self)
        self.measurement_table_widget.submitButton.clicked.connect(
            self._on_submit_button_clicked
        )
        self.measurement_table_widget.measureButton.clicked.connect(
            self._on_measure_button_clicked
        )

        self.test_info_widget.startedValue.setText(datetime.now().strftime("%I:%M %p"))
        self.test_info_widget.statusValue.setText("Press 'Start' to begin")
        self.test_info_widget.barcodeValue.setText(session.barcode)
        self.test_info_widget.interviewerValue.setText(session.interviewer)
        self.test_info_widget.deviceStatusValue.setText(title)

        self.measurement_table_widget.addMeasureButton.setVisible(False)
        self.measurement_table_widget.manualEntryToggle.setVisible(False)
        self.measurement_table_widget.manualEntryToggle.clicked.connect(
            self._on_manual_entry_clicked
        )

        self.measurement_table_widget.measureButton.setEnabled(False)
        self.measurement_table_widget.submitButton.setEnabled(False)
        self.measurement_table_widget.submitButton.setEnabled(False)

        layout = QtWidgets.QVBoxLayout()
        layout.addWidget(self.test_info_widget)
        layout.addWidget(self.measurement_table_widget)

        self.setLayout(layout)

    # slots
    def _on_start_button_clicked(self):
        self.controller.start()

    def _on_measure_button_clicked(self):
        self.controller.measure()

    def _on_submit_button_clicked(self):
        self.measurement_table_widget.submitButton.setEnabled(False)
        submitted = self.controller.submit()

        if not submitted:
            self.measurement_table_widget.submitButton.setEnabled(True)

    def _on_manual_entry_clicked(self):
        print("manual entry clicked")

    def _on_started(self):
        self._set_started()

    def _on_finished(self):
        self._set_finished()

    def _on_measured(self, results):
        self._set_measured()

    def _on_submitted(self):
        self._set_complete()

    def _on_error(self, title: str = "Error", message: str = "Unknown error"):
        self._set_error(message)
        msg = QMessageBox()
        msg.critical(self, title, message)

    def _set_started(self):
        self.test_info_widget.statusValue.setText("Waiting...")
        self.test_info_widget.startButton.setEnabled(False)
        self.measurement_table_widget.measureButton.setEnabled(False)
        self.measurement_table_widget.submitButton.setEnabled(False)

    def _set_finished(self):
        self.test_info_widget.statusValue.setText("Ready to measure")
        self.measurement_table_widget.measureButton.setEnabled(True)

    def _set_error(self, message):
        self.test_info_widget.statusValue.setText(f"Error: {message}")
        self.measurement_table_widget.measureButton.setEnabled(False)
        self.measurement_table_widget.submitButton.setEnabled(False)

    def _set_measured(self):
        self.test_info_widget.statusValue.setText("Ready to submit")
        self.measurement_table_widget.submitButton.setEnabled(True)
        self.measurement_table_widget.measureButton.setEnabled(False)

    def _set_complete(self):
        self.test_info_widget.statusValue.setText("Finished")
