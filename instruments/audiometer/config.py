from pathlib import Path

from typing import Annotated
from pydantic import (
    Field,
    DirectoryPath,
    FilePath,
    AfterValidator
)

from config import DeviceConfig, is_executable


class AudiometerConfig(DeviceConfig):
    # The name of the process as it appears in Task Manager
    process_name: Annotated[str, Field(min_length=1, frozen=True)]

    # Path to the app exe
    process_path: Annotated[FilePath, Field(frozen=True), AfterValidator(is_executable)]

    # The working directory of the exe
    working_path: Annotated[DirectoryPath, Field(frozen=True)]

    # The app's database location (.sdf)
    existing_database_path: Annotated[FilePath, Field(frozen=True)]

    # The location of the database backup
    backup_database_path: Annotated[FilePath, Field(frozen=True)]

    # The path to the plugin exe
    plugin_path: Annotated[FilePath, Field(frozen=True), AfterValidator(is_executable)]

    # The path to the plugin working directory
    plugin_working_directory: Annotated[DirectoryPath, Field(frozen=True)]

    # The path to where the output json file should be stored
    plugin_output_path: Annotated[Path, Field(frozen=True)]

    @classmethod
    def is_device_installed(cls, config_name='config.ini', device_name='audiometer'):
        return super().is_device_installed(config_name, device_name)