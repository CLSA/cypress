from typing import override

from devices.controller import Controller
from devices.utils import is_process_running

from .model import AudiometerModel
from .plugin import AudiometerPlugin
from .session import AudiometerSession
from .config import AudiometerConfig


class AudiometerController(Controller):
    def __init__(
        self,
        session: AudiometerSession,
        config: AudiometerConfig,
        model: AudiometerModel,
        detached: bool = False,
        parent=None,
    ):
        super().__init__(
            parent=parent,
            session=session,
            config=config,
            model=model,
            detached=detached,
        )
        self.plugin = AudiometerPlugin(session=self.session, config=self.config)

    def manual_entry(self, data_received: dict):
        if self.model.set_manual_entry(data_received):
            self.manually_entered = True
            self.measured.emit(self.model.get_response())
        else:
            self.manually_entered = False

    @override
    def start(self) -> bool:
        """
        Setup the Hearcon device by performing the following:
            1. Check that the app isn't already running
            2. Restore the app database from backup
            3. Insert participant information into the app database
            4. Launch the app

        Return

        """
        # TODO check process running
        if is_process_running(self.config.process_name):
            self.error.emit("Error:", f"{self.config.process_name} is already open")
            return False

        # Remove the app database and restore from clean backup
        if not self._restore_database():
            self.error.emit("Error:", f"could not restore database")
            return False

        # Insert the participant information into the app database
        if not self.plugin.initialize():
            self.error.emit("Error:", f"could not initialize plugin")
            return False

        # Run the app
        if not self._prepare_process():
            self.error.emit("Error:", f"could not prepare process")
            return False

        self.process.start()
        self.process.waitForStarted()

        return True

    @override
    def measure(self):
        """
        Retrieve the results from the app database and validate it, then inform the view with the data

        """
        self._clean_output_dir()

        # Read the app database and parse the results
        if not self.plugin.get_results():
            self.error.emit("Error", "could not retrieve hearcon results")
            return

    def _clean_output_dir(self) -> None:
        """
        Remove the output file that gets generated from the plugin

        """
        self.config.plugin_output_path.unlink(missing_ok=True)

    def _restore_database(self) -> bool:
        """
        Overwrite the app database with the backup database

        Returns whether the operation was successful

        """

        return False

    def _prepare_process(self) -> None:
        """
        Configures QProcess that handles the Hearcon app

        """

        self.process.setProgram(self.config.process_path)
        self.process.setArguments([])
        self.process.setWorkingDirectory(self.config.working_path)
