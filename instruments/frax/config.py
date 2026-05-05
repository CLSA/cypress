from typing import Annotated

from pydantic import Field, DirectoryPath, FilePath, AfterValidator

from config import DeviceConfig, is_executable


class FRAXConfig(DeviceConfig):
    # The name of the process as it appears in Task Manager
    process_name: Annotated[str, Field(min_length=1, frozen=True)]

    # The path to the exe
    executable: Annotated[FilePath, Field(frozen=True), AfterValidator(is_executable)]

    # Path to the app's working directory
    directory: Annotated[DirectoryPath, Field(frozen=True)]

    # Where to write the input
    input_file: Annotated[str, Field(min_length=1, frozen=True)]

    # Where to read the output
    output_file: Annotated[str, Field(min_length=1, frozen=True)]

    # Program argument
    country_code: Annotated[str, Field(min_length=2, max_length=2, frozen=True)]

    # Program argument
    type_code: Annotated[str, Field(min_length=1, max_length=1, frozen=True)]

    @classmethod
    def is_device_installed(cls, config_name="config.ini", device_name="frax"):
        return super().is_device_installed(config_name, device_name)

    @classmethod
    def from_ini(cls, ini_file_path="config.ini", section="frax"):
        return super().from_ini(ini_file_path, section)
