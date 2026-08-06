import time
import logging
import logging.config
import json

from PySide6.QtWidgets import QApplication
from PySide6.QtGui import QIcon
from PySide6.QtCore import Qt

from session import Session, SessionDialog
from config import DeviceConfig

from model import Model
from view import View
from controller import Controller


class Device:
    name: str
    config: DeviceConfig
    logging_config: dict
    model: Model
    view: View
    controller: Controller
    session: Session
    session_dialog: SessionDialog

    @classmethod
    def is_installed(cls) -> bool:
        return cls.config.is_device_installed()

    @classmethod
    def get_install_errors(cls) -> list[str]:
        config, errors = cls.config.from_ini()
        return errors

    @classmethod
    def run(cls, session: Session | None = None):
        start = time.perf_counter()

        app = QApplication()

        color_scheme = app.styleHints().colorScheme()

        #if color_scheme == Qt.ColorScheme.Light:
        #app.setStyle("windowsvista")


        #app.styleHints().setColorScheme(Qt.ColorScheme.Light)

        if color_scheme == Qt.ColorScheme.Light:
            app.setStyleSheet(
                """
                QGroupBox {
                    border-color: lightgray;
                }

                QWidget {
                    outline: none;
                }
                """
            )

        config, errors = cls.config.from_ini()
        if not config or errors:
            print(json.dumps(errors, indent=4))
            return 1

        logging.config.dictConfig(cls.logging_config)
        logger = logging.getLogger(config.section_name)

        detached = not session
        if detached:
            try:
                session = cls.session(answer_id=1, **cls.session_dialog().prompt())
            except Exception as e:
                print(e)
                logger.info("session info not entered, exiting")
                return 1

        logger.info(
            f"launching {config.section_name} ({f"{session.origin}" if not detached else "detached"})"
        )

        model = cls.model(session=session, config=config)
        view = cls.view(session=session, config=config, detached=detached)
        controller = cls.controller(
            config=config, session=session, model=model, view=view, detached=detached
        )

        end = time.perf_counter()

        logger.debug(f"{config.section_name}: {end - start}s")

        view.show()

        return app.exec()
