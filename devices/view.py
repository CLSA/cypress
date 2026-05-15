import logging

from enum import Enum

from PySide6 import QtWidgets
from PySide6.QtWidgets import QMessageBox
from PySide6.QtGui import QIcon
from PySide6.QtCore import Signal

from ui.measurement_table import MeasurementTableWidget
from ui.test_info_widget import TestInfoWidget

from session import Session
from config import DeviceConfig

from datetime import datetime

from files.uploader import PineAPI

import settings


class State(Enum):
    BEGIN = 0
    STARTED = 1
    READY_TO_MEASURE = 3
    MEASURED = 2
    SUBMITTED = 4
    ERROR = 5


class View(QtWidgets.QDialog):
    start = Signal()
    measure = Signal()
    submit = Signal()

    def __init__(
        self, session: Session, config: DeviceConfig, detached=False, parent=None, title="Cypress"
    ):
        super().__init__(parent)

        self.state = State.BEGIN
        self.session = session
        self.logger = logging.getLogger(config.section_name)
        self.config = config
        self.detached = detached

        self.setWindowTitle(title)
        self.setWindowIcon(QIcon("favicon.ico"))

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
        self.logger.info("start requested")
        self.start.emit()

    def _on_measure_button_clicked(self):
        self.logger.info("measure requested")
        self.measurement_table_widget.measureButton.setEnabled(False)
        self.measure.emit()

    def _on_submit_button_clicked(self):
        self.logger.info("submit requested")
        self.measurement_table_widget.submitButton.setEnabled(False)
        self.submit.emit()

    def _on_manual_entry_clicked(self):
        self.logger.info("manual entry requested")
        pass

    def on_started(self):
        self.logger.info("started, waiting..")
        self.test_info_widget.statusValue.setText("Waiting...")
        self.test_info_widget.startButton.setEnabled(False)
        self.measurement_table_widget.measureButton.setEnabled(False)
        self.measurement_table_widget.submitButton.setEnabled(False)
        self.state = State.STARTED

    def on_ready_to_measure(self):
        self.logger.info("ready to measure")
        self.test_info_widget.statusValue.setText("Ready to measure")
        self.measurement_table_widget.measureButton.setEnabled(True)
        self.state = State.READY_TO_MEASURE

    def on_measured(self, results):
        self.logger.info("measured, ready to submit..")
        self.measurement_table_widget.measureButton.setEnabled(True)
        self.test_info_widget.statusValue.setText("Ready to submit")
        self.measurement_table_widget.submitButton.setEnabled(True)
        self.measurement_table_widget.measureButton.setEnabled(False)
        self.state = State.MEASURED

    def on_submitted(self):
        self.logger.info("submitted, ready to close")
        self.measurement_table_widget.submitButton.setEnabled(True)
        self.test_info_widget.statusValue.setText("Complete")
        self.state = State.SUBMITTED

    def on_error(self, title: str = "Error", message: str = "Unknown error"):
        self.test_info_widget.statusValue.setText(f"Error: {message}")
        self.measurement_table_widget.measureButton.setEnabled(False)
        self.measurement_table_widget.submitButton.setEnabled(False)
        self.state = State.ERROR

        msg = QMessageBox()
        msg.critical(self, title, message)

    def closeEvent(self, event):
        self.logger.info("close requested")

        if self.state == State.SUBMITTED:
            self.logger.info("done")
            event.accept()
            return

        reply = QMessageBox.question(
            self,
            "Confirm Close",
            "Are you sure you want to close? Any unsaved results will be lost.",
            QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No,
            QMessageBox.StandardButton.No,
        )

        if reply == QMessageBox.StandardButton.Yes:
            if not self.detached:
                cancelled = PineAPI(
                    base_url=settings.config.pine,
                    auth_token=settings.PINE_AUTH_TOKEN,
                    logger_name=self.config.section_name,
                ).send_cancel(session_id=self.session.session_id)

                if not cancelled:
                    self.logger.warning("pine did not accept cancel request")

            event.accept()
        else:
            self.logger.info("close declined")
            event.ignore()
