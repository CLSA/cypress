import json
import tarfile

from pathlib import Path
from typing import override

from controller import Controller
from utils import is_process_running

from devices.cdtt.model import CDTTModel
from devices.cdtt.view import CDTTView
from devices.cdtt.config import CDTTConfig
from devices.cdtt.session import CDTTSession


class CDTTController(Controller):
    def __init__(
        self,
        session: CDTTSession,
        config: CDTTConfig,
        model: CDTTModel,
        view: CDTTView,
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
            { "path": self.config.directory, "arcname": "CDTTStereo" }
        ]

    @override
    def start(self) -> bool:
        self.logger.info("start")

        if is_process_running(self.config.process_name):
            self.logger.error("process is already running")
            self.error.emit(
                f"{self.config.process_name} is already open, please close and try again",
            )
            return False

        self.logger.info("cleaning output directory")
        if not self._clean_output_dir(self.config.output):
            self._handle_error("Something went wrong", store_backup=True)
            return False

        self.logger.info("preparing settings")
        if not self._prepare_settings_files():
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

        success, error = self.model.read_output(
            self.config.output / f"Results-{self.session.barcode}.xlsx",
        )
        if not success:
            self.logger.error(error)
            self._handle_error("Something went wrong", store_backup=True)
            return False

        valid, error = self.model.is_valid()
        if not valid:
            self.logger.error(error)
            self._handle_error(error, store_backup=True)
            return False

        self.logger.debug(json.dumps(self.model.to_response(), indent=2))
        self.measured.emit(self.model.to_response())

        return True

    @override
    def submit(self):
        self.logger.info("submit")
        return super().submit()

    def _clean_output_dir(self, output_dir: Path) -> bool:
        self.logger.debug("_clean_output_dir")

        try:
            for path in output_dir.iterdir():
                if path.is_file() and path.name != "Results-Template.xlsx":
                    path.unlink()
        except Exception as e:
            self.logger.critical(e)
            return False

        return True

    def _prepare_settings_files(self):
        self.logger.debug("_prepare_settings_files")

        try:
            en_settings_file = self.config.en_settings
            fr_settings_file = self.config.fr_settings

            current_settings = Path(self.config.settings_dir / "Settings.xlsx")
            current_settings.unlink(missing_ok=True)

            if self.session.language == "en":
                en_settings_file.copy(current_settings)
            else:
                fr_settings_file.copy(current_settings)

        except Exception as e:
            self.logger.critical(e)
            return False

        return True

    def _prepare_process(self) -> bool:
        self.logger.debug("_prepare_process")
        try:
            self.process.setProgram(str(self.config.jre.resolve()))
            self.process.setArguments(["-jar", str(self.config.jar.resolve()), self.session.barcode])
            self.process.setWorkingDirectory(str(self.config.directory.resolve()))
        except Exception as e:
            self.logger.critical(e)
            return False

        return True