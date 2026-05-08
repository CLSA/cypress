from typing import Annotated
from pydantic import Field, DirectoryPath, FilePath, AfterValidator
from config import DeviceConfig, is_executable

class DXAConfig(DeviceConfig):
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

    @classmethod
    def is_device_installed(cls, config_name='config.ini', device_name='dxa'):
        return super().is_device_installed(config_name, device_name)

    @classmethod
    def from_ini(cls, ini_file_path='config.ini', section='dxa'):
        return super().from_ini(ini_file_path, section)