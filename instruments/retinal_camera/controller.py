from instruments.controller import Controller

from instruments.retinal_camera.session import RetinalCameraSession
from instruments.retinal_camera.config import RetinalCameraConfig
from instruments.retinal_camera.model import RetinalCameraModel


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
