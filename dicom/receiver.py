from PySide6.QtCore import QObject, QProcess, Signal, QFileSystemWatcher

from dataclasses import dataclass
from pathlib import Path

from typing import override, List

from devices.utils import get_file_size


@dataclass(kw_only=True)
class FileInfo:
    name: str
    size: str
    file_name: str
    file_path: Path
    extension: str


@dataclass(kw_only=True)
class DicomFileInfo(FileInfo):
    patient_id: str
    body_part_examined: str
    modality: str
    laterality: str
    study_id: str
    media_storage_uid: str
    study_date: str
    series_number: str
    instance_number: str


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

    # path to storescp.exe config
    config_path: Path

    # name of server
    ae_title: str

    # interface to bind
    host: str

    # port to bind
    port: str



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
        if not self.config.storage_dir.exists():
            print(f'error: {str(self.config.storage_dir.resolve())} does not exist')
            return False

        if not self.config.storage_dir.is_dir():
            print(f'error: {str(self.config.storage_dir.resolve())} is not a directory')
            return False

        for path in self.config.storage_dir.iterdir():
            if path.is_file():
                path.unlink(missing_ok=True)

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


class DicomReceiver(FileReceiver):
    files_received = Signal(object)

    started = Signal
    finished = Signal

    def __init__(self, config, parent=None):
        super().__init__(config, parent)

        self._configure_server()

    def _configure_server(self) -> None:
        print('configure server..')
        pass

    @override
    def _on_directory_changed(self) -> None:
        print("directory changed..")
        files = []
        for file_path in self.config.storage_dir.iterdir():
            if file_path.is_file() and file_path.suffix == '.dcm':
                file_info = DicomFileInfo(
                    name=file_path.stem,
                    size=get_file_size(file_path),
                    file_name=file_path.name,
                    extension=file_path.suffix,
                    file_path=file_path
                )
                files.append(file_info)

        self.files_received.emit(files)


