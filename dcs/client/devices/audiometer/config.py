from typing import Annotated, ClassVar

from pathlib import Path

from pydantic import (
    Field,
    DirectoryPath,
    FilePath,
    AfterValidator
)

from config import DeviceConfig, is_executable
from devices.audiometer.settings import DEVICE_NAME


class AudiometerConfig(DeviceConfig):
    section_name: ClassVar[str] = DEVICE_NAME

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