import sys

from PySide6.QtWidgets import (
    QApplication,
    QFormLayout,
    QComboBox,
    QGroupBox,
    QDoubleSpinBox,
    QCheckBox,
    QDateEdit,
)

from PySide6.QtCore import QDate

from session import SessionDialog
from instruments.ecg.config import ECGConfig
from instruments.ecg.session import ECGSession
from instruments.ecg.controller import ECGController
from instruments.ecg.view import ECGView
from instruments.ecg.model import ECGModel


class ECGSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.setWindowTitle("ECG setup")
        self.setMinimumSize(350, 350)


def run_ecg(session: ECGSession | None):
    app = QApplication()

    try:
        config = ECGConfig.from_ini()
    except Exception as e:
        print(e)
        return -1

    standalone = not session
    if standalone:
        session = ECGSession(answer_id=1, **ECGSessionDialog().prompt())

    model = ECGModel(session=session)
    controller = ECGController(
        config=config, session=session, model=model, standalone=False
    )
    view = ECGView(controller=controller, session=session)
    view.show()

    return app.exec()


if __name__ == '__main__':
    try:
        sys.exit(run_ecg(session=None))
    except Exception as e:
        print(e)
        input("Press enter to continue...")
        sys.exit(-1)
