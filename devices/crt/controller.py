from pathlib import Path
from datetime import date
from typing import override

from devices.controller import Controller
from devices.utils import is_process_running

from devices.crt.model import CRTModel
from devices.crt.config import CRTConfig
from devices.crt.session import CRTSession

from devices.crt.settings import logger

from config import config

class CRTController(Controller):
    def __init__(
        self,
        session: CRTSession,
        config: CRTConfig,
        model: CRTModel,
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

    @override
    def start(self) -> bool:
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

        return True

    @override
    def measure(self):
        logger.info("measure")

        result_filepath = self._find_result_file()
        if not result_filepath:
            logger.error("result filepath not found")
            self.error.emit("File not found", "could not find results")
            return

        if not self.model.read_results(result_filepath):
            logger.error("result file could not be read")
            self.error.emit("File not read", "could not read results")
            return

        self.measured.emit(self.model.to_response())

    @override
    def submit(self):
        logger.info("submit")
        logger.debug(config.pine)
        return super().submit()

    @override
    def _on_process_started(self, *args):
        logger.info("process started")
        self.started.emit()

    @override
    def _on_process_finished(self, *args):
        logger.info("process finished")
        self.finished.emit()

    @override
    def _on_process_destroyed(self, *args):
        logger.debug("process destroyed", args)
        self.error.emit()

    @override
    def _on_process_error(self, *args):
        logger.error("process error", args)
        self.error.emit()

    def _clean_output_dir(self, output_dir: Path) -> bool:
        logger.debug("_clean_output_dir")

        for path in output_dir.iterdir():
            if path.is_file():
                path.unlink(missing_ok=True)
        return True

    def _prepare_process(self) -> None:
        logger.debug("_prepare_process")

        self.process.setProgram(str(self.config.executable.resolve()))
        self.process.setArguments(self._prepare_arguments())
        self.process.setWorkingDirectory(str(self.config.directory.resolve()))

    def _prepare_arguments(self) -> list[str]:
        logger.debug("_prepare_arguments")
        arguments = [
            f"/i{self.session.interviewer}",
            f"/u{self.session.barcode}",
            f"/c{self.config.clinic}",
            f"/l{self.session.language.upper()[0]}",
        ]
        logger.debug(arguments)
        return arguments

    def _find_result_file(self) -> Path | None:
        logger.debug("_find_result_file")

        results_filename = f"{self.config.prefix}_{self.config.clinic}_{date.today().strftime('%Y%m%d')}.csv"
        result_filepath = None
        for path in self.config.output.iterdir():
            if path.is_file():
                if path.name == results_filename:
                    result_filepath = path

        logger.debug(result_filepath)

        return result_filepath
