import json

from typing import override

from controller import Controller

from devices.grip_strength.settings import (
    GRIP_TEST_DATA_DB,
    GRIP_TEST_DATA_PX,
    GRIP_TEST_DB,
    GRIP_TEST_PX,
)
from devices.grip_strength.config import GripStrengthConfig
from devices.grip_strength.session import GripStrengthSession
from devices.grip_strength.model import GripStrengthModel
from devices.grip_strength.view import GripStrengthView


class GripStrengthController(Controller):
    def __init__(
        self,
        session: GripStrengthSession,
        config: GripStrengthConfig,
        model: GripStrengthModel,
        view: GripStrengthView,
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
            {
                "path": (self.config.database_path / GRIP_TEST_DB),
                "arcname": GRIP_TEST_DB,
            },
            {
                "path": (self.config.database_path / GRIP_TEST_PX),
                "arcname": GRIP_TEST_PX,
            },
            {
                "path": (self.config.database_path / GRIP_TEST_DATA_DB),
                "arcname": GRIP_TEST_DATA_DB,
            },
            {
                "path": (self.config.database_path / GRIP_TEST_DATA_PX),
                "arcname": GRIP_TEST_DATA_PX,
            },
        ]

    @override
    def start(self):
        self.logger.info("start")

        if not self._restore_database():
            self._handle_error(store_backup=True)
            return False

        if not self._prepare_process():
            self._handle_error(store_backup=True)
            return False

        self.process.start()

    @override
    def measure(self):
        self.logger.info("measure")

        success, error = self.model.read_results()
        if not success:
            self.logger.error(error)
            self._handle_error(error.capitalize(), store_backup=True)
            return False

        response = self.model.to_response()

        self.logger.debug(json.dumps(response, indent=4))
        self.measured.emit(response)

        self.ready_to_submit.emit(True)

        return True

    def _prepare_process(self) -> bool:
        self.logger.debug("_prepare_process")

        try:
            self.process.setProgram(str(self.config.executable.resolve()))
            self.process.setWorkingDirectory(str(self.config.directory.resolve()))
        except Exception as e:
            self.logger.error(e)
            return False

        return True

    def _restore_database(self) -> bool:
        self.logger.debug("_restore_database")

        try:
            z_grip_test_px = self.config.database_path / GRIP_TEST_PX
            z_grip_test_px.unlink(missing_ok=True)

            z_grip_test_px_backup = self.config.backup_path / GRIP_TEST_PX
            z_grip_test_px_backup.copy(z_grip_test_px)

            z_grip_test_data_px = self.config.database_path / GRIP_TEST_DATA_PX
            z_grip_test_data_px.unlink(missing_ok=True)

            z_grip_test_data_px_backup = self.config.backup_path / GRIP_TEST_DATA_PX
            z_grip_test_data_px_backup.copy(z_grip_test_data_px)

            grip_test_db = self.config.database_path / GRIP_TEST_DB
            grip_test_db.unlink(missing_ok=True)

            grip_test_db_backup = self.config.backup_path / GRIP_TEST_DB
            grip_test_db_backup.copy(grip_test_db)

            grip_test_data_db = self.config.database_path / GRIP_TEST_DATA_DB
            grip_test_data_db.unlink(missing_ok=True)

            grip_test_db_data_backup = self.config.backup_path / GRIP_TEST_DATA_DB
            grip_test_db_data_backup.copy(grip_test_data_db)

        except Exception as e:
            self.logger.error(e)
            return False

        return True