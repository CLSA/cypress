from pathlib import Path

from pydicom import dcmread

from model import Model

from devices.echo.session import ECHOSession
from devices.echo.config import ECHOConfig

from utils import get_file_size, get_file_info


class ECHOModel(Model):
    def __init__(self, session: ECHOSession, config: ECHOConfig):
        super().__init__(session=session, config=config)


    def read_results(self, storage_path: Path) -> tuple[bool, str | None]:
        self.reset()

        if not storage_path.exists() or not storage_path.is_dir():
            return False, "Storage directory does not exist"

        for file_path in storage_path.iterdir():
            if file_path.is_file() and file_path.exists():
                if file_path.suffix.lower() == '.dcm':
                    is_valid_file, error = self._validate_file(file_path)
                    if not is_valid_file:
                        return False, error

                    result_dict, error = self._get_dcm_metadata(file_path)
                    if not result_dict:
                        return False, error

                    self.results.append(result_dict)

                    self.add_file(file_path, file_path.name.replace(".", "_"))
                else:
                    return False, "Unknown file found"

        return True, None

    def _get_dcm_metadata(self, file_path: Path) -> tuple[dict | None, str | None]:
        try:
            file_info = get_file_info(file_path)
            ds = dcmread(file_path, stop_before_pixels=True)

            res = {}
            res['name'] = file_info.file_path.name
            res['size'] = file_info.readable_size
            res['path'] = str(file_info.file_path.resolve())
            res['instance_number'] = ds['InstanceNumber'].value
            res['patient_id'] = ds['PatientID'].value
            res['study_id'] = ds['StudyInstanceUID'].value

            return res, None

        except Exception as e:
            return None, e

    def _validate_file(self, file_path: Path) -> tuple[bool, str | None]:
        try:
            ds = dcmread(file_path, stop_before_pixels=True)
            if ds["PatientID"].value != self.session.barcode:
                return False, f"File ID {ds["PatientID"].value} does not match barcode {self.session.barcode}"
            return True, None
        except Exception as e:
            return False, e