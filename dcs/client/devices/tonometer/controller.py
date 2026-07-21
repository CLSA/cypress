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

    @override
    def start(self):
        self.logger.debug(f"{self.class_name()}.start()")

        if is_process_running(self.config.process_name):
            self.error.emit(f"{self.config.process_name} already running")
            return False

        if not self._restore_database():
            self.error.emit(f"Could not restore database")
            return False

        if not self._insert_participant():
            self.error.emit(f"Could not setup database")
            return False

        self._prepare_process()
        self.process.start()

    @override
    def measure(self):
        self.logger.debug(f"{self.class_name()}.measure()")

        if not self.model.read_results():
            self.error.emit(f"Failed to read results")
            return False

        response = self.model.to_response()

        self.logger.debug(json.dumps(response, indent=4))
        print(json.dumps(response, indent=4))
        self.measured.emit(response)

        return True

    def _restore_database(self) -> bool:
        self.logger.debug(f"{self.class_name()}._restore_database()")

        try:
            self.config.database.unlink(missing_ok=True)
            self.config.backup_database.copy(self.config.database)
        except Exception as e:
            self.logger.critical(e)
            return False

        return True

    def _insert_participant(self) -> bool:
        self.logger.debug(f"{self.class_name()}._insert_participant()")

        try:
            database = TonometerDatabase(self.config.database)
            if database.open():
                inserted = database.insert_participant(self.session)
                return inserted
            else:
                return False
        except Exception as e:
            self.logger.critical(e)
            return False
        finally:
            database.close()

    def _prepare_process(self):
        self.logger.debug(f"{self.class_name()}._prepare_process()")

        self.process.setProgram(str(self.config.executable.resolve()))
        self.process.setWorkingDirectory(str(self.config.directory.resolve()))
