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
    section_name: ClassVar[str] = ""

    @classmethod
    def from_ini(cls, path: Path ="config.ini"):
        try:
            config = configparser.ConfigParser()

            if not Path(path).exists():
                raise FileNotFoundError(f"Config file not found: {str(path.resolve())}")

            config.read(path)

            if cls.section_name not in config:
                raise ValueError(f"Section {cls.section_name} not found in {str(path.resolve())}")

            settings = config[cls.section_name]
            raw_settings = dict(settings.items())

            return cls(**raw_settings), []

        except ValidationError as e:
            errors = e.errors()
            return None, errors



class CypressConfig(BaseConfig):
    # Section in .ini
    section_name: ClassVar[str] = "cypress"

    # Client HTTP server host
    host: str

    # Client HTTP server port
    port: Annotated[int, PositiveInt]

    # Comma delimited list of hosts to allow requests from
    allowed_hosts: str

    log_level: str = "info"

    # Pine authentication token
    auth: str

    # Client SSL
    ssl_certfile: FilePath | None
    ssl_keyfile: FilePath | None

    name: str
    exe: FilePath


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
if errors:
    print(errors)