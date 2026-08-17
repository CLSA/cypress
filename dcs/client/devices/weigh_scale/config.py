from typing import Annotated, ClassVar, Literal

from pydantic import Field

from config import DeviceConfig

from devices.weigh_scale.settings import DEVICE_NAME


class WeighScaleConfig(DeviceConfig):
    section_name: ClassVar[str] = DEVICE_NAME

    port_name: str

    locale: Literal["en", "fr"]
