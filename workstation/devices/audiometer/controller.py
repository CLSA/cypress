import json

from pathlib import Path
from typing import override

from controller import Controller
from utils import is_process_running

from devices.audiometer.model import AudiometerModel
from devices.audiometer.view import AudiometerView
from devices.audiometer.plugin import AudiometerPlugin
from devices.audiometer.session import AudiometerSession
from devices.audiometer.config import AudiometerConfig


class AudiometerController(Controller):
    def __init__(
        self,
        session: AudiometerSession,
        config: AudiometerConfig,
        model: AudiometerModel,
        view: AudiometerView,
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
        self.plugin = AudiometerPlugin(session=self.session, config=self.config)
        self.view.measurement_form.values_changed.connect(self.handle_manual_entry)

    def handle_manual_entry(self, data_received: dict):
        self.model.set_manual_values(data_received)
        #self.measured.emit(self.model.to_response())

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
        self.logger.debug(f"{self.class_name()}::start")

        if is_process_running(self.config.process_name):
            self.error.emit("Error:", f"{self.config.process_name} is already open")
            return False

        # Remove the app database and restore from clean backup
        if not self._restore_database():
            self.error.emit("Error:", f"could not restore database")
            return False

        # Insert the participant information into the app database
        output, errors = self.plugin.initialize()
        if not output or len(errors):
            self.error.emit("Error:", f"could not setup hearcon")
            return False

        # Run the app
        self._prepare_process()
        self.process.start()

        return True

    @override
    def measure(self):
        """
        Retrieve the results from the app database and validate it, then inform the view with the data

        """
        self.logger.debug(f"{self.class_name()}::measure")
        self._clean_output_dir()

        # Read the app database and parse the results
        results, errors = self.plugin.get_results()
        if not results or len(errors):
            self.error.emit("Error", "could not retrieve hearcon results")
            return False

        try:
            with open(Path(self.config.plugin_output_path), "r") as output_json_file:
                json_data = json.load(output_json_file)
                self.model.parse_output_json(json_data)
                self.measured.emit(self.model.to_response())
        except Exception as e:
            print(e)
            return False

        return True

    def _clean_output_dir(self) -> None:
        """
        Remove the output file that gets generated from the plugin

        """
        self.logger.debug(f"{self.class_name()}::_clean_output_dir")
        self.config.plugin_output_path.unlink(missing_ok=True)

    def _restore_database(self) -> bool:
        """
        Overwrite the app database with the backup database

        Returns whether the operation was successful

        """
        self.logger.debug(f"{self.class_name()}::_restore_database")

        self.config.existing_database_path.unlink(missing_ok=True)
        self.config.backup_database_path.copy(self.config.existing_database_path)

        return True

    def _prepare_process(self) -> None:
        """
        Configures QProcess that handles the Hearcon app

        """
        self.logger.debug(f"{self.class_name()}::_prepare_process")

        self.process.setProgram(str(self.config.process_path.resolve()))
        self.process.setArguments([])
        self.process.setWorkingDirectory(str(self.config.working_path.resolve()))
