from instruments.model import Model


from dicom.receiver import DicomFileInfo

class DXAModel(Model):
    def __init__(self, session):
        super().__init__(session)

    def update_files(self, files: list[DicomFileInfo]):
        self.files = [f.file_path for f in files]

