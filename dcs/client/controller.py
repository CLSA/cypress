import logging
import tarfile

from datetime import datetime
from typing import Literal

from abc import abstractmethod
from pathlib import Path
from copy import deepcopy

from PySide6.QtCore import QObject, QProcess, Signal, QThread
from PySide6.QtWidgets import QFileDialog, QMessageBox

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
    ready_to_submit = Signal(bool)
    submitting = Signal()
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
        self.ready_to_submit.connect(self.view.on_ready_to_submit)
        self.submitting.connect(self.view.on_submitting)
        self.submitted.connect(self.view.on_submitted)
        self.error.connect(self.view.on_error)

        self.process = QProcess(self)
        self.process.started.connect(self._on_process_started)
        self.process.finished.connect(self._on_process_finished)
        self.process.destroyed.connect(self._on_process_destroyed)
        self.process.errorOccurred.connect(self._on_process_error)

        self.data = {}
        self.manually_entered = False

        self.backup_paths = []
        self.backup = None

    def start(self):
        self.logger.debug("start")
        self.started.emit()

    def measure(self):
        self.logger.debug("measure")
        self.measured.emit()

    def submit(self):
        self.submitting.emit()

        if self.detached:
            if self.save_to_file():
                self.restore()
                self.submitted.emit()
                self.logger.info("submitted")
        else:
            self.upload_to_server()

    def restore(self):
        self.logger.info("restore device")

    def upload_to_server(self):
        self.logger.debug("upload_to_server")
        try:
            self.files_to_transfer = deepcopy(self.model.files)
            backup_path = self._create_backup_tar(
                directory=Path.cwd(), filename="config.tar.gz"
            )
            if backup_path:
                backup_file_info = get_file_info(Path(backup_path))
                backup_file_info.send_name = "config"
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
        except Exception as e:
            self.logger.error(e)
            self._handle_error()

    def upload_finished(self, successful: bool):
        self.logger.debug(f"upload finished: {"Yes" if successful else "No"}")
        self.thread.quit()

        if successful:
            self.logger.info("submitted")
            self.restore()
            self.submitted.emit()
        else:
            self.logger.info("failed to submit")

    def upload_status(self, file_index, file_name, percentage):
        self.view.set_status(
            f"Uploading {file_index} / {len(self.files_to_transfer)} ({percentage}%) "
        )

    def save_to_file(self):
        try:
            file_path, selected_filter = QFileDialog.getSaveFileName(
                None,
                "Select output file",
                str(
                    (
                        Path.home()
                        / "Documents"
                        / f"{self.session.barcode}_{datetime.now().strftime("%Y%m%d%H%M%S")}.zip"
                    ).resolve()
                ),
                "ZIP files (*.zip)",
            )
            if file_path:
                self.model.to_zip(file_path)
            else:
                return False

        except Exception as e:
            print(e)
            self.logger.error(e)
            return False

        return True

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

    def _handle_error(
        self, msg: str = "Something went wrong", store_backup: bool = False
    ) -> None:
        self.error.emit(msg)
        if store_backup:
            self._store_backup()

    def _show_message_box(
        self,
        title: str,
        msg: str,
        level: Literal["info", "warning", "critical"],
        allow_cancel=False,
    ):
        if level == "info":
            return QMessageBox.information(None, title, msg)
        elif level == "warning":
            return QMessageBox.warning(
                None,
                title,
                msg,
                buttons=(
                    QMessageBox.StandardButton.Ok | QMessageBox.StandardButton.Cancel
                    if allow_cancel
                    else QMessageBox.StandardButton.Ok
                ),
                defaultButton=(
                    QMessageBox.StandardButton.Cancel
                    if allow_cancel
                    else QMessageBox.StandardButton.Ok
                ),
            )
        elif level == "critical":
            return QMessageBox.critical(None, title, msg)
        else:
            self.logger.warning(f"failed to show message box: incorrect level {level}")

    def _store_backup(self):
        backup_path = (
            Path.cwd()
            / "errors"
            / self.session.barcode
            / self.config.section_name
            / self.session.session_id
        )

        backup_path.mkdir(parents=True, exist_ok=True)

        store_path = self._create_backup_tar(
            directory=backup_path, filename="backup.tar.gz"
        )
        if not store_path:
            self.logger.error("failed to store error backup")
        else:
            self.logger.info(f"stored backup at {store_path}")

    def _create_backup_tar(self, directory: Path, filename: str) -> str | None:
        try:
            (directory / filename).unlink(missing_ok=True)

            with tarfile.open(directory / filename, mode="x:gz") as backup_tar:
                for backup_path in self.backup_paths:
                    if not backup_path["path"].exists():
                        self.logger.warning(
                            f"{backup_path["path"]} does not exist.. could not add to backup"
                        )
                        continue
                    backup_tar.add(backup_path["path"], arcname=backup_path["arcname"])
            return str((directory / filename).resolve())
        except Exception as e:
            self.logger.critical(e)
            return None

    @classmethod
    def class_name(cls):
        return cls.__name__
