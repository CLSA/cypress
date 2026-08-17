import json

from typing import override

from controller import Controller

from utils import is_process_running

from devices.tonometer.session import TonometerSession
from devices.tonometer.config import TonometerConfig
from devices.tonometer.model import TonometerModel
from devices.tonometer.view import TonometerView
from devices.tonometer.database import TonometerDatabase


class TonometerController(Controller):
    def __init__(
        self,
        session: TonometerSession,
        config: TonometerConfig,
        model: TonometerModel,
        view: TonometerView,
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
            {"path": self.config.directory, "arcname": "ORAG3"},
        ]

    @override
    def start(self):
        self.logger.info("start")
        if is_process_running(self.config.process_name):
            self._show_message_box(
                title="Tonometer already running",
                msg=f"{self.config.process_name} already running, please close the app and try again",
                level="warning",
            )
            return False

        self.logger.info(f"restoring")
        if not self._restore_database():
            self._handle_error(f"Could not restore database", store_backup=True)
            return False

        self.logger.info(f"initializing db")
        if not self._insert_participant():
            self._handle_error(f"Could not setup database", store_backup=True)
            return False

        self.logger.info(f"preparing db")
        self._prepare_process()

        self.logger.info(f"starting process")
        self.process.start()

    @override
    def measure(self):
        self.logger.info(f"measure")

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

    def _restore_database(self) -> bool:
        self.logger.debug("_restore_database")

        try:
            self.config.database.unlink(missing_ok=True)
            self.config.backup_database.copy(self.config.database)
        except Exception as e:
            self.logger.critical(e)
            return False

        return True

    def _insert_participant(self) -> bool:
        self.logger.debug(f"_insert_participant")

        try:
            database = TonometerDatabase(self.config.database)
            if not database.open():
                return False

            success, error = database.insert_participant(self.session)
            if not success:
                self.logger.critical(error)
                return False
            return True

        except Exception as e:
            self.logger.critical(e)
            return False

        finally:
            database.close()

    def _prepare_process(self):
        self.logger.debug("_prepare_process")

        self.process.setProgram(str(self.config.executable.resolve()))
        self.process.setWorkingDirectory(str(self.config.working_directory.resolve()))
