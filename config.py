import configparser
import os

from pathlib import Path
from typing import Annotated, ClassVar

from pydantic import BaseModel, IPvAnyAddress, PositiveInt, HttpUrl, FilePath


def is_executable(path: Path):
    if not os.access(path, os.X_OK):
        raise ValueError(f"{path} is not executable")

    return path


class BaseConfig(BaseModel):
    config_name: ClassVar[str] = "config.ini"
    section_name: ClassVar[str] = ""

    @classmethod
    def from_ini(cls):
        config = configparser.ConfigParser()

        if not Path(cls.config_name).exists():
            raise FileNotFoundError(f"Config file not found: {cls.config_name}")

        config.read(cls.config_name)

        if cls.section_name not in config:
            raise ValueError(f"Section {cls.section_name} not found in {cls.config_name}")

        settings = config[cls.section_name]
        raw_settings = dict(settings.items())

        return cls(**raw_settings)


class CypressConfig(BaseConfig):
    section_name: ClassVar[str] = "cypress"
    host: str
    port: Annotated[int, PositiveInt]
    allowed_hosts: str # comma delimited list
    log_level: str = "info"
    pine: HttpUrl
    auth: str
    ssl_certfile: FilePath | None
    ssl_keyfile: FilePath | None


class DeviceConfig(BaseConfig):
    @classmethod
    def is_device_installed(cls):
        try:
            cls.from_ini()
            return True
        except Exception as e:
            print(e)
            return False

config = CypressConfig.from_ini()