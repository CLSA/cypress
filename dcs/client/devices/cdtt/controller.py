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

        self.start()

    @override
    def start(self) -> bool:
        self.logger.debug("CDTTController::start")

        if is_process_running(self.config.process_name):
            self.error.emit(
                f"{self.config.process_name} is already open, please close and try again",
            )
            return False

        if not self._clean_output_dir(self.config.output):
            self.error.emit(f"Could not prepare device")
            return False

        if not self._prepare_settings_files():
            self.error.emit(f"Could not prepare settings")
            return False

        self._prepare_process(
            str(self.config.directory.resolve()),
            str(self.config.jre.resolve()),
            str(self.config.jar.resolve()),
            self.session.barcode,
        )

        self.process.start()

        return True

    @override
    def measure(self):
        self.logger.debug("CDTTController::measure")

        if not self.model.read_output(
            self.config.output / f"Results-{self.session.barcode}.xlsx",
            self.session.language,
        ):
            self.error.emit("Failed to read CDTT results")
            return False

        if not self.model.is_valid(self.session.barcode):
            self.error.emit("The results are invalid and cannot be saved")
            return False

        self.logger.debug(json.dumps(self.model.to_response(), indent=2))
        self.measured.emit(self.model.to_response())

        return True

    @override
    def submit(self):
        self.logger.debug("CDTTController::submit")
        return super().submit()

    def _clean_output_dir(self, output_dir: Path) -> bool:
        self.logger.debug("CDTTController::_clean_output_dir")
        for path in output_dir.iterdir():
            if path.is_file() and path.name != "Results-Template.xlsx":
                try:
                    path.unlink()
                except Exception as e:
                    self.logger.error(e)
                    return False
        return True

    def _prepare_settings_files(self):
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
            self.logger.error(e)
            return False

        return True

    def _prepare_process(self, directory: str, jre: str, jar: str, barcode: str):
        self.logger.debug("CDTTController::_prepare_process")
        self.process.setProgram(jre)
        self.process.setArguments(["-jar", jar, barcode])
        self.process.setWorkingDirectory(directory)

    def _create_backup_tar(self, filepath):
        try:
            backup_path = Path(filepath)
            backup_path.unlink(missing_ok=True)
            with tarfile.open(backup_path, mode="x:gz") as backup_tar:
                backup_tar.add(self.config.directory, arcname="CDTTStereo")
            return str(backup_path.resolve())
        except Exception as e:
            self.logger.critical(e)
            return None
