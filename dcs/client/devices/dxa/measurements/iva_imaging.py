from typing import override

import pydicom
import logging

from pathlib import Path

from measure import Record

from utils import get_file_size

from devices.dxa.apex.reference_db import ReferenceDB
from devices.dxa.apex.patscan_db import PatScanDB

logger = logging.getLogger("dxa")

class IVAImagingMeasurement(Record):
    field_map = {
        "NAME": {"attr": "name", "data_type": str, "units": None},

        # From ScanAnalysis
        "PATIENT_KEY": {"attr": "patient_key", "data_type": str, "units": None},
        "SERIAL_NUMBER": {"attr": "serial_number", "data_type": str, "units": None},
        "SCANID": {"attr": "scanid", "data_type": str, "units": None},
        "SCAN_TYPE": {"attr": "scan_type", "data_type": str, "units": None},
        "SCAN_MODE": {"attr": "scan_mode", "data_type": str, "units": None},
        "SCAN_DATE": {"attr": "scan_date", "data_type": str, "units": None},

        # From DicomFile
        "PATIENT_ID": {"attr": "patient_id", "data_type": str, "units": None},
        "FILE_PATH": {"attr": "file_path", "data_type": str, "units": None},
        "MEDIA_STORAGE_UID": {"attr": "media_storage_uid", "data_type": str, "units": None},
        # TODO remove STUDY_ID, corrected to study_instance_uid
        "STUDY_ID": {"attr": "study_id", "data_type": str, "units": None},
        "STUDY_INSTANCE_UID": {"attr": "study_instance_uid", "data_type": str, "units": None},
        "SIZE": {"attr": "size", "data_type": str, "units": None},
    }

    def __init__(
        self,
        raw_data,
        ot_scan_path: Path | None = None,
        pr_scan_path: Path | None = None,
        measure_scan_path: Path | None = None,
    ):
        super().__init__(raw_data)
        self.set_ot_scan(ot_scan_path)
        self.set_pr_scan(pr_scan_path)
        self.set_measure_scan(measure_scan_path)

    def get_scan_type(self):
        return 29

    def get_name(self):
        return "SEL"

    def get_body_part_name(self):
        return "LSPINE"

    def get_ref_type(self):
        return "L"

    def get_ref_source(self):
        return "NHANES"

    def set_ot_scan(self, scan_path: Path | None):
        self.ot_scan_path = scan_path

    def set_pr_scan(self, scan_path: Path | None):
        self.pr_scan_path = scan_path

    def set_measure_scan(self, scan_path: Path | None):
        self.measure_scan_path = scan_path

    def is_valid(self):
        if not self.ot_scan_path:
            return False

        if not self.pr_scan_path:
            return False

        if not self.measure_scan_path:
            return False

        if not self.ot_scan_path.exists():
            return False

        if not self.ot_scan_path.is_file():
            return False

        if not self.pr_scan_path.exists():
            return False

        if not self.pr_scan_path.is_file():
            return False

        if not self.measure_scan_path.exists():
            return False

        if not self.measure_scan_path.is_file():
            return False

        try:
            pydicom.dcmread(self.ot_scan_path)
        except Exception as e:
            logger.error(e)
            return False

        try:
            pydicom.dcmread(self.pr_scan_path)
        except Exception as e:
            logger.error(e)
            return False

        try:
            pydicom.dcmread(self.measure_scan_path)
        except Exception as e:
            logger.error(e)
            return False

        return True

    @override
    def to_dict(self):
        res = super().to_dict()
        return dict(sorted(res.items()))

    def get_file_info(self) -> dict | None:
        if self.measure_scan_path is None:
            return None

        try:
            ds = pydicom.dcmread(self.measure_scan_path, stop_before_pixels=True)
            return {
                "PATIENT_ID": ds.get("PatientID"),
                "FILE_PATH": str(self.measure_scan_path.resolve()),
                "MEDIA_STORAGE_UID": ds.file_meta.get("MediaStorageSOPClassUID"),
                "SIDE": ds.get("Laterality"),
                "SIZE": get_file_size(self.measure_scan_path),
                "STUDY_ID": ds.get("StudyInstanceUID"),
                "STUDY_INSTANCE_UID": ds.get("StudyInstanceUID"),
            }
        except Exception as e:
            logger.error(f"get_file_info {e}")
            return None

    def analyze(self, patient_info, patscan_db: PatScanDB, reference_db: ReferenceDB):
        if self.measure_scan_path is None:
            logger.warning("SEL_DICOM_MEASURE missing, skipping analysis")
            return False

        if self.ot_scan_path is None:
            logger.warning("SEL_DICOM_OT missing, skipping analysis")
            return False

        if self.pr_scan_path is None:
            logger.warning("SEL_DICOM_PR missing, skipping analysis")
            return False

        if not self.is_valid():
            logger.error("lateral spine not valid")
            return False

        patient_key: str = patient_info.get("PATIENT_KEY")
        if patient_key is None:
            logger.error("patient key is none")
            return False

        try:
            file_info = self.get_file_info()
            if file_info is None:
                logger.error("failed to get dicom file info")
                return False

            self._update_fields(file_info)

            self._set_field(self.field_map["NAME"], "SEL_DICOM_MEASURE")

            success, result = patscan_db.get_scan_analysis(
                patient_key, self.get_scan_type()
            )
            if not success:
                logger.error(result)
                return False
            scan_analysis = result[0]
            self._update_fields(raw_data=scan_analysis)
            return True

        except Exception as e:
            logger.error(e)
            return False

