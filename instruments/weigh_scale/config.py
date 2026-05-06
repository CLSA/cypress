from typing import Annotated

from pydantic import Field, DirectoryPath, FilePath, AfterValidator

from config import DeviceConfig, is_executable

class WeighScaleConfig(DeviceConfig):
    port_name: Annotated[str, Field(min_length=4)]

    @classmethod
    def is_device_installed(cls, config_name="config.ini", device_name="weigh_scale"):
        return super().is_device_installed(config_name, device_name)

    @classmethod
    def from_ini(cls, ini_file_path="config.ini", section="weigh_scale"):
        return super().from_ini(ini_file_path, section)