import json

from typing import Annotated, ClassVar

from pydantic import Field, DirectoryPath, FilePath, AfterValidator

from config import DeviceConfig, is_executable

from devices.spirometer.settings import DEVICE_NAME


class SpirometerConfig(DeviceConfig):
    section_name: ClassVar[str] = DEVICE_NAME

    process_name: Annotated[str, Field(min_length=5)]

    executable: Annotated[FilePath, Field(frozen=True), AfterValidator(is_executable)]
    working_directory: Annotated[DirectoryPath, Field(frozen=True)]

    exchange_path: Annotated[DirectoryPath, Field(frozen=True)]
    database_path: Annotated[FilePath, Field(frozen=True)]

    backup_directory: Annotated[DirectoryPath, Field(frozen=True)]
    backup_database: Annotated[FilePath, Field(frozen=True)]
    backup_options: Annotated[FilePath, Field(frozen=True)]

    in_file_name: Annotated[str, Field(min_length=3)]
    out_file_name: Annotated[str, Field(min_length=3)]


if __name__ == "__main__":
    config, errors = SpirometerConfig.from_ini()
    print(config)
    print(json.dumps(errors, indent=4))
