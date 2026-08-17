from PySide6.QtCore import Signal, QCoreApplication
from PySide6.QtSql import QSqlDatabase

import pydicom
import json

from typing import override

from pathlib import Path

from controller import Controller

from devices.dxa.utils.validation import get_scan_type, get_dicom_dict
from devices.dxa.session import DXASession
from devices.dxa.config import DXAConfig
from devices.dxa.model import DXAModel
from devices.dxa.view import DXAView
from devices.dxa.apex.patscan_db import PatScanDB
from devices.dxa.apex.reference_db import ReferenceDB
from files.receiver import DicomReceiver, DicomReceiverConfig


class DXAController(Controller):
    files_received = Signal(object)

    def __init__(
        self,
        session: DXASession,
        config: DXAConfig,
        model: DXAModel,
        view: DXAView,
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

        self.file_receiver = DicomReceiver(
            DicomReceiverConfig(
                executable_path=self.config.executable,
                ae_title=self.config.ae_title,
                host=self.config.host,
                port=self.config.port,
                storage_dir=self.config.storage_path,
                config_path=self.config.storescp_config,
                logger_path=self.config.logger_config,
                extensions=[".dcm"],
            ),
            logger=self.logger,
            parent=self,
        )
        self.file_receiver.files_received.connect(self._on_files_received)
        self.files_received.connect(self.view.on_files_received)

        self.view.measure_button.setVisible(True)
        self.view.close.connect(self.close)
        self.view.destroyed.connect(self.close)

        self.start()

    @override
    def start(self):
        self.logger.info("starting")

        if not self._clean_output_dir(self.config.storage_path):
            self._handle_error(store_backup=False)
            return False

        if not self.file_receiver.start():
            self._handle_error(store_backup=False)
            return False

        self.started.emit()

    @override
    def close(self):
        self.file_receiver.stop()

    @override
    def restore(self):
        try:
            if not self._clean_output_dir(self.config.storage_path):
                self.logger.error("restore: could not clear storage directory")
                return False
            return True
        except Exception as e:
            self.logger.error(e)
            return False

    def _on_files_received(self, dicom_files: list[Path]):
        self.logger.debug("_on_files_received")

        valid_files = {}
        for file_path in dicom_files:
            if ".dcm" != file_path.suffix:
                self.logger.warning(f"unknown file extension: {file_path.suffix}")
                continue

            file_type = get_scan_type(file_path)
            if file_type is None:
                self.logger.warning(f"received unknown file type: {file_path.name}")
                continue

            ds = pydicom.dcmread(file_path, stop_before_pixels=True)
            if ds.PatientID != self.session.barcode:
                self.logger.warning(f"received incorrect patient id: {ds.PatientID}")
                # continue

            valid_files[file_type] = file_path

        ok, scans_status = self.model.update_scans(valid_files)
        if not ok:
            self._handle_error(store_backup=True)
            return False

        self.files_received.emit(scans_status)

        self.ready_to_measure.emit()

    @override
    def measure(self):
        self.logger.info("measure")

        if (
            not self.config.patscan_db_path.exists()
            or not self.config.patscan_db_path.is_file()
        ):
            self._show_message_box(
                self,
                title="Apex is not available",
                msg="Could not access shared Apex drive, please ensure Apex is on and try again",
            )
            self.logger.error("PatScan.mdb is not available")
            return

        if (
            not self.config.reference_db_path.exists()
            or not self.config.reference_db_path.is_file()
        ):
            self._show_message_box(
                self,
                title="Apex is not available",
                msg="Could not access shared Apex drive, please ensure Apex is on and try again",
            )
            self.logger.error("reference.mdb is not available")
            return

        self.view.set_status("Copying files from DEXA (1/2)")
        QCoreApplication.processEvents()

        self.logger.info("copying PatScan.mdb from apex (1/2)")
        self.config.patscan_db_path.copy_into(Path.cwd())

        self.view.set_status("Copying files from DEXA (2/2)")
        QCoreApplication.processEvents()

        self.logger.info("copying reference.mdb from apex (2/2)")
        self.config.reference_db_path.copy_into(Path.cwd())

        try:
            patscan_db = PatScanDB(Path.cwd() / "PatScan.mdb")
            if not patscan_db.open():
                self.logger.error("couldn't open patscan")
                return

            reference_db = ReferenceDB(Path.cwd() / "reference.mdb")
            if not reference_db.open():
                self.logger.error("couldn't open reference")
                return

            ok, patient_info = patscan_db.get_patient_info(barcode=self.session.barcode)
            if not ok:
                self.logger.error("failed to get patient info")
                self._handle_error(store_backup=True)

            self.logger.debug(patient_info)

            self.model.analyze(patient_info, patscan_db, reference_db)
            self.ready_to_submit.emit(True)

            self.logger.debug(json.dumps(self.model.to_response(), indent=4))

            patscan_db.close()
            reference_db.close()

        except Exception as e:
            self.logger.error(e)
            self._handle_error(e)


    def _clean_output_dir(self, output_dir: Path):
        self.logger.debug("_clean_output_dir")

        (Path.cwd() / "PatScan.mdb").unlink(missing_ok=True)
        (Path.cwd() / "reference.mdb").unlink(missing_ok=True)

        try:
            for path in output_dir.iterdir():
                if path.is_file():
                    path.unlink(missing_ok=True)

        except Exception as e:
            self.logger.error(e)
            return False

        return True
