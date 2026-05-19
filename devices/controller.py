import json
import logging

from pathlib import Path

from PySide6.QtCore import QObject, QProcess, Signal
from PySide6.QtWidgets import QFileDialog

from session import Session
from config import DeviceConfig

from devices.model import Model
from devices.view import View

from files.uploader import DataUploaderDialog


class Controller(QObject):
    started = Signal()
    ready_to_measure = Signal()
    measured = Signal(dict)
    submitted = Signal()
    error = Signal(str, str)

    def __init__(
        self,
        session: Session,
        config: DeviceConfig,
        model: Model,
        view: View,
        detached: bool = False,
        parent=None,
    ):

        super().__init__(parent)

        self.session = session
        self.config = config
        self.model = model
        self.detached = detached
        self.logger = logging.getLogger(self.config.section_name)

        self.view = view
        self.view.start.connect(self.start)
        self.view.measure.connect(self.measure)
        self.view.submit.connect(self.submit)

        self.started.connect(self.view.on_started)
        self.ready_to_measure.connect(self.view.on_ready_to_measure)
        self.measured.connect(self.view.on_measured)
        self.submitted.connect(self.view.on_submitted)
        self.error.connect(self.view.on_error)

        self.process = QProcess(self)
        self.process.started.connect(self._on_process_started)
        self.process.finished.connect(self._on_process_finished)
        self.process.destroyed.connect(self._on_process_destroyed)
        self.process.errorOccurred.connect(self._on_process_error)

        self.data = {}
        self.manually_entered = False

        self.uploader = DataUploaderDialog(
            model=self.model,
            session=self.session,
            device_name=self.config.section_name,
            parent=self.view,
        )
        self.uploader.upload_successful.connect(self._upload_succeeded)
        self.uploader.upload_failed.connect(self._upload_failed)

    def start(self):
        self.logger.debug("start")
        self.started.emit()

    def measure(self):
        self.logger.debug("measure")
        self.measured.emit()

    def submit(self):
        saved = False
        if not self.detached:
            self.uploader.reset()
            self.uploader.show()
            self.uploader.start_upload()
        else:
            saved = self.save_to_file()
            if saved:
                self.submitted.emit()

    def save_to_file(self):
        try:
            file_path, selected_filter = QFileDialog.getSaveFileName(
                None,
                "Select output file",
                str(
                    (
                        Path.home() / "Documents" / f"{self.session.barcode}.json"
                    ).resolve()
                ),
                "JSON files (*.json)",
            )
            if file_path:
                with open(file_path, "w") as file:
                    json.dump(self.model.to_response(), file, indent=4)
            else:
                return False

        except Exception as e:
            self.logger.error(e)
            return False

        return True

    def _on_process_started(self, *args):
        self.started.emit()

    def _on_process_finished(self, *args):
        self.ready_to_measure.emit()

    def _on_process_destroyed(self, *args):
        self.error.emit()

    def _on_process_error(self, *args):
        self.error.emit()

    def _upload_succeeded(self):
        self.submitted.emit()

    def _upload_failed(self):
        self.error.emit("Error", "Failed to upload the data")

    @classmethod
    def class_name(cls):
        return cls.__name__
