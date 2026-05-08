from typing import Annotated

from pydantic import Field, DirectoryPath, FilePath, AfterValidator

from config import DeviceConfig, is_executable

class SpirometerConfig(DeviceConfig):
    process_name: Annotated[str, Field(min_length=5)]

    @classmethod
    def is_device_installed(cls, config_name="config.ini", device_name="spirometer"):
        return super().is_device_installed(config_name, device_name)

    @classmethod
    def from_ini(cls, ini_file_path="config.ini", section="spirometer"):
        return super().from_ini(ini_file_path, section)