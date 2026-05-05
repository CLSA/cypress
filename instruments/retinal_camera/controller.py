from instruments.controller import Controller

class RetinalCameraController(Controller):
    def __init__(self, parent=None, session_data: dict = {}):
        super().__init__(parent, session_data)