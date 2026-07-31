import json
import tarfile

from pathlib import Path
from typing import override

from controller import Controller
from utils import is_process_running

from devices.crt.model import CRTModel
from devices.crt.view import CRTView
from devices.crt.config import CRTConfig
from devices.crt.session import CRTSession


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

        self.backup_paths = [
            { "path": self.config.directory, "arcname": "ccb" }
        ]

    @override
    def start(self) -> bool:
        self.logger.info("starting")
        if is_process_running(self.config.process_name):
            self.logger.error("process is already running")
            self._handle_error(f"{self.config.process_name} is already open, please close and try again")
            return False

        self.logger.info("clearing output")
        if not self._clean_output_dir(self.config.output):
            self._handle_error("Something went wrong", store_backup=True)
            return False

        self.logger.info("preparing process")
        if not self._prepare_process():
            self._handle_error("Something went wrong", store_backup=True)
            return False

        self.logger.info("starting process")
        self.process.start()

        return True

    @override
    def measure(self):
        self.logger.info("measure")

        if not self.model.read_results():
            self._handle_error("Something went wrong", store_backup=True)
            return

        self.logger.debug(json.dumps(self.model.to_response(), indent=2))

        self.measured.emit(self.model.to_response())

    @override
    def submit(self):
        self.logger.info("submit")
        return super().submit()

    def _clean_output_dir(self, output_dir: Path) -> bool:
        self.logger.debug("_clean_output_dir")

        try:
            for path in output_dir.iterdir():
                if path.is_file():
                    path.unlink(missing_ok=True)

        except Exception as e:
            self.logger.critical(e)
            return False

        return True

    def _prepare_process(self) -> None:
        self.logger.debug("_prepare_process")

        try:
            self.process.setProgram(str(self.config.executable.resolve()))
            self.process.setArguments(self._prepare_arguments())
            self.process.setWorkingDirectory(str(self.config.directory.resolve()))
        except Exception as e:
            self.logger.critical(e)
            return False
        return True

    def _prepare_arguments(self) -> list[str]:
        self.logger.debug("_prepare_arguments")

        arguments = [
            f"/i{self.session.interviewer}",
            f"/u{self.session.barcode}",
            f"/c{self.config.clinic}",
            f"/l{self.session.language.upper()[0]}",
        ]

        self.logger.debug(arguments)

        return arguments

