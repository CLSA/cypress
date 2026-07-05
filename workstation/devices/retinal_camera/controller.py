from controller import Controller

from devices.retinal_camera.session import RetinalCameraSession
from devices.retinal_camera.config import RetinalCameraConfig
from devices.retinal_camera.model import RetinalCameraModel
from devices.retinal_camera.view import RetinalCameraView


class RetinalCameraController(Controller):
    def __init__(
        self,
        session: RetinalCameraSession,
        config: RetinalCameraConfig,
        model: RetinalCameraModel,
        view: RetinalCameraView,
        detached: bool = False,
        parent=None,
    ):
        super().__init__(
            parent=parent,
            session=session,
            config=config,
            model=model,
            view=view,
            detached=detached,
        )
