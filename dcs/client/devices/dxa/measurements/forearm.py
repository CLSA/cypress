import logging
import json

from pathlib import Path
from typing import override

import pydicom
from measure import Record

from devices.dxa.utils.analysis import compute_age_bracket, compute_years_difference
from devices.dxa.apex.reference_db import ReferenceDB
from devices.dxa.apex.patscan_db import PatScanDB
from devices.dxa.utils.validation import Side

from PySide6.QtCore import QCoreApplication

logger = logging.getLogger("dxa")

from utils import get_file_size


class ForearmMeasure(Record):
    ranges = {
        "ru13tot_bmd": "1..",
        "rumidtot_bmd": ".2.",
        "ruudtot_bmd": "..3",
        "rutot_bmd": "123",
        "r_13_bmd": "R..",
        "r_mid_bmd": ".R.",
        "r_ud_bmd": "..R",
        "rtot_bmd": "RRR",
        "u_13_bmd": "U..",
        "u_mid_bmd": ".U.",
        "u_ud_bmd": "..U",
        "utot_bmd": "UUU",
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
        "MEDIA_STORAGE_UID": {
            "attr": "media_storage_uid",
            "data_type": str,
            "units": None,
        },
        # TODO remove STUDY_ID, corrected to study_instance_uid
        "STUDY_ID": {"attr": "study_id", "data_type": str, "units": None},
        "STUDY_INSTANCE_UID": {
            "attr": "study_instance_uid",
            "data_type": str,
            "units": None,
        },
        "SIDE": {"attr": "side", "data_type": str, "units": None},
        "SIZE": {"attr": "size", "data_type": str, "units": None},
        # Derived variables
        "RU13TOT_T": {"attr": "ru13tot_t", "data_type": float, "units": None},
        "RU13TOT_Z": {"attr": "ru13tot_z", "data_type": float, "units": None},
        "RUMIDTOT_T": {"attr": "rumidtot_t", "data_type": float, "units": None},
        "RUMIDTOT_Z": {"attr": "rumidtot_z", "data_type": float, "units": None},
        "RUUDTOT_T": {"attr": "ruudtot_t", "data_type": float, "units": None},
        "RUUDTOT_Z": {"attr": "ruudtot_z", "data_type": float, "units": None},
        "RUTOT_T": {"attr": "rutot_t", "data_type": float, "units": None},
        "RUTOT_Z": {"attr": "rutot_z", "data_type": float, "units": None},
        "R_13_T": {"attr": "r_13_t", "data_type": float, "units": None},
        "R_13_Z": {"attr": "r_13_z", "data_type": float, "units": None},
        "R_MID_T": {"attr": "r_mid_t", "data_type": float, "units": None},
        "R_MID_Z": {"attr": "r_mid_z", "data_type": float, "units": None},
        "R_UD_T": {"attr": "r_ud_t", "data_type": float, "units": None},
        "R_UD_Z": {"attr": "r_ud_z", "data_type": float, "units": None},
        "RTOT_T": {"attr": "rtot_t", "data_type": float, "units": None},
        "RTOT_Z": {"attr": "rtot_z", "data_type": float, "units": None},
        "U_13_T": {"attr": "u_13_t", "data_type": float, "units": None},
        "U_13_Z": {"attr": "u_13_z", "data_type": float, "units": None},
        "U_MID_T": {"attr": "u_mid_t", "data_type": float, "units": None},
        "U_MID_Z": {"attr": "u_mid_z", "data_type": float, "units": None},
        "U_UD_T": {"attr": "u_ud_t", "data_type": float, "units": None},
        "U_UD_Z": {"attr": "u_ud_z", "data_type": float, "units": None},
        "UTOT_T": {"attr": "utot_t", "data_type": float, "units": None},
        "UTOT_Z": {"attr": "utot_z", "data_type": float, "units": None},
        # From PatScan DB
        "PHYSICIAN_COMMENT": {
            "attr": "physician_comment",
            "data_type": str,
            "units": None,
        },
        "R_13_AREA": {"attr": "r_13_area", "data_type": float, "units": None},
        "R_13_BMC": {"attr": "r_13_bmc", "data_type": float, "units": None},
        "R_13_BMD": {"attr": "r_13_bmd", "data_type": float, "units": None},
        "R_MID_AREA": {"attr": "r_mid_area", "data_type": float, "units": None},
        "R_MID_BMC": {"attr": "r_mid_bmc", "data_type": float, "units": None},
        "R_MID_BMD": {"attr": "r_mid_bmd", "data_type": float, "units": None},
        "R_UD_AREA": {"attr": "r_ud_area", "data_type": float, "units": None},
        "R_UD_BMC": {"attr": "r_ud_bmc", "data_type": float, "units": None},
        "R_UD_BMD": {"attr": "r_ud_bmd", "data_type": float, "units": None},
        "U_13_AREA": {"attr": "u_13_area", "data_type": float, "units": None},
        "U_13_BMC": {"attr": "u_13_bmc", "data_type": float, "units": None},
        "U_13_BMD": {"attr": "u_13_bmd", "data_type": float, "units": None},
        "U_MID_AREA": {"attr": "u_mid_area", "data_type": float, "units": None},
        "U_MID_BMC": {"attr": "u_mid_bmc", "data_type": float, "units": None},
        "U_MID_BMD": {"attr": "u_mid_bmd", "data_type": float, "units": None},
        "U_UD_AREA": {"attr": "u_ud_area", "data_type": float, "units": None},
        "U_UD_BMC": {"attr": "u_ud_bmc", "data_type": float, "units": None},
        "U_UD_BMD": {"attr": "u_ud_bmd", "data_type": float, "units": None},
        "RTOT_AREA": {"attr": "rtot_area", "data_type": float, "units": None},
        "RTOT_BMC": {"attr": "rtot_bmc", "data_type": float, "units": None},
        "RTOT_BMD": {"attr": "rtot_bmd", "data_type": float, "units": None},
        "UTOT_AREA": {"attr": "utot_area", "data_type": float, "units": None},
        "UTOT_BMC": {"attr": "utot_bmc", "data_type": float, "units": None},
        "UTOT_BMD": {"attr": "utot_bmd", "data_type": float, "units": None},
        "RU13TOT_AREA": {"attr": "ru13tot_area", "data_type": float, "units": None},
        "RU13TOT_BMC": {"attr": "ru13tot_bmc", "data_type": float, "units": None},
        "RU13TOT_BMD": {"attr": "ru13tot_bmd", "data_type": float, "units": None},
        "RUMIDTOT_AREA": {"attr": "rumidtot_area", "data_type": float, "units": None},
        "RUMIDTOT_BMC": {"attr": "rumidtot_bmc", "data_type": float, "units": None},
        "RUMIDTOT_BMD": {"attr": "rumidtot_bmd", "data_type": float, "units": None},
        "RUUDTOT_AREA": {"attr": "ruudtot_area", "data_type": float, "units": None},
        "RUUDTOT_BMC": {"attr": "ruudtot_bmc", "data_type": float, "units": None},
        "RUUDTOT_BMD": {"attr": "ruudtot_bmd", "data_type": float, "units": None},
        "RUTOT_BMC": {"attr": "rutot_bmc", "data_type": float, "units": None},
        "RUTOT_BMD": {"attr": "rutot_bmd", "data_type": float, "units": None},
        "ROI_TYPE": {"attr": "roi_type", "data_type": float, "units": None},
        "ROI_WIDTH": {"attr": "roi_width", "data_type": float, "units": None},
        "ROI_HEIGHT": {"attr": "roi_height", "data_type": float, "units": None},
        "ARM_LENGTH": {"attr": "arm_length", "data_type": float, "units": None},
    }

    def __init__(self, raw_data: dict, scan_path: Path = None):
        super().__init__(raw_data)
        self.set_scan(scan_path)

    def set_scan(self, scan_path: Path | None):
        self.scan_path = scan_path

    def get_scan_type(self):
        pass

    def get_name(self):
        return "FA"

    def get_send_name(self):
        return "FA_DICOM"

    def get_body_part_name(self):
        return "ARM"

    def get_ref_type(self):
        return "R"

    def get_ref_source(self):
        return "Hologic"

    @override
    def to_dict(self):
        res = super().to_dict()
        return dict(sorted(res.items()))

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

    def analyze(self, patient_info, patscan_db, reference_db) -> bool:
        if self.scan_path is None:
            logger.warning(f"no {self.get_send_name()}, skipping analysis")
            return

        if not self.is_valid():
            logger.error("forearm not valid")
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
            self._update_fields(scan_analysis)

            logger.debug(json.dumps(scan_analysis, indent=4))

            scan_id = scan_analysis.get("SCANID")
            if scan_id is None:
                logger.error("no scan id found")
                return False

            success, result = patscan_db.get_scan_data("Forearm", patient_key, scan_id)
            if not success:
                logger.error(result)
                return False
            forearm_data = result
            logger.debug(f"forearm_data: {json.dumps(forearm_data, indent=4)}")

            self._update_fields(forearm_data)
            tz_scores = self.compute_tz_scores(
                patient_data=patient_info,
                scan_analysis=scan_analysis,
                reference_db=reference_db,
            )
            self._update_fields(forearm_data | tz_scores)

            logger.debug(f"forearm: {json.dumps(self.to_dict(), indent=4)}")

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
            or ethnicity == "H" # H and B check for forearm only
            or ethnicity == "B"
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
        logger.debug("forearm.compute_tz_scores")

        bmd_data = self.get_bmd_data()
        logger.debug(json.dumps(bmd_data, indent=4))

        tz_scores = {}
        for bmd_key, bmd_value in bmd_data.items():
            logger.debug(f"calculating t score for {bmd_key} ({bmd_value})..")
            t_score = self._get_t_score(bmd_key, bmd_value, reference_db)
            if t_score is None:
                logger.error(
                    f"failed to calculate t score for ({bmd_key}, {bmd_value})"
                )
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
                logger.error(
                    f"failed to calculate z score for ({bmd_key}, {bmd_value})"
                )
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


class LeftForearm(ForearmMeasure):
    @override
    def get_scan_type(self):
        return 6

    @override
    def get_name(self):
        return "FA_L"

    @override
    def get_send_name(self):
        return "FA_L_DICOM"


class RightForearm(ForearmMeasure):
    @override
    def get_scan_type(self):
        return 7

    @override
    def get_name(self):
        return "FA_R"

    @override
    def get_send_name(self):
        return "FA_R_DICOM"