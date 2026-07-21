import json
import tarfile

from pathlib import Path
from typing import override

from controller import Controller

from devices.frax.model import FRAXModel
from devices.frax.view import FRAXView
from devices.frax.session import FRAXSession
from devices.frax.config import FRAXConfig


class FRAXController(Controller):
    def __init__(
        self,
        session: FRAXSession,
        config: FRAXConfig,
        model: FRAXModel,
        view: FRAXView,
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
    def start(self):
        self.logger.debug("FRAXController::start")
        if not self._clean():
            self.error.emit("Could not prepare instrument")
            return False

        if not self.model.write_input_file(self.config.input_file):
            self.error.emit("Could not write input file")
            return False

        self.measure()

        return True

    @override
    def measure(self):
        if not self.model.run(
            executable=self.config.executable, working_directory=self.config.directory
        ):
            self.error.emit("Could not run FRAX")
            return

        if not self.model.read_output_file(self.config.output_file):
            self.error.emit("Could not read output file")
            return

        self.logger.debug(json.dumps(self.model.to_response(), indent=2))
        self.measured.emit(self.model.to_response())

    def _clean(self) -> bool:
        Path(self.config.input_file).unlink(missing_ok=True)
        Path(self.config.output_file).unlink(missing_ok=True)
        return True

    def _create_backup_tar(self) -> str | None:
        try:
            backup_path = Path("./frax_backup.tar.gz")
            backup_path.unlink(missing_ok=True)
            with tarfile.open(backup_path, mode="x:gz") as backup_tar:
                backup_tar.add(self.config.directory, arcname="frax")
            return str(backup_path.resolve())
        except Exception as e:
            self.logger.critical(e)
            return None
