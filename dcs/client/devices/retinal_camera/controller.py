import json

from typing import override

from controller import Controller

from devices.retinal_camera.session import RetinalCameraSession
from devices.retinal_camera.config import RetinalCameraConfig
from devices.retinal_camera.model import RetinalCameraModel
from devices.retinal_camera.database import RetinalCameraDatabase
from devices.retinal_camera.view import RetinalCameraView


class RetinalCameraController(Controller):
    def __init__(
        self,
        session: RetinalCameraSession,
        config: RetinalCameraConfig,
        model: RetinalCameraModel,
        view: RetinalCameraView,
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

        self.backup_paths = [{"path": self.config.database_backup, "arcname": "oct"}]

        self.db = RetinalCameraDatabase(self.config.database_name)

        self.start()

    @override
    def start(self):
        self.logger.info("start")

        if not self._clear_output_dir(self.config.export_path):
            self._handle_error(f"Could not prepare stage", store_backup=True)
            return False

        if not self._clear_output_dir(self.config.data_path):
            self._handle_error(f"Could not prepare stage", store_backup=True)
            return False

        if not self._restore_database():
            self._handle_error(f"Could not restore database", store_backup=True)
            return False

        if not self._insert_participant():
            self._handle_error(f"Could not initialize database", store_backup=True)
            return False

        self.ready_to_measure.emit()

    @override
    def measure(self):
        self.logger.info("measure")

        success = self.model.read_results(self.config.export_path)
        if not success:
            self._handle_error(store_backup=True)
            return

        if len(self.model.measures) == 0:
            self.measured.emit(self.model.to_response())
            self.ready_to_measure.emit()

            self._show_message_box(
                "No files found",
                f"No files found, please export and try again",
                level="warning",
            )

            return

        self.logger.debug(json.dumps(self.model.to_response(), indent=2))
        self.measured.emit(self.model.to_response())

    def _clear_output_dir(self, dir_path):
        self.logger.debug("_clean_output_dir")
        try:
            for path in dir_path.iterdir():
                if path.is_file():
                    path.unlink(missing_ok=True)
        except Exception as e:
            self.logger.critical(e)
            return False

        return True

    def _restore_database(self):
        self.logger.debug("_restore_database")

        try:
            if not self.db.open():
                return False

            success, error = self.db.restore_database(self.config.database_backup)
            if not success:
                self.logger.error(error)
                return False

        except Exception as e:
            self.logger.critical(e)
            return False

        finally:
            self.db.close()

        return True

    def _insert_participant(self):
        self.logger.debug("_insert_participant")

        try:
            if not self.db.open():
                return False

            success, error = self.db.insert_participant(session=self.session)
            if not success:
                self.logger.error(error)
                return False

        except Exception as e:
            self.logger.critical(e)
            return False

        finally:
            self.db.close()

        return True
