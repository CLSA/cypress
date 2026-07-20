from PySide6.QtCore import Signal

from typing import override

from files.receiver import DicomReceiver, DicomReceiverConfig

from utils import DicomFileInfo

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

        self.files: list[DicomFileInfo] = []
        self.file_receiver = DicomReceiver(
            DicomReceiverConfig(
                executable_path=self.config.executable,
                ae_title=self.config.ae_title,
                host=self.config.host,
                port=self.config.port,
                storage_dir=self.config.storage_path,
                config_path=self.config.storescp_config,
                extensions=[".dcm"],
            )
        )
        self.file_receiver.files_received.connect(self._on_files_received)
        self.file_receiver.start()

        self.view.measure_button.setVisible(True)

        self.view.close.connect(self.close)
        self.view.destroyed.connect(self.close)

        self.view.measure_button.setVisible(True)
        self.start()

    @override
    def close(self):
        self.file_receiver.close()

    def _on_files_received(self, files: list[DicomFileInfo]):
        self.ready_to_measure.emit()

        file_count = sum(1 for path in self.config.storage_path.iterdir() if path.is_file())
        print(file_count)

    @override
    def measure(self):
        success, error = self.model.read_results(self.config.storage_path)
        if not success:
            self.logger.error(error)
            self.error.emit("error", error)
            return

        self.measured.emit(self.model.to_response())


