from session import Session, SessionDialog


class CDTTSession(Session):
    pass


class CDTTSessionDialog(SessionDialog):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.setWindowTitle("CDTT setup")

        self.setFixedWidth(350)
        self.setFixedHeight(350)
