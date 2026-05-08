from typing import Annotated

from pydantic import Field, DirectoryPath, FilePath, AfterValidator

from config import DeviceConfig, is_executable


class CRTConfig(DeviceConfig):
    # The name of the process as it appears in Task Manager
    process_name: Annotated[str, Field(min_length=1, frozen=True)]

    # The path to the exe
    executable: Annotated[FilePath, Field(frozen=True), AfterValidator(is_executable)]

    # Path to the app's working directory
    directory: Annotated[DirectoryPath, Field(frozen=True)]

    # Where the instrument results are stored
    output: Annotated[DirectoryPath, Field(frozen=True)]

    # Program argument
    prefix: Annotated[str, Field(min_length=1, frozen=True)]

    # Program argument
    clinic: Annotated[str, Field(min_length=1, frozen=True)]

    @classmethod
    def is_device_installed(cls, config_name='config.ini', device_name='choice_reaction'):
        return super().is_device_installed(config_name, device_name)

    @classmethod
    def from_ini(cls, ini_file_path='config.ini', section='choice_reaction'):
        return super().from_ini(ini_file_path, section)
