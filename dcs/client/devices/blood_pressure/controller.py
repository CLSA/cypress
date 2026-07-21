import json
import tarfile

from pathlib import Path

from controller import Controller

from devices.blood_pressure.model import BPModel
from devices.blood_pressure.view import BPView
from devices.blood_pressure.config import BPConfig
from devices.blood_pressure.session import BPSession
from devices.blood_pressure.database import BPDatabase

from typing import override


class BPController(Controller):
    def __init__(
        self,
        session: BPSession,
        config: BPConfig,
        model: BPModel,
        view: BPView,
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
        self.logger.debug("BPController::__init__")

        self.db = BPDatabase(self.config.database_path)
        self.view.measurement_table_widget.measureButton.setVisible(True)

    @override
    def start(self) -> bool:
        self.logger.debug("BPController::start")

        try:
            if not self.db.open():
                self.error.emit("Failed to open database")
                return False

            self.patient_key = self.db.insert_patient(
                "Participant",
                self.session.barcode,
                self.session.sex,
                self.session.dob,
                "CLSA",
            )
        except:
            self.error.emit("Failed to load results")
            return False
        finally:
            self.db.close()

        self._prepare_process()
        self.process.start()

    @override
    def measure(self):
        self.logger.debug("BPController::measure")
        try:
            if not self.db.open():
                self.error.emit("Failed to open database")
                return False

            self.raw_results = self.db.get_measurements(patient_key=self.patient_key)
            self.model.read_results(self.raw_results)
        except:
            self.error.emit("Failed to load results")
            return False
        finally:
            self.db.close()

        self.measured.emit(self.model.to_response())

    def _prepare_process(self) -> None:
        self.logger.debug("BPController::_prepare_process")

        self.process.setProgram(str(self.config.executable.resolve()))
        self.process.setWorkingDirectory(str(self.config.directory.resolve()))

    def _create_backup_tar(self) -> str | None:
        try:
            backup_path = Path("./config.tar.gz")
            backup_path.unlink(missing_ok=True)
            with tarfile.open(backup_path, mode="x:gz") as backup_tar:
                backup_tar.add("C:/Microlife")
            return str(backup_path.resolve())
        except Exception as e:
            self.logger.critical(e)
            return None
