from instruments.view import View
from instruments.controller import Controller

class RetinalCameraView(View):
    def __init__(self, parent=None, title="Retinal Camera", controller_cls = Controller, session_data = {}):
        super().__init__(parent, title, controller_cls, session_data)