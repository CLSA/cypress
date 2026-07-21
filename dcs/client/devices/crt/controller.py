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

        self.start()

    @override
    def start(self) -> bool:
        self.logger.debug("CRTController::start")

        if is_process_running(self.config.process_name):
            self.error.emit(f"{self.config.process_name} is already open, please close and try again")
            return False

        if not self._clean_output_dir(self.config.output):
            self.error.emit(f"Could not prepare device")
            return False

        self._prepare_process()
        self.process.start()

        return True

    @override
    def measure(self):
        self.logger.debug("CRTController::measure")

        if not self.model.read_results():
            self.error.emit("Could not read results")
            return

        self.logger.debug(json.dumps(self.model.to_response(), indent=2))

        self.measured.emit(self.model.to_response())

    @override
    def submit(self):
        self.logger.debug("CRTController::submit")
        return super().submit()

    @override
    def _on_process_started(self, *args):
        self.logger.debug(f"CRTController::_on_process_started")
        self.started.emit()

    @override
    def _on_process_finished(self, *args):
        self.logger.debug(f"CRTController::_on_process_finished")
        self.ready_to_measure.emit()

    @override
    def _on_process_destroyed(self, *args):
        self.logger.debug(f"CRTController::_on_process_destroyed")
        self.error.emit("")

    @override
    def _on_process_error(self, *args):
        self.logger.debug(f"CRTController::_on_process_error")
        self.error.emit("")

    def _clean_output_dir(self, output_dir: Path) -> bool:
        self.logger.debug("CRTController::_clean_output_dir")

        for path in output_dir.iterdir():
            if path.is_file():
                path.unlink(missing_ok=True)
        return True

    def _prepare_process(self) -> None:
        self.logger.debug("CRTController::_prepare_process")

        self.process.setProgram(str(self.config.executable.resolve()))
        self.process.setArguments(self._prepare_arguments())
        self.process.setWorkingDirectory(str(self.config.directory.resolve()))

    def _prepare_arguments(self) -> list[str]:
        self.logger.debug("CRTController::_prepare_arguments")

        arguments = [
            f"/i{self.session.interviewer}",
            f"/u{self.session.barcode}",
            f"/c{self.config.clinic}",
            f"/l{self.session.language.upper()[0]}",
        ]

        self.logger.debug(arguments)
        return arguments

    def _create_backup_tar(self) -> str | None:
        try:
            backup_path = Path("./config.tar.gz")
            backup_path.unlink(missing_ok=True)
            with tarfile.open(backup_path, mode="x:gz") as backup_tar:
                backup_tar.add(self.config.directory, arcname="ccb")
            return str(backup_path.resolve())
        except Exception as e:
            self.logger.critical(e)
            return None