from instruments.controller import Controller

class GripStrengthController(Controller):
    def __init__(self, parent=None, session_data = {}):
        super().__init__(parent, session_data)