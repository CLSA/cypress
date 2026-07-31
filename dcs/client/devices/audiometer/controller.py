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

        self.backup_paths = [
            { "path": Path("C:/ProgramData/Ra660/"), "arcname": "Ra660" },
            { "path": self.config.plugin_output_path, "arcname": "output.json" },
        ]

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
        self.logger.info(f"starting")
        if is_process_running(self.config.process_name):
            self.logger.error(f"{self.config.process_name} is already open")
            self._handle_error(f"HearCon is already open, please close and try again")
            return False

        self.logger.info(f"restoring database")
        if not self._restore_database():
            self._handle_error("Something went wrong", store_backup=True)
            return False

        self.logger.info(f"inserting participant")
        output, errors = self.plugin.initialize()
        if not output or len(errors):
            self.logger.error(errors)
            self._handle_error(f"Something went wrong", store_backup=True)
            return False

        self.logger.info(f"preparing process")
        self._prepare_process()

        self.logger.info(f"starting process")
        self.process.start()

        return True

    @override
    def measure(self):
        """
        Retrieve the results from the app database and validate it, then inform the view with the data

        """
        self.logger.info("measuring")

        if not self._clean_output_dir():
            self.logger.error("failed to clean output directory")
            self._handle_error("Could not read results", store_backup=True)

        # Read the app database and parse the results
        results, error = self.plugin.get_results()
        if not results or error:
            self.logger.error(error)
            self._handle_error("Could not read database", store_backup=True)
            return False

        try:
            with open(self.config.plugin_output_path, "r") as output_json_file:
                json_data = json.load(output_json_file)
                self.model.parse_output_json(json_data)
                self.measured.emit(self.model.to_response())
        except FileNotFoundError:
            self.logger.error("results file not found")
            self._handle_error(f"Results file not found", store_backup=True)
        except Exception as e:
            self.logger.critical(e)
            self._handle_error("Something went wrong", store_backup=True)
            return False

        return True

    def _clean_output_dir(self) -> bool:
        """
        Remove the output file that gets generated from the plugin

        """
        self.logger.debug("_clean_output_dir")

        try:
            self.config.plugin_output_path.unlink(missing_ok=True)
            return True
        except Exception as e:
            self.logger.critical(e)
            return False

    def _restore_database(self) -> bool:
        """
        Overwrite the app database with the backup database

        Returns whether the operation was successful

        """
        self.logger.debug("_restore_database")

        try:
            self.config.existing_database_path.unlink(missing_ok=True)
            self.config.backup_database_path.copy(self.config.existing_database_path)
        except Exception as e:
            self.logger.critical(e)
            return False
        return True

    def _prepare_process(self) -> None:
        """
        Configures QProcess that handles the Hearcon app

        """
        self.logger.debug("_prepare_process")

        self.process.setProgram(str(self.config.process_path.resolve()))
        self.process.setArguments([])
        self.process.setWorkingDirectory(str(self.config.working_path.resolve()))