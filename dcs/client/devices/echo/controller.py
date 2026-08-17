import json
import pydicom

from PySide6.QtCore import Signal

from pathlib import Path
from typing import override

from files.receiver import DicomReceiver, DicomReceiverConfig

from controller import Controller

from devices.echo.session import ECHOSession
from devices.echo.config import ECHOConfig
from devices.echo.model import ECHOModel
from devices.echo.view import ECHOView


class ECHOController(Controller):
    files_received = Signal(object)

    def __init__(
        self,
        session: ECHOSession,
        config: ECHOConfig,
        model: ECHOModel,
        view: ECHOView,
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

        self.scans_received: list[Path] = []
        self.file_receiver = DicomReceiver(
            DicomReceiverConfig(
                executable_path=self.config.executable,
                ae_title=self.config.ae_title,
                host=self.config.host,
                port=self.config.port,
                storage_dir=self.config.storage_path,
                logger_path=self.config.logging_config,
                config_path=self.config.storescp_config,
                extensions=[".dcm"],
            ),
            logger=self.logger,
        )
        self.file_receiver.files_received.connect(self._on_files_received)
        self.files_received.connect(self.view.on_files_received)

        self.view.measure_button.setVisible(True)

        self.view.close.connect(self.close)
        self.view.destroyed.connect(self.close)
        self.view.measure_button.setVisible(True)

        self.start()

    @override
    def start(self):
        self.logger.info("started")

        if not self._clear_output_dir(self.config.storage_path):
            self._handle_error(store_backup=False)
            return False

        if not self.file_receiver.start():
            self._handle_error(store_backup=False)
            return False

        self.started.emit()

    @override
    def close(self):
        self.file_receiver.close()

    def _on_files_received(self, files_received: list[Path]):
        self.logger.debug("_on_files_received")

        self.scans_received = set([])
        for file_path in files_received:
            if ".dcm" != file_path.suffix:
                self.logger.warning(f"received invalid file ext {file_path.name}")
                continue

            # ds = pydicom.dcmread(file_path, stop_before_pixels=True)
            # if ds["PatientID"].value != self.session.barcode:
            #     # self._handle_error(
            #     #     f"PatientID: {ds["PatientID"].value} does not match Participant ID: {self.session.barcode}",
            #     #     store_backup=True,
            #     # )
            #     return False

            self.scans_received.add(file_path)

        us_count = len([x for x in self.scans_received if x.name.split(".")[0] == "US"])
        usm_count = len(
            [x for x in self.scans_received if x.name.split(".")[0] == "USm"]
        )
        src_count = len(
            [x for x in self.scans_received if x.name.split(".")[0] == "SRc"]
        )
        total = len(self.scans_received)

        self.files_received.emit(
            {"us": us_count, "usm": usm_count, "src": src_count, "total": total}
        )

        self.model.reset()
        for file_path in self.scans_received:
            added, error = self.model.add_file(file_path, send_name=file_path.name)
            if not added:
                self._handle_error(error, store_backup=True)
                return False

        if us_count >= 1 and usm_count >= 1 and src_count >= 1:
            print("Ready to measure")
            self.ready_to_measure.emit()

        #self.measured.emit(self.model.to_response())
        #self.ready_to_measure.emit()

        return True

    @override
    def measure(self):
        self.logger.info("measure")

        if not self.model.is_valid():
            self.logger.error("results are not valid")
            self._handle_error(store_backup=True)

        self.logger.debug(json.dumps(self.model.to_response(), indent=4))
        self.measured.emit(self.model.to_response())
        self.ready_to_submit.emit(True)

    @override
    def restore(self):
        self.file_receiver.stop()

        super().restore()
        try:
            if not self._clear_output_dir(self.config.storage_path):
                self.logger.error("restore: could not clear storage directory")
                return False
            return True
        except Exception as e:
            self.logger.error(e)
            return False

    def _clear_output_dir(self, output_dir: Path) -> bool:
        self.logger.debug("_clean_output_dir")
        try:
            for path in output_dir.iterdir():
                if path.is_file():
                    path.unlink(missing_ok=True)
            return True
        except Exception as e:
            self.logger.error(e)
            return False
