from typing import Annotated, ClassVar

from pydantic import Field, DirectoryPath, FilePath, AfterValidator

from config import DeviceConfig, is_executable

from devices.tonometer.settings import DEVICE_NAME

class TonometerConfig(DeviceConfig):
    section_name: ClassVar[str] = DEVICE_NAME


    process_name: Annotated[str, Field(min_length=5)]