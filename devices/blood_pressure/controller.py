from instruments.controller import Controller

from instruments.blood_pressure.model import BPModel
from instruments.blood_pressure.config import BPConfig
from instruments.blood_pressure.session import BPSession

from typing import override


class BPController(Controller):
    def __init__(
        self,
        session: BPSession,
        config: BPConfig,
        model: BPModel,
        standalone: bool = False,
        parent=None,
    ):
        super().__init__(
            session=session,
            config=config,
            model=model,
            standalone=standalone,
            parent=parent,
        )

    @override
    def start(self) -> bool:
        if not self._clean():
            return False

        self._prepare_process()
        #self.process.start()


    @override
    def measure(self):
        pass

    def _prepare_process(self) -> None:
        self.process.setProgram(str(self.config.executable.resolve()))
        # self.process.setArguments(str(self.executable.resolve()))
        self.process.setWorkingDirectory(str(self.config.working_directory.resolve()))
