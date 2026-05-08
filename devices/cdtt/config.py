from pathlib import Path

from typing import Annotated

from pydantic import (
    Field,
    DirectoryPath,
    FilePath,
    AfterValidator,
)

from config import DeviceConfig, is_executable


class CDTTConfig(DeviceConfig):
    # The name of the process as it appears in Task Manager
    process_name: Annotated[str, Field(min_length=1, frozen=True)]

    # Path to the app's working directory
    directory: Annotated[DirectoryPath, Field(frozen=True)]

    # Path to the java runtime environment
    jre: Annotated[FilePath, Field(frozen=True), AfterValidator(is_executable)]

    # Path to the jar file to execute
    jar: Annotated[FilePath, Field(fronzen=True)]

    # Where the instrument results are stored
    output: Annotated[DirectoryPath, Field(frozen=True)]

    @classmethod
    def is_device_installed(cls, config_name='config.ini', device_name='cdtt'):
        return super().is_device_installed(config_name, device_name)

    @classmethod
    def from_ini(cls, ini_file_path='config.ini', section='cdtt'):
        return super().from_ini(ini_file_path, section)