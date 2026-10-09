import traceback

from typing import override
from pathlib import Path

from PySide6.QtCore import Signal

from controller import Controller

from files.receiver import FileReceiver, ReceiverConfig

from devices.ecg.config import ECGConfig
from devices.ecg.session import ECGSession
from devices.ecg.model import ECGModel
from devices.ecg.view import ECGView


class ECGController(Controller):
    files_received = Signal(object)

    def __init__(
        self,
        session: ECGSession,
        config: ECGConfig,
        model: ECGModel,
        view: ECGView,
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

        self.file_receiver = FileReceiver(
            ReceiverConfig(
                storage_dir=self.config.storage_path,
                extensions=[".pdf", ".Xml", ".ecg"],
            )
        )

        self.file_receiver.files_received.connect(self._on_files_received)
        self.file_receiver.files_received.connect(self.view._on_files_received)

        self.file_receiver.start()

        self.view.measure_button.setVisible(True)

        self.start()

    @override
    def start(self):
        if not self._clean_output_dir(self.config.storage_path):
            self._handle_error(store_backup=True)
            return False

        return super().start()

    def _on_files_received(self, files: list[str]):
        files = [Path(file_path) for file_path in files]

        self.logger.debug(files)

        file_count = len(files)
        if file_count > 3:
            self._handle_error("Too many files received", store_backup=True)
            return

        found, result = self._get_file_from_list(files, "ecg")
        if not found:
            return
        ecg_path = result

        found, result = self._get_file_from_list(files, "pdf")
        if not found:
            return
        pdf_path = result

        found, result = self._get_file_from_list(files, "Xml")
        if not found:
            return
        xml_path = result

        ok = self.model.add_files(
            ecg_path=ecg_path, xml_path=xml_path, pdf_path=pdf_path
        )
        if not ok:
            self.logger.error("failed to add files")
            self._handle_error(store_backup=True)
            return

        self.ready_to_measure.emit()

    def _get_file_from_list(
        self, files: list[Path], ext: str
    ) -> tuple[bool, Path | str]:
        paths = [file_path for file_path in files if file_path.suffix == f".{ext}"]
        if len(paths) != 1:
            return False, f"Incorrect number of {ext.upper()} files: {len(paths)}"

        path = paths[0]
        if not path or not path.exists() or not path.is_file():
            return False, f"{ext} file not found"

        return True, path

    @override
    def measure(self):
        try:
            if not self.model.is_valid():
                self.logger.error("results not valid")
                self._handle_error(store_backup=True)
                return False

            self.model.read_results()

            if self.model.barcode is None or (
                self.model.barcode != self.session.barcode
                and self.model.barcode[1:] != self.session.barcode
            ):
                self.logger.error(f"invalid barcode: {self.model.barcode}")
                self._handle_error(
                    f"Invalid barcode: {self.model.barcode}", store_backup=True
                )
                return

            self.measured.emit(self.model.to_response())
            self.ready_to_submit.emit(True)

        except Exception as e:
            traceback.print_exc()
            self.logger.error(e)
            self._handle_error(store_backup=True)

    @override
    def restore(self):
        super().restore()
        return self._clean_output_dir(self.config.storage_path)

    def _clean_output_dir(self, output_dir: Path):
        self.logger.debug("_clean_output_dir")
        try:
            for path in output_dir.iterdir():
                if path.is_file():
                    path.unlink(missing_ok=True)
            return True
        except Exception as e:
            self.logger.error(e)
            return False
