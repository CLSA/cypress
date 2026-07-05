from config import DeviceConfig, ClassVar

from typing import Annotated

from pydantic import DirectoryPath, Field

from devices.ecg.settings import DEVICE_NAME


class ECGConfig(DeviceConfig):
    section_name: ClassVar[str] = DEVICE_NAME

    storage_path: Annotated[DirectoryPath, Field(frozen=True)]
