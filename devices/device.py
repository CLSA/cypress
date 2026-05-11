from PySide6.QtWidgets import QApplication
from PySide6.QtGui import QIcon

from session import Session, SessionDialog
from config import DeviceConfig

from devices.model import Model
from devices.view import View
from devices.controller import Controller


class Device:
    config: DeviceConfig
    model: Model
    view: View
    controller: Controller
    session: Session
    session_dialog: SessionDialog

    @classmethod
    def is_installed(cls) -> bool:
        return cls.config.is_device_installed()

    @classmethod
    def run(cls, session: Session | None):
        app = QApplication()

        config = cls.config.from_ini()

        detached = not session
        if not detached:
            session = cls.session(answer_id=1, **cls.session_dialog().prompt())

        model = cls.model(session=session)
        controller = cls.controller(
            config=config, session=session, model=model, detached=detached
        )
        view = cls.view(controller=controller, session=session)
        view.show()

        return app.exec()