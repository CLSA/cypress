import logging

from enum import Enum

from PySide6.QtWidgets import (
    QMessageBox,
    QDialog,
    QVBoxLayout,
    QSpacerItem,
    QSizePolicy,
)
from PySide6.QtGui import QIcon
from PySide6.QtCore import Signal, Qt

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
    READY_TO_MEASURE = 2
    MEASURED = 3
    SUBMITTED = 4
    MANUAL_ENTRY = 5
    ERROR = 6


class View(QDialog):
    start = Signal()
    measure = Signal()
    submit = Signal()
    close = Signal()

    def __init__(
        self,
        session: Session,
        config: DeviceConfig,
        detached=False,
        parent=None,
        title="Cypress",
    ):
        super().__init__(parent)

        self.setWindowTitle(title)
        self.setWindowIcon(QIcon("favicon.ico"))

        self.session = session
        self.logger = logging.getLogger(config.section_name)
        self.config = config
        self.detached = detached
        self.title = title

        self.session_widget = TestInfoWidget(self)
        self.measurement_table_widget = MeasurementTableWidget(self)
        self.measurement_table_widget.setSizePolicy(
            QSizePolicy.Policy.Expanding,
            QSizePolicy.Policy.Expanding,
        )

        layout = QVBoxLayout()
        layout.addWidget(self.session_widget)
        layout.addWidget(self.measurement_table_widget)
        #layout.setAlignment(Qt.AlignmentFlag.AlignTop)
        # layout.addStretch()

        self.setLayout(layout)
        self.resize(config.width, config.height)

        self._get_button_references()
        self._connect_signals()

        self.on_begin()

    def _get_button_references(self):
        self.start_button = self.session_widget.startButton
        self.measure_button = self.measurement_table_widget.measureButton
        self.submit_button = self.measurement_table_widget.submitButton
        self.manual_entry_button = self.measurement_table_widget.manualEntryToggle

    def _connect_signals(self):
        self.session_widget.startButton.clicked.connect(self._on_start_button_clicked)
        self.submit_button.clicked.connect(self._on_submit_button_clicked)
        self.measure_button.clicked.connect(self._on_measure_button_clicked)
        self.manual_entry_button.clicked.connect(self._on_manual_entry_clicked)

    # slots
    def _on_start_button_clicked(self):
        self.logger.info("start requested")
        self.start.emit()

    def _on_measure_button_clicked(self):
        self.logger.info("measure requested")
        self.measure.emit()

    def _on_submit_button_clicked(self):
        self.logger.info("submit requested")
        if not self.detached:
            self.submit_button.setEnabled(False)
        self.submit.emit()

    def _on_manual_entry_clicked(self):
        self.logger.info("manual entry requested")
        self.on_manual_entry()

    def on_begin(self):
        self.session_widget.startedValue.setText(datetime.now().strftime("%I:%M %p"))
        self.session_widget.statusValue.setText("Press 'Start' to begin")
        self.session_widget.barcodeValue.setText(self.session.barcode)
        self.session_widget.interviewerValue.setText(self.session.interviewer)
        self.session_widget.deviceStatusValue.setText(self.title)

        self.measure_button.setVisible(False)
        self.measure_button.setEnabled(False)
        self.manual_entry_button.setVisible(False)
        self.submit_button.setEnabled(False)

        self.state = State.BEGIN

    def on_started(self):
        self.logger.info("started, waiting..")
        self.session_widget.statusValue.setText("Waiting...")
        self.session_widget.startButton.setEnabled(False)
        self.measure_button.setEnabled(False)
        self.submit_button.setEnabled(False)
        self.state = State.STARTED

    def on_ready_to_measure(self):
        self.logger.info("ready to measure")
        self.session_widget.statusValue.setText("Ready to measure")
        self.measure_button.setEnabled(True)
        self.state = State.READY_TO_MEASURE

    def on_measured(self):
        self.logger.info("measured, ready to submit..")
        self.measure_button.setEnabled(True)
        self.session_widget.statusValue.setText("Ready to submit")
        self.submit_button.setEnabled(True)
        self.measure_button.setEnabled(False)
        self.state = State.MEASURED

    def on_submitted(self):
        self.logger.info("submitted, ready to close")
        self.submit_button.setEnabled(True)
        self.session_widget.statusValue.setText("Complete")
        self.state = State.SUBMITTED

    def on_error(self, title: str = "Error", message: str = "Unknown error"):
        self.logger.error(f"{title} - {message}")

        self.session_widget.statusValue.setText(f"Error: {message}")
        self.measure_button.setEnabled(False)
        self.submit_button.setEnabled(False)
        self.state = State.ERROR

        msg = QMessageBox()
        msg.critical(self, title, message)

    def on_manual_entry(self):
        self.session_widget.statusValue.setText(f"Manual entry")
        self.session_widget.startButton.setEnabled(False)

        self.measure_button.setEnabled(False)
        self.submit_button.setEnabled(True)
        self.manual_entry_button.setEnabled(False)

        self.state = State.MANUAL_ENTRY

    def set_status(self, text: str):
        self.session_widget.statusValue.setText(text)

    def closeEvent(self, event):
        self.logger.info("close requested")

        if self.state == State.SUBMITTED:
            self.logger.info("done")
            self.close.emit()

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
                    base_url=self.session.origin,
                    logger=self.logger,
                ).send_cancel(session_id=self.session.session_id)

                if not cancelled:
                    self.logger.warning("pine did not accept cancel request")

            self.close.emit()

            event.accept()
        else:
            self.logger.info("close declined")
            event.ignore()

    @classmethod
    def class_name(cls):
        return cls.__name__
