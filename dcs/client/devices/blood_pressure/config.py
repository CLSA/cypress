from config import DeviceConfig

from typing import Annotated, ClassVar
from pydantic import (
    Field,
    DirectoryPath,
    FilePath,
    AfterValidator
)

from config import DeviceConfig, is_executable

from devices.blood_pressure.settings import DEVICE_NAME

class BPConfig(DeviceConfig):
    section_name: ClassVar[str] = DEVICE_NAME

    process_name: Annotated[str, Field(min_length=1, frozen=True)]

    executable: Annotated[FilePath, Field(frozen=True), AfterValidator(is_executable)]

    directory: Annotated[DirectoryPath, Field(frozen=True)]

    database: Annotated[FilePath, Field(frozen=True)]

    backup_database: Annotated[FilePath, Field(frozen=True)]