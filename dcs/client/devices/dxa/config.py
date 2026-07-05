from typing import Annotated, ClassVar
from pydantic import Field, DirectoryPath, FilePath, AfterValidator

from config import DeviceConfig, is_executable

from devices.dxa.settings import DEVICE_NAME

class DXAConfig(DeviceConfig):
    section_name: ClassVar[str] = DEVICE_NAME

    # DICOM receiver settings
    executable: Annotated[
        FilePath, Field(frozen=True), AfterValidator(is_executable)
    ]

    directory: Annotated[DirectoryPath, Field(frozen=True)]

    ae_title: Annotated[str, Field(min_length=8, frozen=True)]

    host: Annotated[str, Field(min_length=7, frozen=True)]

    port: Annotated[int, Field(min=0)]

    storescp_config: Annotated[FilePath, Field(frozen=True)]

    storage_path: Annotated[DirectoryPath, Field(frozen=True)]

    # Apex
    patscan_db_path: Annotated[FilePath, Field(frozen=True)]
    reference_db_path: Annotated[FilePath, Field(frozen=True)]