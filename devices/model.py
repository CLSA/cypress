import copy
from pathlib import Path

from session import Session

from devices.utils import get_file_size, get_file_info

from files.receiver import FileInfo

from config import DeviceConfig


class Model:

    def __init__(self, session: Session, config: DeviceConfig):
        self.session = session
        self.config = config

        self.metadata = {}
        self.results = []
        self.files: list[FileInfo] = []

        self.manual_entry = False

    def reset(self):
        self.metadata = {}
        self.output = []
        self.results = []
        self.files = []

        self.manual_entry = False

    def _add_file(self, file_info: FileInfo):
        self.files.append(file_info)

    def to_response(self):
        response = {
            "value": {
                "session": self.session.model_dump(mode="json"),
                "manual_entry": self.manual_entry,
                "metadata": copy.deepcopy(self.metadata),
                "results": copy.deepcopy(self.results),
                "files": {
                    f"{file_info.send_name}_{file_info.extension}": file_info.readable_size
                    for file_info in self.files
                },
            }
        }

        return response

    @classmethod
    def class_name(cls):
        return cls.__name__
