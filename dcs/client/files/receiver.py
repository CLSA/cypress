import logging

from typing import override
from dataclasses import dataclass

from pathlib import Path

from PySide6.QtCore import QObject, Signal, QFileSystemWatcher, QProcess, QTimer

from utils import stop_process, is_process_running


@dataclass(frozen=True, kw_only=True)
class ReceiverConfig:
    # path to where files are received
    storage_dir: Path

    # which files to look for
    extensions: list[str]


@dataclass(frozen=True, kw_only=True)
class DicomReceiverConfig(ReceiverConfig):
    # path to storescp.exe
    executable_path: Path

    # path to storescp.cfg
    config_path: Path

    # path to logger.cfg
    logger_path: Path

    # name of server
    ae_title: str

    # interface to bind
    host: str

    # port to bind
    port: str


class FileReceiver(QObject):
    files_received = Signal(list)
    started = Signal
    finished = Signal

    def __init__(self, config: ReceiverConfig, parent=None):
        super().__init__(parent)

        self.config = config
        self.directory_listener = QFileSystemWatcher()

        # Prevents checking everytime the directory changes
        self.debounce_timer = QTimer(self)
        self.debounce_timer.setSingleShot(True)
        self.debounce_timer.setInterval(250)  # ms

        self.directory_listener.directoryChanged.connect(self._on_directory_changed)
        self.debounce_timer.timeout.connect(self._check_files)

        # Checks on an interval that all received files are finished uploading
        self.processed_timer = QTimer(self)
        self.processed_timer.setInterval(500)
        self.processed_timer.setSingleShot(False)
        self.processed_timer.timeout.connect(self._check_stability)

        # Tracks received files and their current size
        self.pending_files: dict[Path, int] = {}

        # Tracks received files that have a stable size (i.e finished uploading)
        self.processed_files: set[Path] = set()

    def start(self) -> bool:
        return self.directory_listener.addPath(str(self.config.storage_dir.resolve()))

    def stop(self) -> bool:
        return self.directory_listener.removePath(
            str(self.config.storage_dir.resolve())
        )

    def close(self) -> bool:
        return self.stop()

    def _on_directory_changed(self) -> None:
        print("directory changed")
        self.debounce_timer.start()

    def _check_files(self) -> None:
        print("check files")
        for path in self.config.storage_dir.iterdir():
            if path.suffix not in self.config.extensions:
                continue

            if path in self.processed_files:
                continue

            try:
                size = path.stat().st_size
            except (FileNotFoundError, PermissionError):
                continue

            self.pending_files[path] = size

        if self.pending_files and not self.processed_timer.isActive():
            self.processed_timer.start()

    def _check_stability(self):
        print("checking stability")
        for path, previous_size in list(self.pending_files.items()):
            try:
                current_size = path.stat().st_size
            except (FileNotFoundError, PermissionError):
                del self.pending_files[path]
                continue

            if current_size == previous_size:
                del self.pending_files[path]
                self.processed_files.add(path)
            else:
                self.pending_files[path] = current_size

        if not self.pending_files:
            self.processed_timer.stop()
            self._process_files()

    def _process_files(self):
        self.files_received.emit(list(self.processed_files))


class DicomReceiver(FileReceiver):
    files_received = Signal(object)

    started = Signal
    finished = Signal

    def __init__(self, config: DicomReceiverConfig, logger, parent=None):
        super().__init__(config, parent)

        self.logger = logger

        if is_process_running("storescp.exe"):
            stopped = stop_process("storescp.exe")
            if not stopped:
                self.logger.error("could not stop storescp.exe")

        self.dicom_process = QProcess(self)
        self.dicom_process.started.connect(self._on_process_started)
        self.dicom_process.finished.connect(self._on_process_finished)
        self.dicom_process.destroyed.connect(self._on_process_destroyed)
        self.dicom_process.errorOccurred.connect(self._on_process_error)

        arguments = [
            str(self.config.port),
            "--config-file",
            str(self.config.config_path.resolve()),
            "default",
            "--log-config",
            str(self.config.logger_path.resolve()),
            "--aetitle",
            str(self.config.ae_title),
            "--output-directory",
            str(self.config.storage_dir.resolve()),
            "--filename-extension",
            str(self.config.extensions[0]),
        ]

        self.dicom_process.setProgram(str(self.config.executable_path))
        self.dicom_process.setArguments(arguments)

    @override
    def start(self) -> bool:
        self.logger.info("dicom receiver start")

        if not super().start():
            self.logger.error("could not start directory watcher")
            return False

        self.dicom_process.start()
        return self.dicom_process.waitForStarted(msecs=5000)

    @override
    def stop(self):
        self.logger.info("dicom receiver stop")

        if not super().stop():
            self.logger.error("could not stop directory watcher")
            return False

        self.dicom_process.close()
        return self.dicom_process.waitForFinished(msecs=5000)

    def _on_process_started(self):
        self.logger.info("dicom server process started")

    def _on_process_finished(self):
        self.logger.info("dicom server process finished")
        self.process = None

    def _on_process_destroyed(self):
        self.logger.info("dicom server process destroyed")
        self.process = None

    def _on_process_error(self, error):
        self.logger.error(f"dicom server process error: {error}")
