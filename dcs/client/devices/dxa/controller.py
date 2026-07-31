from PySide6.QtCore import Signal, QCoreApplication
from PySide6.QtSql import QSqlDatabase

import pydicom
import json

from typing import override

from pathlib import Path

from controller import Controller
from utils import DicomFileInfo

from devices.dxa.utils.validation import get_file_type
from devices.dxa.session import DXASession
from devices.dxa.config import DXAConfig
from devices.dxa.model import DXAModel
from devices.dxa.view import DXAView

#from devices.dxa.utils.apex import ApexDatabaseReader

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

        self.files: list[DicomFileInfo] = []
        self.file_receiver = DicomReceiver(
            DicomReceiverConfig(
                executable_path=self.config.executable,
                ae_title=self.config.ae_title,
                host=self.config.host,
                port=self.config.port,
                storage_dir=self.config.storage_path,
                config_path=self.config.storescp_config,
                extensions=[".dcm"],
            )
        )
        self.file_receiver.files_received.connect(self._on_files_received)
        self.file_receiver.start()
        self.files_received.connect(self.view.on_files_received)

        self.view.measure_button.setVisible(True)

        self.view.close.connect(self.close)
        self.view.destroyed.connect(self.close)

    @override
    def close(self):
        self.file_receiver.close()

    @override
    def start(self):
        super().start()
        self.ready_to_measure.emit()

    @override
    def measure(self):
        if (
            not self.config.patscan_db_path.exists()
            or not self.config.patscan_db_path.is_file()
        ):
            self.logger.error("patscan.mdb is not available")
            return

        if (
            not self.config.reference_db_path.exists()
            or not self.config.reference_db_path.is_file()
        ):
            self.logger.error("reference.mdb is not available")
            return

        self.logger.debug("Copying patscan")
        self.config.patscan_db_path.copy_into(Path.cwd())

        QCoreApplication.processEvents()

        self.logger.debug("Copying reference")
        self.config.reference_db_path.copy_into(Path.cwd())

        QCoreApplication.processEvents()

        self.model.set_apex_db_paths(
            Path.cwd() / "PatScan.mdb", Path.cwd() / "reference.mdb"
        )

        patient_info = self.model.get_patient_info()
        print("patient info", json.dumps(patient_info, indent=4))

        scans = self.model.get_scans(patient_info)
        print("scans", json.dumps(scans, indent=4))

        for file_info in self.files:
            ds = pydicom.dcmread(file_info.file_path, stop_before_pixels=True)
            print(get_file_type(ds))

        self.measured.emit(self.model.to_response())

    def _on_files_received(self, dicom_files: list[DicomFileInfo]):
        self.files = dicom_files
        self.files_received.emit(self.files)
