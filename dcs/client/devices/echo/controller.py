from PySide6.QtCore import Signal

from typing import override

from files.receiver import FileReceiver, FileInfo, ReceiverConfig

from controller import Controller

from devices.echo.session import ECHOSession
from devices.echo.config import ECHOConfig
from devices.echo.model import ECHOModel
from devices.echo.view import ECHOView


class ECHOController(Controller):
    files_received = Signal(object)

    def __init__(
        self,
        session: ECHOSession,
        config: ECHOConfig,
        model: ECHOModel,
        view: ECHOView,
        detached=False,
        parent=None,
    ):
        super().__init__(
            parent=parent,
            session=session,
            config=config,
            model=model,
            view=view,
            detached=detached,
        )

        self.file_receiver = FileReceiver(
            ReceiverConfig(storage_dir=self.config.storage_path, extensions=[".dcm"])
        )

        self.file_receiver.files_received.connect(self._on_files_received)
        self.file_receiver.start()
        self.files_received.connect(self.view.on_files_received)

        self.view.measure_button.setVisible(True)

    @override
    def close(self):
        #self.file_receiver.close()
        pass

    def _on_files_received(self, dicom_files: list[FileInfo]):
        self.files_received.emit(dicom_files)
