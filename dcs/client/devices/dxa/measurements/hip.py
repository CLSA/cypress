import json
import logging

from typing import override
from pathlib import Path

import pydicom
from measure import Record

from devices.dxa.utils.analysis import compute_age_bracket, compute_years_difference
from devices.dxa.apex.reference_db import ReferenceDB
from devices.dxa.apex.patscan_db import PatScanDB

from PySide6.QtCore import QCoreApplication

from utils import get_file_size

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
        "SIDE": {"attr": "side", "data_type": str, "units": None},
        "SIZE": {"attr": "size", "data_type": str, "units": None},

        # Derived using ranges
        "NECK_T": {"attr": "neck_t", "data_type": float, "units": None},
        "TROCH_T": {"attr": "troch_t", "data_type": float, "units": None},
        "INTER_T": {"attr": "inter_t", "data_type": float, "units": None},
        "WARDS_T": {"attr": "wards_t", "data_type": float, "units": None},
        "HTOT_T": {"attr": "htot_t", "data_type": float, "units": None},
        "NECK_Z": {"attr": "neck_z", "data_type": float, "units": None},
        "TROCH_Z": {"attr": "troch_z", "data_type": float, "units": None},
        "INTER_Z": {"attr": "inter_z", "data_type": float, "units": None},
        "WARDS_Z": {"attr": "wards_z", "data_type": float, "units": None},
        "HTOT_Z": {"attr": "htot_z", "data_type": float, "units": None},
        # From PatScanDB
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
        if self.scan_path is None:
            return False

        if not self.scan_path.exists():
            return False

        if not self.scan_path.is_file():
            return False

        try:
            pydicom.dcmread(self.scan_path)
        except Exception as e:
            logger.error(e)
            return False

        return True

    def get_scan_type(self):
        pass

    def get_ref_type(self):
        return "H"

    def get_ref_source(self):
        return "NHANES"

    def get_body_part_name(self):
        return "HIP"

    def get_name(self):
        return "HIP"

    def get_send_name(self):
        return "HIP_DICOM"

    def get_side(self):
        return ""

    def get_bmd_data(self):
        bmd_data = {}
        for key, value in self.to_dict().items():
            if key.endswith("_bmd") and key in self.ranges:
                bmd_data[key] = value
        return bmd_data

    def get_file_info(self) -> dict | None:
        if self.scan_path is None:
            return None

        try:
            ds = pydicom.dcmread(self.scan_path, stop_before_pixels=True)
            return {
                "PATIENT_ID": ds.get("PatientID"),
                "FILE_PATH": str(self.scan_path.resolve()),
                "MEDIA_STORAGE_UID": ds.file_meta.get("MediaStorageSOPClassUID"),
                "SIDE": ds.get("Laterality"),
                "SIZE": get_file_size(self.scan_path),
                "STUDY_ID": ds.get("StudyInstanceUID"),
                "STUDY_INSTANCE_UID": ds.get("StudyInstanceUID"),
            }
        except Exception as e:
            logger.error(f"get_file_info {e}")
            return None

    @override
    def to_dict(self):
        res = super().to_dict()
        return dict(sorted(res.items()))

    def analyze(self, patient_info, patscan_db, reference_db) -> bool:
        if self.scan_path is None:
            logger.warning(f"no {self.get_send_name()}, skipping analysis")
            return

        if not self.is_valid():
            logger.error("hip not valid")
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

            self._set_field(self.field_map["NAME"], self.get_send_name())

            success, result = patscan_db.get_scan_analysis(
                patient_key, self.get_scan_type()
            )
            if not success:
                logger.error(result)
                return False
            scan_analysis = result[0]
            self._update_fields(raw_data=scan_analysis)

            logger.debug(json.dumps(scan_analysis, indent=4))

            scan_id = scan_analysis.get("SCANID")
            if scan_id is None:
                logger.error("no scan id found")
                return False

            success, result = patscan_db.get_scan_data("Hip", patient_key, scan_id)
            if not success:
                logger.error(result)
                return False
            hip_data = result
            logger.debug(f"hip: {json.dumps(hip_data, indent=4)}")

            success, result = patscan_db.get_scan_data("HipHSA", patient_key, scan_id)
            if not success:
                logger.error("no hip_hsa_data")
                return False
            hip_hsa_data = result
            logger.debug(f"hip_hsa: {json.dumps(hip_hsa_data, indent=4)}")

            all_hip_data = hip_data | hip_hsa_data
            logger.debug(f"hip:all_hip_data: {json.dumps(all_hip_data, indent=4)}")

            self._update_fields(all_hip_data)

            tz_scores = self.compute_tz_scores(
                patient_data=patient_info,
                scan_analysis=scan_analysis,
                reference_db=reference_db,
            )

            self._update_fields(all_hip_data | tz_scores)

            logger.debug(f"hip: {json.dumps(self.to_dict(), indent=4)}")

            return True

        except Exception as e:
            logger.error(e)
            return False

    def get_ethnicity(self, patient_data: dict) -> str | None:
        ethnicity = patient_data.get("ETHNICITY")
        if (
            not ethnicity
            or ethnicity == "W"
            or ethnicity == "O"
            or ethnicity == "P"
            or ethnicity == "I"
        ):
            ethnicity = None
        else:
            ethnicity = ethnicity.upper()
        return ethnicity

    def get_sex(self, patient_data: dict) -> str:
        sex = patient_data.get("SEX")
        if not sex:
            logger.warning("sex not entered")
            sex = "F"
        sex = sex[0].upper()
        return sex

    def compute_tz_scores(
        self, patient_data, scan_analysis, reference_db: ReferenceDB
    ) -> bool:
        logger.debug("hip.compute_tz_scores")

        bmd_data = self.get_bmd_data()
        logger.debug(json.dumps(bmd_data, indent=4))

        tz_scores = {}
        for bmd_key, bmd_value in bmd_data.items():
            logger.debug(f"calculating t score for {bmd_key} ({bmd_value})..")
            t_score = self._get_t_score(bmd_key, bmd_value, reference_db)
            if t_score is None:
                logger.error(f"failed to calculate t score for ({bmd_key}, {bmd_value})")
                continue
            tz_scores[t_score[0]] = t_score[1]

            logger.debug(f"calculating z score for {bmd_key} ({bmd_value})..")
            z_score = self._get_z_score(
                bmd_key=bmd_key,
                bmd_value=bmd_value,
                patient_data=patient_data,
                scan_analysis=scan_analysis,
                reference_db=reference_db,
            )
            if z_score is None:
                logger.error(f"failed to calculate z score for ({bmd_key}, {bmd_value})")
            tz_scores[z_score[0]] = z_score[1]

        return tz_scores

    def _get_t_score(
        self, bmd_key, bmd_value, reference_db: ReferenceDB
    ) -> tuple[str, float] | None:

        attr_name = bmd_key.replace("_bmd", "_t").upper()
        t_score = None

        success, result = reference_db.select_reference_curve(
            method="NULL",
            sex="F",
            ethnicity=None,
            ref_type=self.get_ref_type(),
            ref_source=self.get_ref_source(),
            bone_range=self.ranges.get(bmd_key),
        )

        if not success:
            logger.error(f"couldn't select valid reference curve: {bmd_key}")
            return (attr_name, None)

        curve_id: str = result.get("UNIQUE_ID")
        age_young: float = result.get("AGE_YOUNG")

        success, result = reference_db.select_point_from_curve(curve_id, age_young)
        if not success:
            logger.error(result)
            return (attr_name, None)

        m_value = result.get("Y_VALUE")
        l_value = result.get("L_VALUE")
        sigma = result.get("STD")

        t_score = (
            m_value * (pow(bmd_value / m_value, l_value) - 1.0) / (l_value * sigma)
        )

        logger.debug(
            f"{attr_name}: {t_score} = {m_value} * (pow({bmd_value} / {m_value}, {l_value}) - 1.0) / ({l_value} * {sigma})"
        )

        return (attr_name, t_score)

    def _get_z_score(
        self,
        bmd_key: str,
        bmd_value: float,
        patient_data: dict,
        scan_analysis: dict,
        reference_db: ReferenceDB,
    ) -> tuple[str, float] | None:
        z_score = None

        attr_name = bmd_key.replace("_bmd", "_z").upper()

        sex: str = self.get_sex(patient_data)
        if sex == "M":
            if bmd_key == "U_UD_BMD":
                return (attr_name, 0.0)

        ethnicity: str = self.get_ethnicity(patient_data)

        scan_date = scan_analysis.get("SCAN_DATE")
        if not scan_date:
            logger.error("scan date is invalid")
            return (attr_name, None)

        birthdate = patient_data.get("BIRTHDATE")
        if not birthdate:
            logger.error("birthdate is invalid")
            return (attr_name, None)

        age = compute_years_difference(
            first=scan_date,
            second=birthdate,
        )

        logger.debug(f"scan_date: {scan_date} birthdate: {birthdate} age: {age}")

        if age == 0.0:
            logger.error("age is 0.0")
            return (attr_name, None)

        success, result = reference_db.select_reference_curve(
            method="NULL",
            sex=sex,
            ethnicity=ethnicity,
            ref_type=self.get_ref_type(),
            ref_source=self.get_ref_source(),
            bone_range=self.ranges.get(bmd_key),
        )

        if not success:
            return (attr_name, None)

        curve = result
        curve_id = curve.get("UNIQUE_ID")
        if curve_id is None:
            return (attr_name, None)

        success, result = reference_db.select_x_values_from_curve(curve_id)
        if not success:
            return (attr_name, None)
        age_table = result

        bracket = compute_age_bracket(age=age, age_table=age_table)
        logger.debug(f"age bracket: {age} {json.dumps(bracket, indent=4)}")

        age_span = bracket.get("age_span")
        if age_span:
            age_min = bracket["age_min"]
            age_max = bracket["age_max"]

            min_point = None
            max_point = None

            success, result = reference_db.select_point_from_curve(
                curve_id, age=age_min
            )
            if not success:
                return (attr_name, None)
            min_point = result

            success, result = reference_db.select_point_from_curve(
                curve_id, age=age_max
            )
            if not success:
                return (attr_name, None)
            max_point = result

            u = (age - age_min) / age_span
            logger.debug(f"u: {u} = ({age} - {age_min}) / {age_span}")

            m_value = ((1.0 - u) * min_point.get("Y_VALUE")) + (
                u * max_point.get("Y_VALUE")
            )
            l_value = ((1.0 - u) * min_point.get("L_VALUE")) + (
                u * max_point.get("L_VALUE")
            )
            sigma = ((1.0 - u) * min_point.get("STD")) + (u * max_point.get("STD"))

            z_score = (
                m_value * (pow(bmd_value / m_value, l_value) - 1.0) / (l_value * sigma)
            )
            logger.debug(
                f"z_score: {z_score} = {m_value} * (pow({bmd_value} / {m_value}, {l_value}) - 1.0) / ({l_value} * {sigma})"
            )

        return (attr_name, z_score)


class LeftHip(HipMeasurement):
    @override
    def get_scan_type(self):
        return 2

    @override
    def get_side(self):
        return "L"

    @override
    def get_name(self):
        return "L_HIP"

    @override
    def get_send_name(self):
        return "L_HIP_DICOM"


class RightHip(HipMeasurement):
    @override
    def get_scan_type(self):
        return 3

    @override
    def get_side(self):
        return "R"

    @override
    def get_name(self):
        return "R_HIP"

    @override
    def get_send_name(self):
        return "R_HIP_DICOM"