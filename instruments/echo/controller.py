from PySide6.QtCore import Signal

from dicom.receiver import FileReceiver, FileInfo, ReceiverConfig

from instruments.controller import Controller

from instruments.echo.session import ECHOSession
from instruments.echo.config import ECHOConfig
from instruments.echo.model import ECHOModel


class ECHOController(Controller):
    files_received = Signal(object)

    def __init__(
        self,
        session: ECHOSession,
        config: ECHOConfig,
        model: ECHOModel,
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
            ReceiverConfig(storage_dir=self.config.storage_path, extensions=[".dcm"])
        )

        self.file_receiver.files_received.connect(self._on_files_received)
        self.file_receiver.start()

    def _on_files_received(self, dicom_files: list[FileInfo]):
        self.files_received.emit(dicom_files)
