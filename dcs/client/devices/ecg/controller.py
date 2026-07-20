from typing import override
from PySide6.QtCore import Signal
from controller import Controller

from files.receiver import FileReceiver, FileInfo, ReceiverConfig

from devices.ecg.config import ECGConfig
from devices.ecg.session import ECGSession
from devices.ecg.model import ECGModel
from devices.ecg.view import ECGView


class ECGController(Controller):
    files_received = Signal(object)

    def __init__(
        self,
        session: ECGSession,
        config: ECGConfig,
        model: ECGModel,
        view: ECGView,
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
            ReceiverConfig(
                storage_dir=self.config.storage_path,
                extensions=[".pdf", ".xml", ".ecg"],
            )
        )
        self.file_receiver.files_received.connect(self._on_files_received)
        self.file_receiver.start()

        self.view.measure_button.setVisible(True)

        self.start()

    def _on_files_received(self, files: list[FileInfo]):
        self.ready_to_measure.emit()

    @override
    def measure(self):
        success, error = self.model.read_results(self.config.storage_path)
        if not success:
            self.logger.error(error)
            self.error.emit("error", error)
            return

        for ecg_file in self.model.files:
            print(ecg_file)

        self.measured.emit(self.model.to_response())