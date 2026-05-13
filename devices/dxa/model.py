from devices.model import Model

from devices.utils import DicomFileInfo

class DXAModel(Model):
    def __init__(self, session, config):
        super().__init__(session, config)

    def update_files(self, files: list[DicomFileInfo]):
        self.files = [f.file_path for f in files]

