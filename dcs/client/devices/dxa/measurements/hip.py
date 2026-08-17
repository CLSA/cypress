import logging
from typing import override

import pydicom

from pathlib import Path

from measure import Record

logger = logging.getLogger("dxa")

class HipMeasurement(Record):
    ranges = {
        "neck_bmd": "1...",
        "troch_bmd": ".2..",
        "inter_bmd": "..3.",
        "wards_bmd": "...4",
        "htot_bmd": "123.",
    }

    field_map = {
        "TROCH_AREA": {"attr": "troch_area", "data_type": float, "units": None},
        "TROCH_BMC": {"attr": "troch_bmc", "data_type": float, "units": None},
        "TROCH_BMD": {"attr": "troch_bmd", "data_type": float, "units": None},
        "INTER_AREA": {"attr": "inter_area", "data_type": float, "units": None},
        "INTER_BMC": {"attr": "inter_bmc", "data_type": float, "units": None},
        "INTER_BMD": {"attr": "inter_bmd", "data_type": float, "units": None},
        "NECK_AREA": {"attr": "neck_area", "data_type": float, "units": None},
        "NECK_BMC": {"attr": "neck_bmc", "data_type": float, "units": None},
        "NECK_BMD": {"attr": "neck_bmd", "data_type": float, "units": None},
        "WARDS_AREA": {"attr": "wards_area", "data_type": float, "units": None},
        "WARDS_BMC": {"attr": "wards_bmc", "data_type": float, "units": None},
        "WARDS_BMD": {"attr": "wards_bmd", "data_type": float, "units": None},
        "HTOT_AREA": {"attr": "htot_area", "data_type": float, "units": None},
        "HTOT_BMC": {"attr": "htot_bmc", "data_type": float, "units": None},
        "HTOT_BMD": {"attr": "htot_bmd", "data_type": float, "units": None},
        "ROI_TYPE": {"attr": "roi_type", "data_type": int, "units": None},
        "ROI_WIDTH": {"attr": "roi_width", "data_type": float, "units": None},
        "ROI_HEIGHT": {"attr": "roi_height", "data_type": float, "units": None},
        "AXIS_LENGTH": {"attr": "axis_length", "data_type": float, "units": None},
        "PHYSICIAN_COMMENT": {
            "attr": "physician_comment",
            "data_type": str,
            "units": None,
        },
        "NN_BMD": {"attr": "nn_bmd", "data_type": float, "units": None},
        "NN_CSA": {"attr": "nn_csa", "data_type": float, "units": None},
        "NN_CSMI": {"attr": "nn_csmi", "data_type": float, "units": None},
        "NN_WIDTH": {"attr": "nn_width", "data_type": float, "units": None},
        "NN_ED": {"attr": "nn_ed", "data_type": float, "units": None},
        "NN_ACT": {"attr": "nn_act", "data_type": float, "units": None},
        "NN_PCD": {"attr": "nn_pcd", "data_type": float, "units": None},
        "NN_CMP": {"attr": "nn_cmp", "data_type": float, "units": None},
        "NN_SECT_MOD": {"attr": "nn_sect_mod", "data_type": float, "units": None},
        "NN_BR": {"attr": "nn_br", "data_type": float, "units": None},
        "IT_BMD": {"attr": "it_bmd", "data_type": float, "units": None},
        "IT_CSA": {"attr": "it_csa", "data_type": float, "units": None},
        "IT_CSMI": {"attr": "it_csmi", "data_type": float, "units": None},
        "IT_WIDTH": {"attr": "it_width", "data_type": float, "units": None},
        "IT_ED": {"attr": "it_ed", "data_type": float, "units": None},
        "IT_ACT": {"attr": "it_act", "data_type": float, "units": None},
        "IT_PCD": {"attr": "it_pcd", "data_type": float, "units": None},
        "IT_CMP": {"attr": "it_cmp", "data_type": float, "units": None},
        "IT_SECT_MOD": {"attr": "it_sect_mod", "data_type": float, "units": None},
        "IT_BR": {"attr": "it_br", "data_type": float, "units": None},
        "FS_BMD": {"attr": "fs_bmd", "data_type": float, "units": None},
        "FS_CSA": {"attr": "fs_csa", "data_type": float, "units": None},
        "FS_CSMI": {"attr": "fs_csmi", "data_type": float, "units": None},
        "FS_WIDTH": {"attr": "fs_width", "data_type": float, "units": None},
        "FS_ED": {"attr": "fs_ed", "data_type": float, "units": None},
        "FS_ACT": {"attr": "fs_act", "data_type": float, "units": None},
        "FS_PCD": {"attr": "fs_pcd", "data_type": float, "units": None},
        "FS_CMP": {"attr": "fs_cmp", "data_type": float, "units": None},
        "FS_SECT_MOD": {"attr": "fs_sect_mod", "data_type": float, "units": None},
        "FS_BR": {"attr": "fs_br", "data_type": float, "units": None},
        "SHAFT_NECK_ANGLE": {
            "attr": "shaft_neck_angle",
            "data_type": float,
            "units": None,
        },
    }

    def __init__(self, raw_data, scan_path: Path = None):
        super().__init__(raw_data)
        self.set_scan(scan_path)

    def set_scan(self, scan_path: Path | None):
        self.scan_path = scan_path



    def is_valid(self):
        if not self.scan_path:
            return False

        if not self.scan_path.exists():
            return False

        if not self.scan_path.is_file():
            return False

        try:
            pydicom.dcmread(self.scan_path)
        except Exception as e:
            print(e)
            return False

        return True

    @staticmethod
    def get_scan_type():
        pass

    @staticmethod
    def get_ref_type():
        return "H"

    @staticmethod
    def get_ref_source():
        return "NHANES"

    @staticmethod
    def get_body_part_name():
        return "HIP"

    def get_bmd_data(self):
        bmd_data = {}
        for key, value in self.to_dict().items():
            if key.endswith("_bmd") and key in self.ranges:
                bmd_data[key] = value
        return bmd_data


