import pydicom

from pathlib import Path

from measure import Record
from devices.dxa.utils.validation import Side


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

    @staticmethod
    def get_body_part_name():
        return "ARM"

    @staticmethod
    def get_ref_type():
        return "R"

    @staticmethod
    def get_ref_source():
        return "Hologic"

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

    def get_bmd_data(self):
        bmd_data = {}
        for key, value in self.to_dict().items():
            if key.endswith("_bmd") and key in self.ranges:
                bmd_data[key] = value
        return bmd_data


class LeftForearm(ForearmMeasure):
    @staticmethod
    def get_side():
        return Side.LEFT

    @staticmethod
    def get_scan_type():
        return 6

    @staticmethod
    def get_name():
        return "L_FA"


class RightForearm(ForearmMeasure):
    @staticmethod
    def get_side():
        return Side.RIGHT

    @staticmethod
    def get_scan_type():
        return 7

    @staticmethod
    def get_name():
        return "R_FA"