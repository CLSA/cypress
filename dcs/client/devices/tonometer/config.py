from typing import Annotated, ClassVar

from pydantic import Field, DirectoryPath, FilePath, AfterValidator

from config import DeviceConfig, is_executable

from devices.tonometer.settings import DEVICE_NAME

class TonometerConfig(DeviceConfig):
    section_name: ClassVar[str] = DEVICE_NAME

    process_name: Annotated[str, Field(min_length=5)]

    executable: Annotated[FilePath, Field(frozen=True), AfterValidator(is_executable)]

    working_directory: Annotated[DirectoryPath, Field(frozen=True)]

    directory: Annotated[DirectoryPath, Field(frozen=True)]

    database: Annotated[FilePath, Field(frozen=True)]

    backup_database: Annotated[FilePath, Field(frozen=True)]

