from devices.controller import Controller

from devices.retinal_camera.session import RetinalCameraSession
from devices.retinal_camera.config import RetinalCameraConfig
from devices.retinal_camera.model import RetinalCameraModel


class RetinalCameraController(Controller):
    def __init__(
        self,
        session: RetinalCameraSession,
        config: RetinalCameraConfig,
        model: RetinalCameraModel,
        standalone: bool = False,
        parent=None,
    ):
        super().__init__(
            session=session,
            config=config,
            model=model,
            standalone=standalone,
            parent=parent,
        )
