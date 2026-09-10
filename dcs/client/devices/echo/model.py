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
            if len(self.files) < 3:
                return False, "Not enough files"

            has_us = False
            has_usm = False
            has_src = False

            for file_info in self.files:
                logger.debug(f"validating {file_info.file_path.name}...")

                if "US." in file_info.file_path.name:
                    has_us = True
                elif "USm." in file_info.file_path.name:
                    has_usm = True
                elif "SRc." in file_info.file_path.name:
                    has_src = True
                else:
                    logger.error(f"unknown file: {file_info.file_path.name}")
                    return False, "Unknown file found"

                valid, error = self._check_file(file_info)
                if not valid:
                    return False, error

            if not has_us:
                return False, "No US file found"

            if not has_usm:
                return False, "No USm file found"

            if not has_src:
                return False, "No SRc file found"

        except Exception as e:
            logger.error(e)
            return False, "Something went wrong"

        return True, None

    def set_scans(self, scans_received):
        self.reset()

        for file_path in scans_received:
            self.add_file(file_path, send_name=file_path.stem, ext=".dcm")

        for file_info in self.files:
            ds = pydicom.dcmread(file_info.file_path, stop_before_pixels=True)

            res = {}
            res["name"] = file_info.file_path.name
            res["size"] = file_info.readable_size
            res["path"] = str(file_info.file_path.resolve())
            res["instance_number"] = ds["InstanceNumber"].value
            res["patient_id"] = ds["PatientID"].value
            res["study_id"] = ds["StudyInstanceUID"].value
            self.results.append(res)

    def _check_file(self, file_info: FileInfo) -> tuple[bool, str | None]:
        if not any(file_type in file_info.file_path.name for file_type in ["US.", "SRc.", "USm."]):
            return False, "Unknown file found"

        ds = pydicom.dcmread(file_info.file_path, stop_before_pixels=False)
        if ds["PatientID"].value != self.session.barcode:
            return False, f"Patient ID does not match {self.session.barcode}"

        return True, None

    @override
    def to_response(self):
        response = super().to_response()

        results = response["value"]["results"]
        results = sorted(results, key=lambda result: int(result["instance_number"]))
        response["value"]["results"] = results

        return response
