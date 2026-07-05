from typing import override
from dataclasses import dataclass
from pathlib import Path

from PySide6.QtCore import QObject, Signal, QFileSystemWatcher, QProcess

from utils import FileInfo, DicomFileInfo, get_file_size, clear_directory


@dataclass(frozen=True, kw_only=True)
class ReceiverConfig:
    # path to where files are received
    storage_dir: Path

    # which files to look for
    extensions: list[str]






class FileReceiver(QObject):
    files_received = Signal(object)
    started = Signal
    finished = Signal

    def __init__(self, config: ReceiverConfig, parent = None):
        super().__init__(parent)
        self.config = config

        self._clear_files()
        self._configure_listener()

    def _clear_files(self) -> bool:
        try:
            clear_directory(self.config.storage_dir)
        except ValueError as e:
            print(e)

    def start(self):
        self.directory_listener.addPath(str(self.config.storage_dir.resolve()))

    def stop(self):
        self.directory_listener.removePath(str(self.config.storage_dir.resolve()))

    def _configure_listener(self) -> None:
        self.directory_listener = QFileSystemWatcher()
        self.directory_listener.directoryChanged.connect(self._on_directory_changed)

    def _on_directory_changed(self) -> None:
        files: list[FileInfo] = []
        for file_path in self.config.storage_dir.iterdir():
            if file_path.is_file() and file_path.exists() and file_path.suffix in self.config.extensions:
                file_info = FileInfo(
                    name=file_path.stem,
                    size=get_file_size(file_path),
                    file_name=file_path.name,
                    extension=file_path.suffix,
                    file_path=file_path
                )
                files.append(file_info)

        self.files_received.emit(files)


@dataclass(frozen=True, kw_only=True)
class DicomReceiverConfig(ReceiverConfig):
    # path to storescp.exe
    executable_path: Path

    # path to storescp.exe config
    config_path: Path

    # name of server
    ae_title: str

    # interface to bind
    host: str

    # port to bind
    port: str

class DicomReceiver(FileReceiver):
    files_received = Signal(object)

    started = Signal
    finished = Signal

    def __init__(self, config: DicomReceiverConfig, parent=None):
        super().__init__(config, parent)
        self._configure_server()

    def close(self):
        self.dicom_process.close()

    def _configure_server(self) -> None:
        self.dicom_process = QProcess(self)
        self.dicom_process.started.connect(self._on_process_started)
        self.dicom_process.finished.connect(self._on_process_finished)
        self.dicom_process.destroyed.connect(self._on_process_destroyed)
        self.dicom_process.errorOccurred.connect(self._on_process_error)

        arguments = [
            str(self.config.port),
            "--config-file", str(self.config.config_path.resolve()), "default",
            "--aetitle", str(self.config.ae_title),
            "--output-directory", str(self.config.storage_dir.resolve()),
            "--filename-extension", str(self.config.extensions[0])
        ]

        self.dicom_process.setProgram(str(self.config.executable_path))
        self.dicom_process.setArguments(arguments)
        self.dicom_process.start()

    @override
    def _on_directory_changed(self) -> None:
        files = []
        for file_path in self.config.storage_dir.iterdir():
            if file_path.is_file() and file_path.suffix == '.dcm':
                file_info = FileInfo(
                    name=file_path.stem,
                    raw_size=file_path.stat().st_size,
                    readable_size=get_file_size(file_path),
                    file_name=file_path.name,
                    file_path=file_path,
                    extension=file_path.suffix,
                    send_name=file_path.name
                )
                files.append(file_info)
        self.files_received.emit(files)

    def _on_process_started(self):
        print("dicom: process started")

    def _on_process_finished(self):
        print("dicom: finished")

    def _on_process_destroyed(self):
        print("dicom: destroyed")

    def _on_process_error(self, error):
        print("dicom: ", error)

