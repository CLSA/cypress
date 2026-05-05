from config import DeviceConfig

from typing import Annotated

from pydantic import DirectoryPath, Field


class ECGConfig(DeviceConfig):
    storage_path: Annotated[DirectoryPath, Field(frozen=True)]

    @classmethod
    def is_device_installed(cls, config_name='config.ini', device_name='ecg'):
        return super().is_device_installed(config_name, device_name)

    @classmethod
    def from_ini(cls, ini_file_path='config.ini', section='ecg'):
        return super().from_ini(ini_file_path, section)