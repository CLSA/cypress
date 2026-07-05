from controller import Controller

from devices.blood_pressure.model import BPModel
from devices.blood_pressure.view import BPView
from devices.blood_pressure.config import BPConfig
from devices.blood_pressure.session import BPSession

from typing import override


class BPController(Controller):
    def __init__(
        self,
        session: BPSession,
        config: BPConfig,
        model: BPModel,
        view: BPView,
        detached: bool = False,
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
        self.logger.debug("BPController::__init__")

        self.view.measurement_table_widget.measureButton.setVisible(True)

    @override
    def start(self) -> bool:
        self.logger.debug("BPController::start")

        # self._prepare_process()
        self.started.emit()
        self.ready_to_measure.emit()

    @override
    def measure(self):
        self.logger.debug("BPController::measure")

        self.measured.emit(self.model.to_response())

    def _prepare_process(self) -> None:
        self.logger.debug("BPController::_prepare_process")

        self.process.setProgram(str(self.config.executable.resolve()))
        self.process.setWorkingDirectory(str(self.config.working_directory.resolve()))
