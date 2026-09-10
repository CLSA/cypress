import json
import tarfile

from pathlib import Path
import traceback

from PySide6.QtWidgets import QMessageBox

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

        self.db = BPDatabase(self.config.database)

        #self.view.measurement_form.values_changed.connect(self.handle_manual_entry)

        self.backup_paths = [
            {"path": Path("C:/Microlife"), "arcname": "Microlife"}
        ]

    @override
    def start(self) -> bool:
        self.logger.info("start")

        try:
            if not self._restore_database():
                self._handle_error("Something went wrong", store_backup=True)
                return False

            if not self.db.open():
                self._handle_error("Something went wrong", store_backup=True)
                return False

            success, patient_key = self.db.insert_patient(
                name="Participant",
                barcode=self.session.barcode,
                gender=self.session.sex,
                dob=self.session.dob,
                physician="CLSA",
            )

            if not success:
                self.error.emit("Failed to initialize database")
                return False

            self.logger.debug(patient_key)
            self.patient_key = patient_key
        except Exception as e:
            self.logger.error(e)
            self._handle_error(store_backup=True)
            return False

        finally:
            self.db.close()

        self._prepare_process()
        self.process.start()

    @override
    def measure(self):
        self.logger.info("measure")

        try:
            if not self.db.open():
                self._handle_error(store_backup=True)
                return False

            if not self.patient_key:
                self.logger.error("no patient key found")
                self._handle_error(store_backup=True)
                return False

            success, result = self.db.get_measurements(patient_key=self.patient_key)
            if not success:
                self._handle_error(result, store_backup=True)
                return False

            success, error = self.model.read_results(result)
            if not success:
                self.logger.error(error)
                self._handle_error(store_backup=True)
                return False

        except Exception as e:
            self.logger.critical(e)
            self._handle_error(store_backup=True)
            return False
        finally:
            self.db.close()

        self.measured.emit(self.model.to_response())

    @override
    def restore(self):
        super().restore()
        return self._restore_database()

    def _on_manual_entry(self):
        if (
            not self.model.manual_entry
            and self.model.test
            and len(self.model.test.measures)
        ):
            btn = self._show_message_box(
                title="Entering manual entry mode",
                msg="Automatic measurements exist, are you sure you'd like to enter manual entry? This will remove existing measurements",
                level="warning",
                allow_cancel=True
            )

            if btn == QMessageBox.StandardButton.Ok:
                self.model.set_manual_entry()
                self.view.on_manual_entry()
                self.measured.emit(self.model.to_response())
        else:
            self.view.on_manual_entry()
            self.measured.emit(self.model.to_response())

    def _restore_database(self):
        self.logger.debug("_restore_database")

        try:
            self.config.database.unlink(missing_ok=True)
            self.config.backup_database.copy(self.config.database)
        except Exception as e:
            self.logger.critical(e)
            return False

        return True

    def _prepare_process(self) -> None:
        self.logger.debug("_prepare_process")

        try:
            self.process.setProgram(str(self.config.executable.resolve()))
            self.process.setWorkingDirectory(str(self.config.directory.resolve()))
        except Exception as e:
            self.logger.critical(e)
            return False

        return True