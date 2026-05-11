from PySide6.QtCore import Signal

from dicom.receiver import FileReceiver, FileInfo, ReceiverConfig
from dicom.uploader import FileUploaderDialog

from devices.controller import Controller

from devices.echo.session import ECHOSession
from devices.echo.config import ECHOConfig
from devices.echo.model import ECHOModel


class ECHOController(Controller):
    files_received = Signal(object)

    def __init__(
        self,
        session: ECHOSession,
        config: ECHOConfig,
        model: ECHOModel,
        detached=False,
        parent=None,
    ):
        super().__init__(
            parent=parent,
            session=session,
            config=config,
            model=model,
            detached=detached,
        )

        self.file_receiver = FileReceiver(
            ReceiverConfig(storage_dir=self.config.storage_path, extensions=[".dcm"])
        )

        self.file_receiver.files_received.connect(self._on_files_received)
        self.file_receiver.start()

        self.file_uploader = FileUploaderDialog()

    def _on_files_received(self, dicom_files: list[FileInfo]):
        self.files_received.emit(dicom_files)
