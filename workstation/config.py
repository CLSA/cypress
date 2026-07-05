import configparser
import os

from pathlib import Path
from typing import Annotated, ClassVar

from pydantic import BaseModel, IPvAnyAddress, PositiveInt, HttpUrl, FilePath, ValidationError


def is_executable(path: Path):
    if not os.access(path, os.X_OK):
        raise ValueError(f"{path} is not executable")

    return path


class BaseConfig(BaseModel):
    config_name: ClassVar[str] = "config.ini"
    section_name: ClassVar[str] = ""

    @classmethod
    def from_ini(cls):
        try:
            config = configparser.ConfigParser()

            if not Path(cls.config_name).exists():
                raise FileNotFoundError(f"Config file not found: {cls.config_name}")

            config.read(cls.config_name)

            if cls.section_name not in config:
                raise ValueError(f"Section {cls.section_name} not found in {cls.config_name}")

            settings = config[cls.section_name]
            raw_settings = dict(settings.items())

            return cls(**raw_settings), []

        except ValidationError as e:
            errors = e.errors()
            return None, errors



class CypressConfig(BaseConfig):
    # Section in .ini
    section_name: ClassVar[str] = "cypress"

    # HTTP server host
    host: str

    # HTTP server port
    port: Annotated[int, PositiveInt]

    # Comma delimited list of hosts to allow requests from
    allowed_hosts: str

    log_level: str = "info"

    # Pine authentication token
    auth: str

    ssl_certfile: FilePath | None
    ssl_keyfile: FilePath | None


class DeviceConfig(BaseConfig):
    width: int = 800
    height: int = 600

    @classmethod
    def is_device_installed(cls):
        config, errors = cls.from_ini()
        if not config or errors:
            return False
        return True

config, errors = CypressConfig.from_ini()
print(errors)