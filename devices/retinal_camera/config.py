from typing import Annotated

from pydantic import Field, DirectoryPath, FilePath, AfterValidator

from config import DeviceConfig, is_executable

class RetinalCameraConfig(DeviceConfig):
    # process_name: Annotated[str, Field(min_length=5, frozen=True)]
    # executable: Annotated[FilePath, Field(frozen=True), AfterValidator(is_executable)]
    # directory: Annotated[DirectoryPath, Field(frozen=True)]
    # backup_database: Annotated[FilePath, Field(frozen=True)]

    @classmethod
    def is_device_installed(cls, config_name="config.ini", device_name="retinal_camera"):
        return super().is_device_installed(config_name, device_name)

    @classmethod
    def from_ini(cls, ini_file_path="config.ini", section="retinal_camera"):
        return super().from_ini(ini_file_path, section)