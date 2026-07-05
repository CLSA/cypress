from typing import Annotated, ClassVar

from pydantic import Field, DirectoryPath, FilePath, AfterValidator

from config import DeviceConfig, is_executable

from devices.grip_strength.settings import DEVICE_NAME

class GripStrengthConfig(DeviceConfig):
    section_name: ClassVar[str] = DEVICE_NAME

    # The name of the process as it appears in Task Manager
    process_name: Annotated[str, Field(min_length=5, frozen=True)]

    # The path to the exe
    #executable: Annotated[FilePath, Field(frozen=True), AfterValidator(is_executable)]

    # Path to the app's working directory
    #directory: Annotated[DirectoryPath, Field(frozen=True)]

    # Where we store the clean database
    #backup_path: Annotated[str, Field(frozen=True)]

    # Where the database files are located
    #database_path: Annotated[DirectoryPath, Field(frozen=True)]

    # The name of the grip test database
    #grip_test_db: Annotated[str, Field(min_length=1, frozen=True)]

    # The name of the grip test database that stores the data
    #grip_test_data_db: Annotated[str, Field(min_length=1, frozen=True)]
