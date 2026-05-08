import sys

from PySide6.QtWidgets import (
    QApplication,
)

from session import SessionDialog
from devices.echo.config import ECHOConfig
from devices.echo.session import ECHOSession
from devices.echo.controller import ECHOController
from devices.echo.view import ECHOView
from devices.echo.model import ECHOModel


class ECHOSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.setWindowTitle("ECHO setup")
        self.setMinimumSize(350, 350)


def run_echo(session: ECHOSession | None):
    app = QApplication()

    config = ECHOConfig.from_ini()

    standalone = not session
    if standalone:
        session = ECHOSession(answer_id=1, **ECHOSessionDialog().prompt())

    model = ECHOModel(session=session)
    controller = ECHOController(
        config=config, session=session, model=model, standalone=standalone
    )
    view = ECHOView(controller=controller, session=session)
    view.show()

    return app.exec()


if __name__ == '__main__':
    try:
        sys.exit(run_echo(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)