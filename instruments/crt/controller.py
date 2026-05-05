from pathlib import Path
from datetime import date
from typing import override

from instruments.controller import Controller
from instruments.utils import is_process_running

from instruments.crt.model import CRTModel
from instruments.crt.config import CRTConfig
from instruments.crt.session import CRTSession


class CRTController(Controller):
    def __init__(
        self,
        session: CRTSession,
        config: CRTConfig,
        model: CRTModel,
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

    @override
    def start(self) -> bool:
        if is_process_running(self.config.process_name):
            self.error.emit("Error:", f"{self.config.process_name} is already open")
            return False

        if not self._clean_output_dir(self.config.output):
            self.error.emit("Error:", f"could not prepare instrument")
            return False

        self._prepare_process()
        self.process.start()

        return True

    @override
    def measure(self):
        result_filepath = self._find_result_file()
        if not result_filepath:
            self.error.emit("File not found", "could not find results")
            return

        if not self.model.read_results(result_filepath):
            self.error.emit("File not read", "could not read results")
            return

        self.measured.emit(self.model.to_response())

    def _clean_output_dir(self, output_dir: Path) -> bool:
        for path in output_dir.iterdir():
            if path.is_file():
                path.unlink(missing_ok=True)
        return True

    def _prepare_process(self) -> None:
        self.process.setProgram(str(self.config.executable.resolve()))
        self.process.setArguments(self._prepare_arguments())
        self.process.setWorkingDirectory(str(self.config.directory.resolve()))

    def _prepare_arguments(self) -> list[str]:
        arguments = [
            f"/i{self.session.interviewer}",
            f"/u{self.session.barcode}",
            f"/c{self.config.clinic}",
            f"/l{self.session.language.upper()[0]}",
        ]
        return arguments

    def _find_result_file(self) -> Path | None:
        results_filename = f"{self.config.prefix}_{self.config.clinic}_{date.today().strftime('%Y%m%d')}.csv"
        result_filepath = None
        for path in self.config.output.iterdir():
            if path.is_file():
                if path.name == results_filename:
                    result_filepath = path
        return result_filepath
