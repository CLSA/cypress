import copy

from session import Session

from instruments.utils import get_file_size
from dicom.receiver import FileInfo


class Model:
    def __init__(self, session: Session):
        self.session = session
        self.metadata = {}
        self.results = []
        self.files = []
        self.manual_entry = False

    def reset(self):
        self.metadata = {}
        self.output = []
        self.results = []
        self.files = []

    def add_file(self, file_info: FileInfo):
        if not file_info:
            return

        self.files.append(file_info)

    def to_response(self):
        response = {
            "session": self.session.model_dump(mode="json"),
            "manual_entry": self.manual_entry,
            "metadata": copy.deepcopy(self.metadata),
            "results": copy.deepcopy(self.results),
        }

        if self.files:
            response["files"] = {
                file_info.name: get_file_size(file_info.file_path)
                for file_info in self.files
            }

        return response
