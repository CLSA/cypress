from config import DeviceConfig

from typing import Annotated
from pydantic import (
    Field,
    DirectoryPath,
    FilePath,
    AfterValidator
)

from config import DeviceConfig, is_executable

class BPConfig(DeviceConfig):
    process_name: Annotated[str, Field(min_length=1, frozen=True)]
    executable: Annotated[FilePath, Field(frozen=True), AfterValidator(is_executable)]
    directory: Annotated[DirectoryPath, Field(frozen=True)]
    database_path: Annotated[FilePath, Field(frozen=True)]
    backup_database_path: Annotated[FilePath, Field(frozen=True)]

    @classmethod
    def is_device_installed(cls, config_name='config.ini', device_name='blood_pressure'):
        return super().is_device_installed(config_name, device_name)

    @classmethod
    def from_ini(cls, ini_file_path='config.ini', section='blood_pressure'):
        return super().from_ini(ini_file_path, section)
