import copy
import tempfile
import zipfile
import json

from pathlib import Path

from session import Session

from utils import get_file_info, FileInfo


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

    def add_file(self, file_path: Path, send_name: str) -> bool:
        if not file_path.exists() or not file_path.is_file():
            return False
        try:
            file_info = get_file_info(file_path)
            file_info.send_name = send_name
            self.files.append(file_info)
        except (FileNotFoundError, PermissionError):
            return False
        except Exception as e:
            return False
        return True

    def _add_file(self, file_info: FileInfo):
        self.files.append(file_info)

    def to_response(self):
        response = {
            "value": {
                "session": self.session.model_dump(mode="json") if self.session else {},
                "manual_entry": self.manual_entry,
                "metadata": copy.deepcopy(self.metadata),
                "results": copy.deepcopy(self.results),
                "files": {
                    f"{file_info.send_name}{file_info.extension.replace(".", "_")}": file_info.readable_size
                    for file_info in self.files
                },
            }
        }

        return response

    def to_zip(self, file_path) -> bool:
        try:
            with tempfile.TemporaryFile(mode="w+", delete=False) as temp_file:
                json.dump(self.to_response(), temp_file, indent=4)

            with zipfile.ZipFile(file_path, "w", zipfile.ZIP_DEFLATED) as backup_zip:
                backup_zip.write(temp_file.name, arcname="response.json")
                for data_file in self.files:
                    backup_zip.write(
                        data_file.file_path,
                        arcname=f"{data_file.send_name}{data_file.extension}",
                    )

        except Exception as e:
            print(e)
            return False

        finally:
            Path(temp_file.name).unlink(missing_ok=True)

    @classmethod
    def class_name(cls):
        return cls.__name__