class LeftHip(HipMeasurement):
    @staticmethod
    def get_scan_type():
        return 2

    @staticmethod
    def get_name():
        return "L_HIP"

    @staticmethod
    def get_send_name():
        return "L_HIP_DICOM"

    def analyze(self, patient_info, patscan_db, reference_db):
        try:
            if self.scan_path is not None:
                logging.debug(patient_info, patscan_db, reference_db)

                ok, scan_analysis = patscan_db.get_scan_analysis(patient_info["PATIENT_KEY"], LeftHip.get_scan_type())
                scan_analysis = scan_analysis[0]
                logging.debug(scan_analysis)

                ok, hip_data = patscan_db.get_scan_data("Hip", patient_info["PATIENT_KEY"], scan_analysis["SCANID"])
                logging.debug(hip_data)

                ok, hip_hsa_data = patscan_db.get_scan_data("HipHSA", patient_info["PATIENT_KEY"], scan_analysis["SCANID"])
                logging.debug(hip_hsa_data)

                all_hip_data = hip_data | hip_hsa_data
                self._set_fields(all_hip_data)
        except Exception as e:
            logger.error(e)


class RightHip(HipMeasurement):
    @staticmethod
    @override
    def get_scan_type():
        return 3

    @staticmethod
    @override
    def get_name():
        return "R_HIP"

    @staticmethod
    @override
    def get_send_name():
        return "R_HIP_DICOM"

    def analyze(self, patient_info, patscan_db, reference_db):
        try:
            if self.scan_path is not None:
                logging.debug(patient_info, patscan_db, reference_db)

                ok, scan_analysis = patscan_db.get_scan_analysis(patient_info["PATIENT_KEY"], RightHip.get_scan_type())
                scan_analysis = scan_analysis[0]
                logging.debug(scan_analysis)

                ok, hip_data = patscan_db.get_scan_data("Hip", patient_info["PATIENT_KEY"], scan_analysis["SCANID"])
                logging.debug(hip_data)

                ok, hip_hsa_data = patscan_db.get_scan_data("HipHSA", patient_info["PATIENT_KEY"], scan_analysis["SCANID"])
                logging.debug(hip_hsa_data)

                all_hip_data = hip_data | hip_hsa_data
                self._set_fields(all_hip_data)
        except Exception as e:
            logger.error(e)