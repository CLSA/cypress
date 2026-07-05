from typing import Annotated, ClassVar

from pydantic import Field, DirectoryPath, FilePath, AfterValidator

from config import DeviceConfig, is_executable

from devices.frax.settings import DEVICE_NAME


class FRAXConfig(DeviceConfig):
    section_name: ClassVar[str] = DEVICE_NAME

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