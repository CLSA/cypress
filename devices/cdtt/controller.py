import json

from pathlib import Path
from typing import override

from instruments.controller import Controller
from instruments.utils import is_process_running

from instruments.cdtt.model import CDTTModel
from instruments.cdtt.config import CDTTConfig
from instruments.cdtt.session import CDTTSession


class CDTTController(Controller):
    def __init__(
        self,
        session: CDTTSession,
        config: CDTTConfig,
        model: CDTTModel,
        standalone=False,
        parent=None,
    ):
        super().__init__(
            session=session,
            config=config,
            model=model,
            standalone=standalone,
            parent=parent
        )

    @override
    def start(self) -> bool:
        if is_process_running(self.config.process_name):
            self.error.emit("Error:", f"{self.config.process_name} is already open")
            return False

        if not self._clean_output_dir(self.config.output):
            self.error.emit("Error:", f"could not prepare instrument")
            return False

        self._prepare_process(
            str(self.config.directory.resolve()),
            str(self.config.jre.resolve()),
            str(self.config.jar.resolve()),
            self.session.barcode,
        )

        self.process.start()

        return True

    def _prepare_process(self, directory: str, jre: str, jar: str, barcode: str):
        self.process.setProgram(jre)
        self.process.setArguments(["-jar", jar, barcode])
        self.process.setWorkingDirectory(directory)

    @override
    def measure(self) -> dict:
        print("CDTT measure..")
        if not self.model.read_output(
            self.config.output / f"Results-{self.session.barcode}.xlsx",
            self.session.language,
        ):
            self.error.emit("File not read", "error parsing CDTT results")
            return

        if not self.model.is_valid(self.session.barcode):
            self.error.emit(
                "Invalid results", "the results are invalid and cannot be saved"
            )
            return

        print(json.dumps(self.model.get_response(), indent=2))
        self.measured.emit(self.model.get_response())

    def _clean_output_dir(self, output_dir: Path) -> bool:
        for path in output_dir.iterdir():
            if path.is_file() and path.name != "Results-Template.xlsx":
                try:
                    path.unlink()
                except Exception as e:
                    print(f"error: {e}")
                    return False
        return True
