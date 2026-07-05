from typing import Annotated, ClassVar

from pydantic import (
    Field,
    DirectoryPath,
    FilePath,
    AfterValidator
)

from config import DeviceConfig, is_executable

from devices.general_proxy.settings import DEVICE_NAME


class GeneralProxyConfig(DeviceConfig):
    section_name: ClassVar[str] = DEVICE_NAME

    # Adobe
    #executable: Annotated[FilePath, Field(frozen=True), AfterValidator(is_executable)]
    #directory: Annotated[DirectoryPath, Field(frozen=True)]

    # PDFTK
    #pdftk: Annotated[FilePath, Field(frozen=True), AfterValidator(is_executable)]

