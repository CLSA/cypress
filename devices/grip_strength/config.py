from typing import Annotated

from pydantic import Field, DirectoryPath, FilePath, AfterValidator

from config import DeviceConfig, is_executable


class GripStrengthConfig(DeviceConfig):
    # The name of the process as it appears in Task Manager
    # process_name: Annotated[str, Field(min_length=5, frozen=True)]

    # # The path to the exe
    # executable: Annotated[FilePath, Field(frozen=True), AfterValidator(is_executable)]

    # # Path to the app's working directory
    # directory: Annotated[DirectoryPath, Field(frozen=True)]

    # # Where we store the clean database
    # backup_path: Annotated[str, Field(frozen=True)]

    # # Where the database files are located
    # database_path: Annotated[DirectoryPath, Field(frozen=True)]

    # # The name of the grip test database
    # grip_test_db: Annotated[str, Field(min_length=1, frozen=True)]

    # # The name of the grip test database that stores the data
    # grip_test_data_db: Annotated[str, Field(min_length=1, frozen=True)]
    @classmethod
    def is_device_installed(cls, config_name="config.ini", device_name="grip_strength"):
        return super().is_device_installed(config_name, device_name)

    @classmethod
    def from_ini(cls, ini_file_path="config.ini", section="grip_strength"):
        return super().from_ini(ini_file_path, section)
