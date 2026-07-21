import logging

from datetime import datetime

from abc import abstractmethod
from pathlib import Path
from copy import deepcopy

from PySide6.QtCore import QObject, QProcess, Signal, QThread
from PySide6.QtWidgets import QFileDialog

from session import Session
from config import DeviceConfig

from model import Model
from view import View

from files.uploader import PineAPI
from utils import get_file_info, FileInfo


class Controller(QObject):
    started = Signal()
    ready_to_measure = Signal()
    measured = Signal(dict)
    submitted = Signal()
    error = Signal(str)

    start_upload = Signal()

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

        self.backup = None

    def start(self):
        self.logger.debug("start")
        self.started.emit()

    def measure(self):
        self.logger.debug("measure")
        self.measured.emit()

    def submit(self):
        if self.detached:
            if self.save_to_file():
                self.submitted.emit()
        else:
            self.upload_to_server()

    def upload_to_server(self):
        self.files_to_transfer = deepcopy(self.model.files)
        backup_path = self._create_backup_tar()
        if backup_path:
            backup_file_info = get_file_info(Path(backup_path))
            backup_file_info.send_name = "backup"
            self.files_to_transfer.append(backup_file_info)

        self.api = PineAPI(
            base_url=self.session.origin,
            logger=self.logger,
            session=self.session,
            json=self.model.to_response(),
            files=self.files_to_transfer,
        )

        self.start_upload.connect(self.api.send_device_response)
        self.api.progress_updated.connect(self.upload_status)
        self.api.all_finished.connect(self.upload_finished)

        self.thread = QThread()
        self.thread.finished.connect(self.thread.deleteLater)

        self.api.moveToThread(self.thread)
        self.thread.start()

        self.start_upload.emit()

    def upload_finished(self, successful: bool):
        self.logger.debug(f"upload finished: {"Yes" if successful else "No"}")
        self.thread.quit()

        if successful:
            self.submitted.emit()

    def upload_status(self, file_index, file_name, percentage):
        self.view.set_status(
            f"Uploading ({percentage}%) {file_name} ({file_index} / {len(self.files_to_transfer)})"
        )

    def save_to_file(self):
        try:
            file_path, selected_filter = QFileDialog.getSaveFileName(
                None,
                "Select output file",
                str(
                    (
                        Path.home() / "Documents" / f"{self.session.barcode}_{datetime.now().strftime("%Y%m%d%H%M%S")}.zip"
                    ).resolve()
                ),
                "ZIP files (*.zip)",
            )
            if file_path:
                self.model.to_zip(file_path)
            else:
                return False

        except Exception as e:
            self.logger.error(e)
            return False

        return True

    @abstractmethod
    def _create_backup_tar(self) -> FileInfo | None:
        pass

    def _on_process_started(self, *args):
        self.started.emit()

    def _on_process_finished(self, *args):
        self.ready_to_measure.emit()

    def _on_process_destroyed(self, *args):
        self.error.emit("")

    def _on_process_error(self, *args):
        self.error.emit("")

    def _upload_succeeded(self):
        self.submitted.emit()

    def _upload_failed(self):
        self.error.emit("Failed to upload the data")

    @classmethod
    def class_name(cls):
        return cls.__name__
