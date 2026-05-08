import json

from pathlib import Path

from PySide6.QtCore import QObject, QProcess, Signal
from PySide6.QtWidgets import QFileDialog

from session import Session
from config import DeviceConfig

from devices.model import Model

class Controller(QObject):
    started = Signal()
    finished = Signal()
    measured = Signal(dict)
    submitted = Signal()
    error = Signal(str, str)

    def __init__(self,
                 session: Session,
                 config: DeviceConfig,
                 model: Model,
                 standalone: bool = False,
                 parent = None):
        super().__init__(parent)

        self.session = session
        self.config = config
        self.model = model
        self.standalone = standalone

        self.process = QProcess(self)
        self.process.started.connect(self._on_process_started)
        self.process.finished.connect(self._on_process_finished)
        self.process.destroyed.connect(self._on_process_destroyed)
        self.process.errorOccurred.connect(self._on_process_error)

        self.data = {}
        self.manually_entered = False

    def start(self) -> bool:
        print("Controller start..")
        return True

    def measure(self) -> dict:
        print("Controller measure..")
        self.measured.emit()
        return self.data

    def submit(self):
        print("Controller submitted..")
        if self.standalone:
            file_path, selected_filter = QFileDialog.getSaveFileName(
                None,
                "Select output file",
                str((Path.home() / 'Documents' / f'{self.session.barcode}.json').resolve()),
                'JSON files (*.json)'
            )
            if file_path:
                with open(file_path, "w") as file:
                    json.dump(self.model.to_response(), file, indent=4)
            else:
                print("User closed dialog")

        self.submitted.emit()

    def _on_process_started(self, *args):
        print("on process started")
        self.started.emit()

    def _on_process_finished(self, *args):
        print("on process finished")
        self.finished.emit()

    def _on_process_destroyed(self, *args):
        print("on process destroyed")
        self.error.emit()

    def _on_process_error(self, *args):
        print("on process error", args)
        self.error.emit()