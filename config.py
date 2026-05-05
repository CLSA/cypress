import configparser
import os

from pathlib import Path

from pydantic import BaseModel


def is_executable(path: Path):
    if not os.access(path, os.X_OK):
        raise ValueError(f"{path} is not executable")

    return path


class DeviceConfig(BaseModel):

    @classmethod
    def is_device_installed(cls, config_name, device_name):
        try:
            cls.from_ini(config_name, device_name)
            return True
        except Exception as e:
            print(e)
            return False

    @classmethod
    def from_ini(cls, ini_file_path: Path | str, section: str):
        config = configparser.ConfigParser()

        if not Path(ini_file_path).exists():
            raise FileNotFoundError(f"Config file not found: {ini_file_path}")

        config.read(ini_file_path)

        if section not in config:
            raise ValueError(f"Section {section} not found in {ini_file_path}")

        settings = config[section]
        raw_settings = dict(settings.items())

        return cls(**raw_settings)
