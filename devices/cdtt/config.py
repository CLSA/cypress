from typing import Annotated, ClassVar

from pydantic import (
    Field,
    DirectoryPath,
    FilePath,
    AfterValidator,
)

from config import DeviceConfig, is_executable

from devices.cdtt.settings import DEVICE_NAME


class CDTTConfig(DeviceConfig):
    # ini section
    section_name: ClassVar[str] = DEVICE_NAME

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
