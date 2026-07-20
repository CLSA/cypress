import tarfile

from typing import override
from pathlib import Path

from PySide6.QtWidgets import QDialog

from controller import Controller

from devices.spirometer.session import SpirometerSession
from devices.spirometer.config import SpirometerConfig
from devices.spirometer.model import SpirometerModel
from devices.spirometer.view import SpirometerView, SelectEthnicityDialog

from devices.spirometer.emr.input.emr import EMRPlugin
from utils import is_process_running


class SpirometerController(Controller):
    def __init__(
        self,
        session: SpirometerSession,
        config: SpirometerConfig,
        model: SpirometerModel,
        view: SpirometerView,
        detached: bool = False,
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
        if is_process_running(self.config.process_name):
            self.error.emit("Error", f"{self.config.process_name} is already open")
            return False

        self._restore_database()

        ethnicity = self._select_ethnicity()
        if not ethnicity:
            self.logger.info("ethnicity selection cancelled")
            return False

        EMRPlugin.write(
            session=self.session,
            ethnicity=ethnicity,
            output_path=self.config.exchange_path / self.config.in_file_name,
        )

        self._prepare_process()
        self.process.start()

        return True

    @override
    def measure(self):
        try:
            self.model.read_results(
                self.config.exchange_path / self.config.out_file_name
            )
        except FileNotFoundError as e:
            self.logger.critical(e)
            self.error.emit("error", "File not found")
        except ValueError as e:
            self.logger.critical(e)
            self.error.emit("error", "File is invalid")
        except Exception as e:
            self.error.emit("error", "Unknown error")
            self.logger.critical(e)

        self.measured.emit(self.model.to_response())

    def _restore_database(self):
        self.config.database_path.unlink(missing_ok=True)
        self.config.backup_database.copy(self.config.database_path)

    def _select_ethnicity(self):
        dialog = SelectEthnicityDialog()
        ethnicity = None
        if dialog.exec() == QDialog.Accepted:
            ethnicity = dialog.get_selected_value()
        return ethnicity

    def _prepare_process(self):
        self.logger.debug(f"{self.class_name()}::_prepare_process")

        self.process.setProgram(str(self.config.executable.resolve()))
        self.process.setArguments([])
        self.process.setWorkingDirectory(str(self.config.working_directory.resolve()))

    def _create_backup_tar(self) -> str | None:
        try:
            backup_path = Path("./config.tar.gz")
            backup_path.unlink(missing_ok=True)
            with tarfile.open(backup_path, mode="x:gz") as backup_tar:
                backup_tar.add(self.config.database_path, arcname="database")
                backup_tar.add(self.config.exchange_path, arcname="emr")
            return str(backup_path.resolve())
        except Exception as e:
            return None
