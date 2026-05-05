from instruments.controller import Controller

from .model import BPModel
from typing import override


class BloodPressureController(Controller):

    def __init__(self, parent=None, session_data={}):
        super().__init__(parent, session_data)
        self.model = BPModel(session_data=session_data)

    @override
    def start(self) -> bool:
        if not self.is_installed():
            return False

        if not self._clean():
            return False

        self._prepare_process()
        self.process.start()

    @override
    def measure(self):
        pass

    @override
    def submit(self):
        pass

    def _prepare_process(self) -> None:
        self.process.setProgram(str(self.config.executable.resolve()))
        # self.process.setArguments(str(self.executable.resolve()))
        self.process.setWorkingDirectory(str(self.config.working_directory.resolve()))
