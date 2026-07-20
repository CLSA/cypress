from typing import Annotated, ClassVar

from pydantic import Field, DirectoryPath, FilePath, AfterValidator

from config import DeviceConfig, is_executable

from devices.general_proxy.settings import DEVICE_NAME


class GeneralProxyConfig(DeviceConfig):
    section_name: ClassVar[str] = DEVICE_NAME

    adobe_executable: Annotated[
        FilePath, Field(frozen=True), AfterValidator(is_executable)
    ]
    adobe_working_dir: Annotated[DirectoryPath, Field(frozen=True)]

    pdftk_executable: Annotated[
        FilePath, Field(frozen=True), AfterValidator(is_executable)
    ]

    form_en: Annotated[FilePath, Field(frozen=True)]
    fdf_en: Annotated[FilePath, Field(frozen=True)]

    form_fr: Annotated[FilePath, Field(frozen=True)]
    fdf_fr: Annotated[FilePath, Field(frozen=True)]

    output_base_dir: Annotated[DirectoryPath, Field(frozen=True)]


if __name__ == '__main__':
    import json

    config, errors = GeneralProxyConfig.from_ini()
    print(json.dumps(errors, indent=4))