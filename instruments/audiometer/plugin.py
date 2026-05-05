from PySide6.QtCore import QProcess, QObject

from .session import AudiometerSession
from .config import AudiometerConfig

class AudiometerPlugin(QObject):
    def __init__(
        self, session: AudiometerSession, config: AudiometerConfig, parent=None
    ):
        super().__init__(parent)

        self.session = session
        self.config = config
        self.process = QProcess()

    def initialize(self):
        self._configure_plugin("initialize")

        self.process.start()
        if not self.process.waitForFinished():
            return None, ["plugin did not finish"]

        output = self.process.readAllStandardOutput()
        errors = self.process.readAllStandardError()

        return output, errors

    def get_results(self):
        self._configure_plugin("get_results")
        self.process.start()

        if not self.process.waitForFinished():
            return None, ["plugin did not finish"]

        output = self.process.readAllStandardOutput()
        errors = self.process.readAllStandardError()

        return output, errors

    def _configure_plugin(self, operation="initialize"):
        args = []

        barcode = self.session.barcode
        dob = self.session.dob
        sex = self.session.sex[0].upper()

        args.append(
            str(self.config.existing_database_path.resolve()),
            str(self.config.reader_output_path.resolve()),
            operation,
            barcode,
            dob,
            sex,
        )

        self.process.setProgram(self.config.reader_path)
        self.process.setArguments(args)
        self.process.setWorkingDirectory(self.config.reader_working_directory)
