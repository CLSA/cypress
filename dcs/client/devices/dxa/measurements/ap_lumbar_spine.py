import json
from typing import override
import pydicom

import logging

from measure import Record
from pathlib import Path

from devices.dxa.utils.validation import Side

from devices.dxa.utils.analysis import compute_age_bracket, compute_years_difference
from devices.dxa.apex.reference_db import ReferenceDB
from devices.dxa.apex.patscan_db import PatScanDB

from PySide6.QtCore import QCoreApplication

from utils import get_file_size

logger = logging.getLogger("dxa")

class APLumbarSpine(Record):
    ranges = {
        "l1_bmd": "1...",
        "l2_bmd": ".2..",
        "l3_bmd": "..3.",
        "l4_bmd": "...4",
        "tot_bmd": "1234",
        "tot_l1_bmd": "1...",
        "tot_l2_bmd": ".2..",
        "tot_l3_bmd": "..3.",
        "tot_l4_bmd": "...4",
        "tot_l1l2_bmd": "12..",
        "tot_l1l3_bmd": "1.3.",
        "tot_l1l4_bmd": "1..4",
        "tot_l2l3_bmd": ".23.",
        "tot_l2l4_bmd": ".2.4",
        "tot_l3l4_bmd": "..34",
        "tot_l1l2l3_bmd": "123.",
        "tot_l1l2l4_bmd": "12.4",
        "tot_l1l3l4_bmd": "1.34",
        "tot_l2l3l4_bmd": ".234",
    }

    name = "SP_DICOM_1"
    file_name = "SP_DICOM_1.dcm"

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

        "PHYSICIAN_COMMENT": {
            "attr": "physician_comment",
            "data_type": str,
            "units": None,
        },

        # Derived
        "L1_T": {"attr": "l1_t", "data_type": float, "units": None},
        "L1_Z": {"attr": "l1_z", "data_type": float, "units": None},
        "L2_T": {"attr": "l2_t", "data_type": float, "units": None},
        "L2_Z": {"attr": "l2_z", "data_type": float, "units": None},
        "L3_T": {"attr": "l3_t", "data_type": float, "units": None},
        "L3_Z": {"attr": "l3_z", "data_type": float, "units": None},
        "L4_T": {"attr": "l4_t", "data_type": float, "units": None},
        "L4_Z": {"attr": "l4_z", "data_type": float, "units": None},
        "TOT_T": {"attr": "tot_t", "data_type": float, "units": None},
        "TOT_Z": {"attr": "tot_z", "data_type": float, "units": None},

        # PatScanDB
        "NO_REGIONS": {"attr": "no_regions", "data_type": int, "units": None},
        "STARTING_REGION": {"attr": "starting_region", "data_type": int, "units": None},
        "L1_INCLUDED": {"attr": "l1_included", "data_type": bool, "units": None},
        "L1_AREA": {"attr": "l1_area", "data_type": float, "units": None},
        "L1_BMC": {"attr": "l1_bmc", "data_type": float, "units": None},
        "L1_BMD": {"attr": "l1_bmd", "data_type": float, "units": None},
        "L2_INCLUDED": {"attr": "l2_included", "data_type": bool, "units": None},
        "L2_AREA": {"attr": "l2_area", "data_type": float, "units": None},
        "L2_BMC": {"attr": "l2_bmc", "data_type": float, "units": None},
        "L2_BMD": {"attr": "l2_bmd", "data_type": float, "units": None},
        "L3_INCLUDED": {"attr": "l3_included", "data_type": bool, "units": None},
        "L3_AREA": {"attr": "l3_area", "data_type": float, "units": None},
        "L3_BMC": {"attr": "l3_bmc", "data_type": float, "units": None},
        "L3_BMD": {"attr": "l3_bmd", "data_type": float, "units": None},
        "L4_INCLUDED": {"attr": "l4_included", "data_type": bool, "units": None},
        "L4_AREA": {"attr": "l4_area", "data_type": float, "units": None},
        "L4_BMC": {"attr": "l4_bmc", "data_type": float, "units": None},
        "L4_BMD": {"attr": "l4_bmd", "data_type": float, "units": None},
        "TOT_AREA": {"attr": "tot_area", "data_type": float, "units": None},
        "TOT_BMC": {"attr": "tot_bmc", "data_type": float, "units": None},
        "TOT_BMD": {"attr": "tot_bmd", "data_type": float, "units": None},
        "STD_TOT_BMD": {"attr": "std_tot_bmd", "data_type": float, "units": None},
        "ROI_TYPE": {"attr": "roi_type", "data_type": int, "units": None},
        "ROI_WIDTH": {"attr": "roi_width", "data_type": float, "units": None},
        "ROI_HEIGHT": {"attr": "roi_height", "data_type": float, "units": None},
    }

    def __init__(self, raw_data: dict, scan_path: Path | None = None):
        super().__init__(raw_data)
        self.set_scan(scan_path)

    def set_scan(self, scan_path: Path | None):
        self.scan_path = scan_path

    def get_scan_type(self):
        return 1

    def get_ref_type(self):
        return "S"

    def get_body_part_name(self):
        return "SPINE"

    def get_name(self):
        return "SP"

    def get_send_name(self):
        return "SP_DICOM_1"

    def get_ref_source(self):
        return "Hologic"

    def get_bone_range_key(self, l1: bool, l2: bool, l3: bool, l4: bool) -> str | None:
        if not l1 and not l2 and not l3 and not l4:
            logger.warning("No spine vertebrae included")

        if l1 and not l2 and not l3 and not l4:
            return "tot_l1_bmd"
        elif not l1 and l2 and not l3 and not l4:
            return "tot_l2_bmd"
        elif not l1 and not l2 and l3 and not l4:
            return "tot_l3_bmd"
        elif not l1 and not l2 and not l3 and l4:
            return "tot_l4_bmd"
        elif l1 and l2 and not l3 and not l4:
            return "tot_l1l2_bmd"
        elif l1 and not l2 and l3 and not l4:
            return "tot_l1l3_bmd"
        elif l1 and not l2 and not l3 and l4:
            return "tot_l1l4_bmd"
        elif not l1 and l2 and l3 and not l4:
            return "tot_l2l3_bmd"
        elif not l1 and l2 and not l3 and l4:
            return "tot_l2l4_bmd"
        elif not l1 and not l2 and l3 and l4:
            return "tot_l3l4_bmd"
        elif l1 and l2 and l3 and not l4:
            return "tot_l1l2l3_bmd"
        elif l1 and l2 and not l3 and l4:
            return "tot_l1l2l4_bmd"
        elif l1 and not l2 and l3 and l4:
            return "tot_l1l3l4_bmd"
        elif not l1 and l2 and l3 and l4:
            return "tot_l2l3l4_bmd"
        else:
            return "tot_bmd"

    def get_bmd_data(self):
        ## AP lumbar spine:
        ## - identify the included vertebral levels
        ## - sum the area and sum the bmc of the included vertebral levels
        ## - compute the revised total bmd from summed bmc / summed area
        ## - provide the proper bone range code for total bmd
        bmd_data = {}

        for key, value in self.to_dict().items():
            if key.endswith("_bmd") and key in self.ranges:
                bmd_data[key] = value

        tot_bmd = self.tot_bmd

        tot_bmc = 0.0
        tot_area = 0.0

        if self.l1_included:
            tot_bmc += self.l1_bmc
            tot_area += self.l1_area

        if self.l2_included:
            tot_bmc += self.l2_bmc
            tot_area += self.l2_area

        if self.l3_included:
            tot_bmc += self.l3_bmc
            tot_area += self.l3_area

        if self.l4_included:
            tot_bmc += self.l4_bmc
            tot_area += self.l4_area

        if tot_area != 0.0:
            last_tot_bmd = tot_bmd
            tot_bmd = tot_bmc / tot_area

            logger.info(
                f"updating ap lumbar spine total bmd from {last_tot_bmd} to {tot_bmd}"
            )

        tot_key = self.get_bone_range_key(
            l1=self.l1_included if self.l1_included else False,
            l2=self.l2_included if self.l2_included else False,
            l3=self.l3_included if self.l3_included else False,
            l4=self.l4_included if self.l4_included else False,
        )

        bmd_data[tot_key] = tot_bmd

        return bmd_data

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
            logger.info("ap spine not valid")
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

            success, result = patscan_db.get_scan_data("Spine", patient_key, scan_id)
            if not success:
                logger.error("no spine scan data")
                return False
            spine_data = result

            self._update_fields(spine_data)
            tz_scores = self.compute_tz_scores(
                patient_data=patient_info,
                scan_analysis=scan_analysis,
                reference_db=reference_db,
            )
            self._update_fields(spine_data | tz_scores)
            logger.debug(f"spine: {json.dumps(self.to_dict(), indent=4)}")

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
        logger.debug("ap_spine.compute_tz_scores")

        bmd_data = self.get_bmd_data()
        print(bmd_data)
        logger.debug(json.dumps(bmd_data, indent=4))

        tz_scores = {}
        for bmd_key, bmd_value in bmd_data.items():
            logger.debug(f"calculating t score for {bmd_key} ({bmd_value})..")
            t_score = self._get_t_score(bmd_key, bmd_value, reference_db)
            if t_score is None:
                logger.error(f"failed to calculate t score for ({bmd_key}, {bmd_value})")
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
                continue
            tz_scores[z_score[0]] = z_score[1]

        return tz_scores

    def _get_t_score(
        self, bmd_key, bmd_value, reference_db: ReferenceDB
    ) -> tuple[str, float] | None:

        if bmd_key.startswith("tot_"):
            attr_name = "TOT_T"
        else:
            attr_name = bmd_key.replace("_bmd", "_t").upper()

        t_score = None

        success, result = reference_db.select_reference_curve(
            method="APEX" if "l1_" in bmd_key or "l4_" in bmd_key else "NULL",
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

        if bmd_key.startswith("tot_"):
            attr_name = "TOT_Z"
        else:
            attr_name = bmd_key.replace("_bmd", "_z").upper()

        sex: str = self.get_sex(patient_data)
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

        def interpolate(u: float, min_val: float, max_val: float) -> float:
            return (1.0 - u) * min_val + u * max_val

        age_span = bracket.get("age_span")
        if age_span:
            age_min = bracket["age_min"]
            age_max = bracket["age_max"]

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

            m_value = interpolate(u, min_point.get("Y_VALUE"), max_point.get("Y_VALUE"))
            l_value = interpolate(u, min_point.get("L_VALUE"), max_point.get("L_VALUE"))
            sigma = interpolate(u, min_point.get("STD"), max_point.get("STD"))

            z_score = (
                m_value * (pow(bmd_value / m_value, l_value) - 1.0) / (l_value * sigma)
            )

            logger.debug(
                f"z_score: {z_score} = {m_value} * (pow({bmd_value} / {m_value}, {l_value}) - 1.0) / ({l_value} * {sigma})"
            )

        return (attr_name, z_score)