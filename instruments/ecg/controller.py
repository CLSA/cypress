from PySide6.QtCore import Signal
from instruments.controller import Controller

from dicom.receiver import FileReceiver, FileInfo, ReceiverConfig

from instruments.ecg.config import ECGConfig
from instruments.ecg.session import ECGSession
from instruments.ecg.model import ECGModel


class ECGController(Controller):
    files_received = Signal(object)

    def __init__(
        self,
        session: ECGSession,
        config: ECGConfig,
        model: ECGModel,
        standalone=False,
        parent=None,
    ):
        super().__init__(
            parent=parent,
            session=session,
            config=config,
            model=model,
            standalone=standalone,
        )

        self.file_receiver = FileReceiver(
            ReceiverConfig(
                storage_dir=self.config.storage_path,
                extensions=[".ecg", ".xml", ".pdf"],
            )
        )
        self.file_receiver.files_received.connect(self._on_files_received)
        self.file_receiver.start()

    def _on_files_received(self, files: list[FileInfo]):
        self.files_received.emit(files)
