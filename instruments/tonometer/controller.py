from instruments.controller import Controller



class TonometerController(Controller):
    def __init__(self, parent=None, session_data: dict = {}):
        super().__init__(parent=parent, session_data=session_data)

    @staticmethod
    def is_installed():
        return True
