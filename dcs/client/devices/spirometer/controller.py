import json
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

        self.backup_paths = [
            {"path": self.config.exchange_path, "arcname": "exchange"},
            {"path": self.config.database_path, "arcname": "database"},
        ]

        self.start()

    @override
    def start(self) -> bool:
        self.logger.info("start")

        if is_process_running(self.config.process_name):
            self.error.emit(f"{self.config.process_name} is already open")
            return False

        if not self._restore_database():
            self.logger.error("could not restore database")
            self._handle_error(store_backup=True)
            return False

        if not self._clean_output_dir(self.config.exchange_path):
            self.logger.error("could not clear exchange path")
            self._handle_error(store_backup=True)
            return False

        ethnicity = self._select_ethnicity()
        if not ethnicity:
            self.logger.warning("ethnicity selection cancelled")
            self.view.close()
            return False

        if not EMRPlugin.write(
            session=self.session,
            config=self.config,
            ethnicity=ethnicity,
            output_path=self.config.exchange_path / self.config.in_file_name,
        ):
            self.logger.error("failed to write emr")
            self._handle_error(store_backup=False)
            return False

        self._prepare_process()
        self.process.start()

        return True

    @override
    def measure(self):
        self.logger.info("measure")

        try:
            self.model.read_results(
                self.config.exchange_path / self.config.out_file_name
            )
            self.measured.emit(self.model.to_response())
            # if not self.model.is_valid():
            #     self._handle_error(store_backup=True)
            #     return
            self.ready_to_submit.emit(True)
        except FileNotFoundError as e:
            self.logger.error(e)
            self._handle_error(store_backup=True)
            return
        except ValueError as e:
            self.logger.error(e)
            self._handle_error(store_backup=True)
            return
        except Exception as e:
            self.logger.error(e)
            self._handle_error(store_backup=True)
            return

    @override
    def restore(self) -> bool:
        self.logger.info("restore")
        return all(
            [
                self._restore_database(),
                self._clean_output_dir(self.config.exchange_path),
            ]
        )

    def _restore_database(self) -> bool:
        self.logger.debug("_restore_database")

        try:
            self.config.database_path.unlink(missing_ok=True)
            self.config.backup_database.copy(self.config.database_path)
            return True
        except Exception as e:
            self.logger.error(e)
            return False

    def _clean_output_dir(self, output_dir: Path) -> bool:
        self.logger.debug("_clean_output_dir")
        try:
            for path in output_dir.iterdir():
                if path.is_file():
                    path.unlink(missing_ok=True)
            return True
        except Exception as e:
            self.logger.error(e)
            return False

    def _select_ethnicity(self):
        self.logger.debug("_select_ethnicity")

        dialog = SelectEthnicityDialog()
        ethnicity = None
        if dialog.exec() == QDialog.Accepted:
            ethnicity = dialog.get_selected_value()
        return ethnicity

    def _prepare_process(self):
        self.logger.debug("_prepare_process")

        self.process.setProgram(str(self.config.executable.resolve()))
        self.process.setArguments([])
        self.process.setWorkingDirectory(str(self.config.working_directory.resolve()))
