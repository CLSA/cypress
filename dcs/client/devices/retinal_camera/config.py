from typing import Annotated, ClassVar

from pydantic import Field, DirectoryPath, FilePath, AfterValidator

from config import DeviceConfig, is_executable

from devices.retinal_camera.settings import DEVICE_NAME

class RetinalCameraConfig(DeviceConfig):
    section_name: ClassVar[str] = DEVICE_NAME

    process_name: Annotated[str, Field(min_length=5, frozen=True)]

    data_path: Annotated[DirectoryPath, Field(frozen=True)]

    database_name: Annotated[str, Field(frozen=True)]

    database_backup: Annotated[FilePath, Field(frozen=True)]


if __name__ == "__main__":
    config, errors = RetinalCameraConfig.from_ini()
    print(errors)