from model import Model


class ECGModel(Model):
    def __init__(self, session, config):
        super().__init__(session, config)
