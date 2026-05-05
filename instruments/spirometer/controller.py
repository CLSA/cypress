from instruments.controller import Controller

import configparser

config = configparser.ConfigParser()

config['spirometry'] = {
    'executable': ''
}

class SpirometerController(Controller):
    def __init__(self, parent=None, session_data: dict = {}):
        super().__init__(parent, session_data)