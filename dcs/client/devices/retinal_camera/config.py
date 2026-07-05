from typing import Annotated, ClassVar

from pydantic import Field, DirectoryPath, FilePath, AfterValidator

from config import DeviceConfig, is_executable

from devices.retinal_camera.settings import DEVICE_NAME

class RetinalCameraConfig(DeviceConfig):
    section_name: ClassVar[str] = DEVICE_NAME

    process_name: Annotated[str, Field(min_length=5, frozen=True)]