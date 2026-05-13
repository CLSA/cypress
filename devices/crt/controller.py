from pathlib import Path
from typing import override

from devices.controller import Controller
from devices.utils import is_process_running

from devices.crt.model import CRTModel
from devices.crt.view import CRTView
from devices.crt.config import CRTConfig
from devices.crt.session import CRTSession

from devices.crt.settings import logger


class CRTController(Controller):
    def __init__(
        self,
        session: CRTSession,
        config: CRTConfig,
        model: CRTModel,
        view: CRTView,
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

    @override
    def start(self) -> bool:
        print("start")
        logger.info("start")

        if is_process_running(self.config.process_name):
            msg = f"{self.config.process_name} is already open"
            logger.error(msg)
            self.error.emit("Error", msg)
            return False

        if not self._clean_output_dir(self.config.output):
            msg = f"could not prepare instrument"
            logger.error(msg)
            self.error.emit("Error", msg)
            return False

        self._prepare_process()
        self.process.start()

    @override
    def measure(self):
        logger.info("measure")

        if not self.model.read_results():
            logger.error("result file could not be read")
            self.error.emit("File not read", "could not read results")
            return

        self.measured.emit(self.model.to_response())

    @override
    def submit(self):
        logger.info("submit")
        return super().submit()

    @override
    def _on_process_started(self, *args):
        logger.info(f"{self.config.process_name} started")
        self.started.emit()

    @override
    def _on_process_finished(self, *args):
        logger.info(f"{self.config.process_name} closed")
        self.ready_to_measure.emit()

    @override
    def _on_process_destroyed(self, *args):
        logger.info(f"{self.config.process_name} destroyed")
        self.error.emit()

    @override
    def _on_process_error(self, *args):
        logger.info(f"{self.config.process_name} error")
        self.error.emit()

    def _clean_output_dir(self, output_dir: Path) -> bool:
        logger.debug("CRTController::_clean_output_dir")

        for path in output_dir.iterdir():
            if path.is_file():
                path.unlink(missing_ok=True)
        return True

    def _prepare_process(self) -> None:
        logger.debug("CRTController::_prepare_process")

        self.process.setProgram(str(self.config.executable.resolve()))
        self.process.setArguments(self._prepare_arguments())
        self.process.setWorkingDirectory(str(self.config.directory.resolve()))

    def _prepare_arguments(self) -> list[str]:
        logger.debug("CRTController::_prepare_arguments")

        arguments = [
            f"/i{self.session.interviewer}",
            f"/u{self.session.barcode}",
            f"/c{self.config.clinic}",
            f"/l{self.session.language.upper()[0]}",
        ]

        logger.debug(arguments)

        return arguments


