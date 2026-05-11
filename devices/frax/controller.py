from pathlib import Path
from typing import override

from devices.controller import Controller

from devices.frax.model import FRAXModel
from devices.frax.session import FRAXSession
from devices.frax.config import FRAXConfig


class FRAXController(Controller):
    def __init__(
        self,
        session: FRAXSession,
        config: FRAXConfig,
        model: FRAXModel,
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
    def start(self):
        self.started.emit()

        if not self._clean():
            self.error.emit("error", "could not prepare instrument")
            return False

        if not self.model.write_input_file(self.config.input_file):
            self.error.emit("error", "could not write input file")
            return False

        if not self.model.run(
            executable=self.config.executable, working_directory=self.config.directory
        ):
            self.error.emit("error", "could not run FRAX")
            return False

        self.finished.emit()

        return True

    @override
    def measure(self):
        if not self.model.read_output_file(self.config.output_file):
            self.error.emit("error", "could not read output file")
            return

        self.measured.emit(self.model.to_response())

    def _clean(self) -> bool:
        Path(self.config.input_file).unlink(missing_ok=True)
        Path(self.config.output_file).unlink(missing_ok=True)
        return True
