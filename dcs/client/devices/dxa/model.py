import logging
import copy

from typing import override

from model import Model

from devices.dxa.session import DXASession
from devices.dxa.config import DXAConfig
from devices.dxa.measurements.hip import LeftHip, RightHip
from devices.dxa.measurements.forearm import LeftForearm, RightForearm
from devices.dxa.measurements.iva_imaging import IVAImagingMeasurement
from devices.dxa.measurements.ap_lumbar_spine import APLumbarSpine
from devices.dxa.measurements.whole_body import WholeBody

logger = logging.getLogger("dxa")


class DXAModel(Model):
    def __init__(self, session: DXASession, config: DXAConfig):
        super().__init__(session, config)
        self.reset()

    def analyze(self, patient_info, patscan_db, reference_db):
        self.left_hip.analyze(patient_info, patscan_db, reference_db)
        self.right_hip.analyze(patient_info, patscan_db, reference_db)

    def update_scans(self, scans: dict) -> tuple[bool, dict]:
        self.reset()

        for scan_type, scan_path in scans.items():
            if scan_type == "L_HIP_DICOM":
                if not self.add_file(scan_path, send_name=scan_type):
                    return False, None
                self.left_hip.set_scan(scan_path)

            elif scan_type == "R_HIP_DICOM":
                if not self.add_file(scan_path, send_name=scan_type):
                    return False, None
                self.right_hip.set_scan(scan_path)

            elif scan_type == "WB_DICOM_1":
                if not self.add_file(scan_path, send_name=scan_type):
                    return False, None
                self.whole_body.set_wb1_scan_path(scan_path)

            elif scan_type == "WB_DICOM_2":
                if not self.add_file(scan_path, send_name=scan_type):
                    return False, None
                self.whole_body.set_wb2_scan_path(scan_path)

            elif scan_type == "FA_L_DICOM":
                if not self.add_file(scan_path, send_name=scan_type):
                    return False, None
                self.left_forearm.set_scan(scan_path)

            elif scan_type == "FA_R_DICOM":
                if not self.add_file(scan_path, send_name=scan_type):
                    return False, None
                self.right_forearm.set_scan(scan_path)

            elif scan_type == "SP_DICOM_1":
                if not self.add_file(scan_path, send_name=scan_type):
                    return False, None
                self.ap_lumbar_spine.set_scan(scan_path)

            elif scan_type == "SEL_DICOM_MEASURE":
                if not self.add_file(scan_path, send_name=scan_type):
                    return False, None
                self.lateral_spine.set_measure_scan(scan_path)

            elif scan_type == "SEL_DICOM_OT":
                if not self.add_file(scan_path, send_name=scan_type):
                    return False, None
                self.lateral_spine.set_ot_scan(scan_path)

            elif scan_type == "SEL_DICOM_PR":
                if not self.add_file(scan_path, send_name=scan_type):
                    return False, None
                self.lateral_spine.set_pr_scan(scan_path)

            elif scan_type == "SR_DICOM":
                if not self.add_file(scan_path, send_name=scan_type):
                    return False, None

            else:
                logger.error("Unknown scan type")
                return False, None

        return True, self._get_scans_count()

    def _get_scans_count(self) -> dict:
        wbody = len(
            [
                x
                for x in self.files
                if x.send_name == "WB_DICOM_1" or x.send_name == "WB_DICOM_2"
            ]
        )
        l_hip = len([x for x in self.files if x.send_name == "L_HIP_DICOM"])
        r_hip = len([x for x in self.files if x.send_name == "R_HIP_DICOM"])

        fa_l = len([x for x in self.files if x.send_name == "FA_L_DICOM"])
        fa_r = len([x for x in self.files if x.send_name == "FA_R_DICOM"])

        spine = len([x for x in self.files if x.send_name == "SP_DICOM_1"])
        lateral_spine = len(
            [
                x
                for x in self.files
                if x.send_name == "SEL_DICOM_MEASURE"
                or x.send_name == "SEL_DICOM_OT"
                or x.send_name == "SEL_DICOM_PR"
            ]
        )

        return {
            "whole_body": wbody,
            "left_hip": l_hip,
            "right_hip": r_hip,
            "left_forearm": fa_l,
            "right_forearm": fa_r,
            "spine": spine,
            "lateral_spine": lateral_spine,
        }

    @override
    def reset(self):
        self.left_hip = LeftHip(raw_data={}, scan_path=None)
        self.right_hip = RightHip(raw_data={}, scan_path=None)
        self.left_forearm = LeftForearm(raw_data={}, scan_path=None)
        self.right_forearm = RightForearm(raw_data={}, scan_path=None)
        self.lateral_spine = IVAImagingMeasurement(
            ot_scan_path=None, pr_scan_path=None, measure_scan_path=None
        )
        self.ap_lumbar_spine = APLumbarSpine(raw_data={}, scan_path=None)
        self.whole_body = WholeBody(raw_data={}, wb1_scan_path=None, wb2_scan_path=None)

    def is_valid(self) -> tuple[bool, str]:
        if self.session.include_lhip and not self.left_hip.is_valid():
            return False, "Missing left hip scan"

        if self.session.include_rhip and not self.right_hip.is_valid():
            return False, "Missing right hip scan"

        if self.session.include_apspine and not self.ap_lumbar_spine.is_valid():
            return False, "Missing AP lumbar spine scan"

        if self.session.include_lfa and not self.left_forearm.is_valid():
            return False, "Missing left forearm scan"

        if self.session.include_rfa and not self.right_forearm.is_valid():
            return False, "Missing right forearm scan"

        if self.session.include_spine and not self.lateral_spine.is_valid():
            return False, "Missing lateral spine scan"

        if self.session.include_wholebody and not self.whole_body.is_valid():
            return False, "Missing whole body scan"

        return True


    @override
    def to_response(self):
        response = {
            "value": {
                "session": self.session.model_dump(mode="json") if self.session else {},
                "manual_entry": self.manual_entry,
                "metadata": copy.deepcopy(self.metadata),
                "results": {},
                "files": {
                    f"{file_info.send_name}_dcm": file_info.readable_size
                    for file_info in self.files
                },
            }
        }

        response["value"]["results"] = {}

        if self.left_hip.is_valid():
            response["value"]["results"]["hip_l"] = self.left_hip.to_dict()
            response["value"]["metadata"]["femoral_neck_bmd"] = self.left_hip.neck_bmd

        if self.right_hip.is_valid():
            response["value"]["results"]["hip_r"] = self.right_hip.to_dict()
            response["value"]["metadata"]["femoral_neck_bmd"] = self.right_hip.neck_bmd

        if self.whole_body.is_valid():
            response["value"]["results"]["whole_body"] = self.whole_body.to_dict()

        if self.ap_lumbar_spine.is_valid():
            response["value"]["results"]["ap_spine"] = self.ap_lumbar_spine.to_dict()

        if self.left_forearm.is_valid():
            response["value"]["results"]["forearm_left"] = self.left_forearm.to_dict()

        if self.right_forearm.is_valid():
            response["value"]["results"]["forearm_right"] = self.right_forearm.to_dict()

        if self.lateral_spine.is_valid():
            response["value"]["results"]["iva"] = {}

        return response
