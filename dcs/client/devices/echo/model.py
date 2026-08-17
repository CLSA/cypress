import logging
import pydicom

from pathlib import Path
from typing import override

from model import Model

from devices.echo.session import ECHOSession
from devices.echo.config import ECHOConfig

from utils import get_file_info, FileInfo

logger = logging.getLogger("echo")

class ECHOModel(Model):
    def __init__(self, session: ECHOSession, config: ECHOConfig):
        super().__init__(session=session, config=config)

    def is_valid(self):
        try:
            for file_info in self.files:
                logger.debug(f"validating {file_info.file_path.name}...")
                pydicom.dcmread(file_info.file_path)
        except pydicom.errors.InvalidDicomError as e:
            logger.error(e)
            return False
        return len(self.files) >= 1

    @override
    def add_file(self, file_path: Path, send_name: str) -> tuple[bool, str | None]:
        try:
            file_info = get_file_info(file_path)
            file_info.send_name = send_name
            self.files.append(file_info)
        except Exception as e:
            return False, e

        valid, result = self._check_file(file_info)
        if not valid:
            return False, result

        self.results.append(result)
        return True, None


    def _check_file(self, file_info: Path) -> tuple[bool, str | dict]:
        try:
            ds = pydicom.dcmread(file_info.file_path, stop_before_pixels=False)
            # if ds["PatientID"].value != self.session.barcode:
            #     return (
            #         False,
            #         f"PatientID: {ds["PatientID"].value} does not match Participant ID: {self.session.barcode}",
            #     )

            res = {}
            res["name"] = file_info.file_path.name
            res["size"] = file_info.readable_size
            res["path"] = str(file_info.file_path.resolve())
            res["instance_number"] = ds["InstanceNumber"].value
            res["patient_id"] = ds["PatientID"].value
            res["study_id"] = ds["StudyInstanceUID"].value

            return True, res

        except FileNotFoundError:
            return False, f"File not found: {file_info.file_path.name}"
        except PermissionError:
            return False, f"Permission error: {file_info.file_path.name}"
        except pydicom.errors.InvalidDicomError:
            return False, f"Invalid DICOM file: {file_info.file_path.name}"
        except Exception as e:
            return False, e

    @override
    def to_response(self):
        response = super().to_response()
        return response
