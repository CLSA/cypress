from PySide6.QtCore import Signal

from devices.controller import Controller

from typing import override

from dicom.receiver import FileReceiver, ReceiverConfig, DicomFileInfo

from devices.dxa.utils.validation import get_file_type
from devices.dxa.session import DXASession
from devices.dxa.config import DXAConfig
from devices.dxa.model import DXAModel


class DXAController(Controller):
    files_received = Signal(object)

    def __init__(
        self,
        session: DXASession,
        config: DXAConfig,
        model: DXAModel,
        detached=False,
        parent=None
    ):
        super().__init__(
            parent=parent,
            session=session,
            config=config,
            model=model,
            detached=detached
        )

        self.files: list[DicomFileInfo] = []
        self.file_receiver = FileReceiver(
            ReceiverConfig(storage_dir=self.config.storage_path, extensions=[".dcm"])
        )
        self.file_receiver.files_received.connect(self._on_files_received)
        self.file_receiver.start()

    @override
    def start(self):
        super().start()
        self.finished.emit()

    @override
    def measure(self):
        for dicom_file in self.files:
            dicom_file.name = get_file_type(dicom_file.file_path)
        self.files_received.emit(self.files)

    def _on_files_received(self, dicom_files: list[DicomFileInfo]):
        self.files = dicom_files
        self.files_received.emit(self.files)
