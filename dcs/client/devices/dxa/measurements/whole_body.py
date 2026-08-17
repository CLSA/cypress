import logging
import pydicom

from pathlib import Path

from measure import Record
from devices.dxa.utils.validation import Side

logger = logging.getLogger("dxa")

class WholeBody(Record):
    ranges = {"wbtot_bmd": "NULL"}

    field_map = {
        "NAME": {"attr": "name", "data_type": str, "units": None},
        "PATIENT_ID": {"attr": "patient_id", "data_type": str, "units": None},
        "FILEPATH": {"attr": "filepath", "data_type": str, "units": None},
        "STUDY_ID": {"attr": "study_id", "data_type": str, "units": None},
        "MEDIA_STORAGE_UID": {
            "attr": "media_storage_uid",
            "data_type": str,
            "units": None,
        },
        # Wbody
        "WBTOT_AREA": {"attr": "wbtot_area", "data_type": float, "units": None},
        "WBTOT_BMC": {"attr": "wbtot_bmc", "data_type": float, "units": None},
        "WBTOT_BMD": {"attr": "wbtot_bmd", "data_type": float, "units": None},
        "SUBTOT_AREA": {"attr": "subtot_area", "data_type": float, "units": None},
        "SUBTOT_BMC": {"attr": "subtot_bmc", "data_type": float, "units": None},
        "SUBTOT_BMD": {"attr": "subtot_bmd", "data_type": float, "units": None},
        "HEAD_AREA": {"attr": "head_area", "data_type": float, "units": None},
        "HEAD_BMC": {"attr": "head_bmc", "data_type": float, "units": None},
        "HEAD_BMD": {"attr": "head_bmd", "data_type": float, "units": None},
        "LARM_AREA": {"attr": "larm_area", "data_type": float, "units": None},
        "LARM_BMC": {"attr": "larm_bmc", "data_type": float, "units": None},
        "LARM_BMD": {"attr": "larm_bmd", "data_type": float, "units": None},
        "RARM_AREA": {"attr": "rarm_area", "data_type": float, "units": None},
        "RARM_BMC": {"attr": "rarm_bmc", "data_type": float, "units": None},
        "RARM_BMD": {"attr": "rarm_bmd", "data_type": float, "units": None},
        "LRIB_AREA": {"attr": "lrib_area", "data_type": float, "units": None},
        "LRIB_BMC": {"attr": "lrib_bmc", "data_type": float, "units": None},
        "LRIB_BMD": {"attr": "lrib_bmd", "data_type": float, "units": None},
        "RRIB_AREA": {"attr": "rrib_area", "data_type": float, "units": None},
        "RRIB_BMC": {"attr": "rrib_bmc", "data_type": float, "units": None},
        "RRIB_BMD": {"attr": "rrib_bmd", "data_type": float, "units": None},
        "T_S_AREA": {"attr": "t_s_area", "data_type": float, "units": None},
        "T_S_BMC": {"attr": "t_s_bmc", "data_type": float, "units": None},
        "T_S_BMD": {"attr": "t_s_bmd", "data_type": float, "units": None},
        "L_S_AREA": {"attr": "l_s_area", "data_type": float, "units": None},
        "L_S_BMC": {"attr": "l_s_bmc", "data_type": float, "units": None},
        "L_S_BMD": {"attr": "l_s_bmd", "data_type": float, "units": None},
        "PELV_AREA": {"attr": "pelv_area", "data_type": float, "units": None},
        "PELV_BMC": {"attr": "pelv_bmc", "data_type": float, "units": None},
        "PELV_BMD": {"attr": "pelv_bmd", "data_type": float, "units": None},
        "LLEG_AREA": {"attr": "lleg_area", "data_type": float, "units": None},
        "LLEG_BMC": {"attr": "lleg_bmc", "data_type": float, "units": None},
        "LLEG_BMD": {"attr": "lleg_bmd", "data_type": float, "units": None},
        "RLEG_AREA": {"attr": "rleg_area", "data_type": float, "units": None},
        "RLEG_BMC": {"attr": "rleg_bmc", "data_type": float, "units": None},
        "RLEG_BMD": {"attr": "rleg_bmd", "data_type": float, "units": None},
        "PHYSICIAN_COMMENT_WBODY": {
            "attr": "physician_comment_wbody",
            "data_type": str,
            "units": None,
        },
        # WbodyComposition start
        "FAT_STD": {"attr": "fat_std", "data_type": float, "units": None},
        "LEAN_STD": {"attr": "lean_std", "data_type": float, "units": None},
        "BRAIN_FAT": {"attr": "brain_fat", "data_type": float, "units": None},
        "WATER_LBM": {"attr": "water_lbm", "data_type": float, "units": None},
        "HEAD_FAT": {"attr": "head_fat", "data_type": float, "units": None},
        "HEAD_LEAN": {"attr": "head_lean", "data_type": float, "units": None},
        "HEAD_MASS": {"attr": "head_mass", "data_type": float, "units": None},
        "HEAD_PFAT": {"attr": "head_pfat", "data_type": float, "units": None},
        "LARM_FAT": {"attr": "larm_fat", "data_type": float, "units": None},
        "LARM_LEAN": {"attr": "larm_lean", "data_type": float, "units": None},
        "LARM_MASS": {"attr": "larm_mass", "data_type": float, "units": None},
        "LARM_PFAT": {"attr": "larm_pfat", "data_type": float, "units": None},
        "RARM_FAT": {"attr": "rarm_fat", "data_type": float, "units": None},
        "RARM_LEAN": {"attr": "rarm_lean", "data_type": float, "units": None},
        "RARM_MASS": {"attr": "rarm_mass", "data_type": float, "units": None},
        "RARM_PFAT": {"attr": "rarm_pfat", "data_type": float, "units": None},
        "TRUNK_FAT": {"attr": "trunk_fat", "data_type": float, "units": None},
        "TRUNK_LEAN": {"attr": "trunk_lean", "data_type": float, "units": None},
        "TRUNK_MASS": {"attr": "trunk_mass", "data_type": float, "units": None},
        "TRUNK_PFAT": {"attr": "trunk_pfat", "data_type": float, "units": None},
        "L_LEG_FAT": {"attr": "l_leg_fat", "data_type": float, "units": None},
        "L_LEG_LEAN": {"attr": "l_leg_lean", "data_type": float, "units": None},
        "L_LEG_MASS": {"attr": "l_leg_mass", "data_type": float, "units": None},
        "L_LEG_PFAT": {"attr": "l_leg_pfat", "data_type": float, "units": None},
        "R_LEG_FAT": {"attr": "r_leg_fat", "data_type": float, "units": None},
        "R_LEG_LEAN": {"attr": "r_leg_lean", "data_type": float, "units": None},
        "R_LEG_MASS": {"attr": "r_leg_mass", "data_type": float, "units": None},
        "R_LEG_PFAT": {"attr": "r_leg_pfat", "data_type": float, "units": None},
        "SUBTOT_FAT": {"attr": "subtot_fat", "data_type": float, "units": None},
        "SUBTOT_LEAN": {"attr": "subtot_lean", "data_type": float, "units": None},
        "SUBTOT_MASS": {"attr": "subtot_mass", "data_type": float, "units": None},
        "SUBTOT_PFAT": {"attr": "subtot_pfat", "data_type": float, "units": None},
        "WBTOT_FAT": {"attr": "wbtot_fat", "data_type": float, "units": None},
        "WBTOT_LEAN": {"attr": "wbtot_lean", "data_type": float, "units": None},
        "WBTOT_MASS": {"attr": "wbtot_mass", "data_type": float, "units": None},
        "WBTOT_PFAT": {"attr": "wbtot_pfat", "data_type": float, "units": None},
        "PHYSICIAN_COMMENT_WBODY_COMP": {
            "attr": "physician_comment_wbody_comp",
            "data_type": str,
            "units": None,
        },
        # SubRegionBone start
        "PHYSICIAN_COMMENT_SUB_REGION_BONE": {
            "attr": "physician_comment_sub_region_bone",
            "data_type": str,
            "units": None,
        },
        "NO_REGIONS": {"attr": "no_regions", "data_type": int, "units": None},
        "NET_AVG_AREA": {"attr": "net_avg_area", "data_type": float, "units": None},
        "NET_AVG_BMC": {"attr": "net_avg_bmc", "data_type": float, "units": None},
        "NET_AVG_BMD": {"attr": "net_avg_bmd", "data_type": float, "units": None},
        "GLOBAL_AREA": {"attr": "global_area", "data_type": float, "units": None},
        "GLOBAL_BMC": {"attr": "global_bmc", "data_type": float, "units": None},
        "GLOBAL_BMD": {"attr": "global_bmd", "data_type": float, "units": None},
        "REG1_NAME": {"attr": "reg1_name", "data_type": str, "units": None},
        "REG1_AREA": {"attr": "reg1_area", "data_type": float, "units": None},
        "REG1_BMC": {"attr": "reg1_bmc", "data_type": float, "units": None},
        "REG1_BMD": {"attr": "reg1_bmd", "data_type": float, "units": None},
        "REG2_NAME": {"attr": "reg2_name", "data_type": str, "units": None},
        "REG2_AREA": {"attr": "reg2_area", "data_type": float, "units": None},
        "REG2_BMC": {"attr": "reg2_bmc", "data_type": float, "units": None},
        "REG2_BMD": {"attr": "reg2_bmd", "data_type": float, "units": None},
        "REG3_NAME": {"attr": "reg3_name", "data_type": str, "units": None},
        "REG3_AREA": {"attr": "reg3_area", "data_type": float, "units": None},
        "REG3_BMC": {"attr": "reg3_bmc", "data_type": float, "units": None},
        "REG3_BMD": {"attr": "reg3_bmd", "data_type": float, "units": None},
        "REG4_NAME": {"attr": "reg4_name", "data_type": str, "units": None},
        "REG4_AREA": {"attr": "reg4_area", "data_type": float, "units": None},
        "REG4_BMC": {"attr": "reg4_bmc", "data_type": float, "units": None},
        "REG4_BMD": {"attr": "reg4_bmd", "data_type": float, "units": None},
        "REG5_NAME": {"attr": "reg5_name", "data_type": str, "units": None},
        "REG5_AREA": {"attr": "reg5_area", "data_type": float, "units": None},
        "REG5_BMC": {"attr": "reg5_bmc", "data_type": float, "units": None},
        "REG5_BMD": {"attr": "reg5_bmd", "data_type": float, "units": None},
        "REG6_NAME": {"attr": "reg6_name", "data_type": str, "units": None},
        "REG6_AREA": {"attr": "reg6_area", "data_type": float, "units": None},
        "REG6_BMC": {"attr": "reg6_bmc", "data_type": float, "units": None},
        "REG6_BMD": {"attr": "reg6_bmd", "data_type": float, "units": None},
        "REG7_NAME": {"attr": "reg7_name", "data_type": str, "units": None},
        "REG7_AREA": {"attr": "reg7_area", "data_type": float, "units": None},
        "REG7_BMC": {"attr": "reg7_bmc", "data_type": float, "units": None},
        "REG7_BMD": {"attr": "reg7_bmd", "data_type": float, "units": None},
        "REG8_NAME": {"attr": "reg8_name", "data_type": str, "units": None},
        "REG8_AREA": {"attr": "reg8_area", "data_type": float, "units": None},
        "REG8_BMC": {"attr": "reg8_bmc", "data_type": float, "units": None},
        "REG8_BMD": {"attr": "reg8_bmd", "data_type": float, "units": None},
        "REG9_NAME": {"attr": "reg9_name", "data_type": str, "units": None},
        "REG9_AREA": {"attr": "reg9_area", "data_type": float, "units": None},
        "REG9_BMC": {"attr": "reg9_bmc", "data_type": float, "units": None},
        "REG9_BMD": {"attr": "reg9_bmd", "data_type": float, "units": None},
        "REG10_NAME": {"attr": "reg10_name", "data_type": str, "units": None},
        "REG10_AREA": {"attr": "reg10_area", "data_type": float, "units": None},
        "REG10_BMC": {"attr": "reg10_bmc", "data_type": float, "units": None},
        "REG10_BMD": {"attr": "reg10_bmd", "data_type": float, "units": None},
        "REG11_NAME": {"attr": "reg11_name", "data_type": str, "units": None},
        "REG11_AREA": {"attr": "reg11_area", "data_type": float, "units": None},
        "REG11_BMC": {"attr": "reg11_bmc", "data_type": float, "units": None},
        "REG11_BMD": {"attr": "reg11_bmd", "data_type": float, "units": None},
        "REG12_NAME": {"attr": "reg12_name", "data_type": str, "units": None},
        "REG12_AREA": {"attr": "reg12_area", "data_type": float, "units": None},
        "REG12_BMC": {"attr": "reg12_bmc", "data_type": float, "units": None},
        "REG12_BMD": {"attr": "reg12_bmd", "data_type": float, "units": None},
        "REG13_NAME": {"attr": "reg13_name", "data_type": str, "units": None},
        "REG13_AREA": {"attr": "reg13_area", "data_type": float, "units": None},
        "REG13_BMC": {"attr": "reg13_bmc", "data_type": float, "units": None},
        "REG13_BMD": {"attr": "reg13_bmd", "data_type": float, "units": None},
        "REG14_NAME": {"attr": "reg14_name", "data_type": str, "units": None},
        "REG14_AREA": {"attr": "reg14_area", "data_type": float, "units": None},
        "REG14_BMC": {"attr": "reg14_bmc", "data_type": float, "units": None},
        "REG14_BMD": {"attr": "reg14_bmd", "data_type": float, "units": None},
        # SubRegionComposition
        "NET_AVG_FAT": {"attr": "net_avg_fat", "data_type": float, "units": None},
        "NET_AVG_LEAN": {"attr": "net_avg_lean", "data_type": float, "units": None},
        "NET_AVG_MASS": {"attr": "net_avg_mass", "data_type": float, "units": None},
        "NET_AVG_PFAT": {"attr": "net_avg_pfat", "data_type": float, "units": None},
        "GLOBAL_FAT": {"attr": "global_fat", "data_type": float, "units": None},
        "GLOBAL_LEAN": {"attr": "global_lean", "data_type": float, "units": None},
        "GLOBAL_MASS": {"attr": "global_mass", "data_type": float, "units": None},
        "GLOBAL_PFAT": {"attr": "global_pfat", "data_type": float, "units": None},
        "NO_REGIONS": {"attr": "no_regions", "data_type": int, "units": None},
        "TISSUE_ANALYSIS_METHOD": {
            "attr": "tissue_analysis_method",
            "data_type": int,
            "units": None,
        },
        "PHYSICIAN_COMMENT_SUB_REGION_COMP": {
            "attr": "physician_comment_sub_region_comp",
            "data_type": str,
            "units": None,
        },
        "REG1_NAME": {"attr": "reg1_name", "data_type": str, "units": None},
        "REG1_FAT": {"attr": "reg1_fat", "data_type": float, "units": None},
        "REG1_LEAN": {"attr": "reg1_lean", "data_type": float, "units": None},
        "REG1_MASS": {"attr": "reg1_mass", "data_type": float, "units": None},
        "REG1_PFAT": {"attr": "reg1_pfat", "data_type": float, "units": None},
        "REG2_NAME": {"attr": "reg2_name", "data_type": str, "units": None},
        "REG2_FAT": {"attr": "reg2_fat", "data_type": float, "units": None},
        "REG2_LEAN": {"attr": "reg2_lean", "data_type": float, "units": None},
        "REG2_MASS": {"attr": "reg2_mass", "data_type": float, "units": None},
        "REG2_PFAT": {"attr": "reg2_pfat", "data_type": float, "units": None},
        "REG3_NAME": {"attr": "reg3_name", "data_type": str, "units": None},
        "REG3_FAT": {"attr": "reg3_fat", "data_type": float, "units": None},
        "REG3_LEAN": {"attr": "reg3_lean", "data_type": float, "units": None},
        "REG3_MASS": {"attr": "reg3_mass", "data_type": float, "units": None},
        "REG3_PFAT": {"attr": "reg3_pfat", "data_type": float, "units": None},
        "REG4_NAME": {"attr": "reg4_name", "data_type": str, "units": None},
        "REG4_FAT": {"attr": "reg4_fat", "data_type": float, "units": None},
        "REG4_LEAN": {"attr": "reg4_lean", "data_type": float, "units": None},
        "REG4_MASS": {"attr": "reg4_mass", "data_type": float, "units": None},
        "REG4_PFAT": {"attr": "reg4_pfat", "data_type": float, "units": None},
        "REG5_NAME": {"attr": "reg5_name", "data_type": str, "units": None},
        "REG5_FAT": {"attr": "reg5_fat", "data_type": float, "units": None},
        "REG5_LEAN": {"attr": "reg5_lean", "data_type": float, "units": None},
        "REG5_MASS": {"attr": "reg5_mass", "data_type": float, "units": None},
        "REG5_PFAT": {"attr": "reg5_pfat", "data_type": float, "units": None},
        "REG6_NAME": {"attr": "reg6_name", "data_type": str, "units": None},
        "REG6_FAT": {"attr": "reg6_fat", "data_type": float, "units": None},
        "REG6_LEAN": {"attr": "reg6_lean", "data_type": float, "units": None},
        "REG6_MASS": {"attr": "reg6_mass", "data_type": float, "units": None},
        "REG6_PFAT": {"attr": "reg6_pfat", "data_type": float, "units": None},
        "REG7_NAME": {"attr": "reg7_name", "data_type": str, "units": None},
        "REG7_FAT": {"attr": "reg7_fat", "data_type": float, "units": None},
        "REG7_LEAN": {"attr": "reg7_lean", "data_type": float, "units": None},
        "REG7_MASS": {"attr": "reg7_mass", "data_type": float, "units": None},
        "REG7_PFAT": {"attr": "reg7_pfat", "data_type": float, "units": None},
        "REG8_NAME": {"attr": "reg8_name", "data_type": str, "units": None},
        "REG8_FAT": {"attr": "reg8_fat", "data_type": float, "units": None},
        "REG8_LEAN": {"attr": "reg8_lean", "data_type": float, "units": None},
        "REG8_MASS": {"attr": "reg8_mass", "data_type": float, "units": None},
        "REG8_PFAT": {"attr": "reg8_pfat", "data_type": float, "units": None},
        "REG9_NAME": {"attr": "reg9_name", "data_type": str, "units": None},
        "REG9_FAT": {"attr": "reg9_fat", "data_type": float, "units": None},
        "REG9_LEAN": {"attr": "reg9_lean", "data_type": float, "units": None},
        "REG9_MASS": {"attr": "reg9_mass", "data_type": float, "units": None},
        "REG9_PFAT": {"attr": "reg9_pfat", "data_type": float, "units": None},
        "REG10_NAME": {"attr": "reg10_name", "data_type": str, "units": None},
        "REG10_FAT": {"attr": "reg10_fat", "data_type": float, "units": None},
        "REG10_LEAN": {"attr": "reg10_lean", "data_type": float, "units": None},
        "REG10_MASS": {"attr": "reg10_mass", "data_type": float, "units": None},
        "REG10_PFAT": {"attr": "reg10_pfat", "data_type": float, "units": None},
        "REG11_NAME": {"attr": "reg11_name", "data_type": str, "units": None},
        "REG11_FAT": {"attr": "reg11_fat", "data_type": float, "units": None},
        "REG11_LEAN": {"attr": "reg11_lean", "data_type": float, "units": None},
        "REG11_MASS": {"attr": "reg11_mass", "data_type": float, "units": None},
        "REG11_PFAT": {"attr": "reg11_pfat", "data_type": float, "units": None},
        "REG12_NAME": {"attr": "reg12_name", "data_type": str, "units": None},
        "REG12_FAT": {"attr": "reg12_fat", "data_type": float, "units": None},
        "REG12_LEAN": {"attr": "reg12_lean", "data_type": float, "units": None},
        "REG12_MASS": {"attr": "reg12_mass", "data_type": float, "units": None},
        "REG12_PFAT": {"attr": "reg12_pfat", "data_type": float, "units": None},
        "REG13_NAME": {"attr": "reg13_name", "data_type": str, "units": None},
        "REG13_FAT": {"attr": "reg13_fat", "data_type": float, "units": None},
        "REG13_LEAN": {"attr": "reg13_lean", "data_type": float, "units": None},
        "REG13_MASS": {"attr": "reg13_mass", "data_type": float, "units": None},
        "REG13_PFAT": {"attr": "reg13_pfat", "data_type": float, "units": None},
        "REG14_NAME": {"attr": "reg14_name", "data_type": str, "units": None},
        "REG14_FAT": {"attr": "reg14_fat", "data_type": float, "units": None},
        "REG14_LEAN": {"attr": "reg14_lean", "data_type": float, "units": None},
        "REG14_MASS": {"attr": "reg14_mass", "data_type": float, "units": None},
        "REG14_PFAT": {"attr": "reg14_pfat", "data_type": float, "units": None},
        # ObesityIndices
        "FAT_STD": {"attr": "fat_std", "data_type": float, "units": None},
        "LEAN_STD": {"attr": "lean_std", "data_type": float, "units": None},
        "BRAIN_FAT": {"attr": "brain_fat", "data_type": float, "units": None},
        "WATER_LBM": {"attr": "water_lbm", "data_type": float, "units": None},
        "TOTAL_PERCENT_FAT": {
            "attr": "total_percent_fat",
            "data_type": float,
            "units": None,
        },
        "BODY_MASS_INDEX": {
            "attr": "body_mass_index",
            "data_type": float,
            "units": None,
        },
        "ANDROID_GYNOID_RATIO": {
            "attr": "android_gynoid_ratio",
            "data_type": float,
            "units": None,
        },
        "ANDROID_PERCENT_FAT": {
            "attr": "android_percent_fat",
            "data_type": float,
            "units": None,
        },
        "GYNOID_PERCENT_FAT": {
            "attr": "gynoid_percent_fat",
            "data_type": float,
            "units": None,
        },
        "FAT_MASS": {"attr": "fat_mass", "data_type": float, "units": None},
        "TRUNK_LIMB_FAT_MASS_RATIO": {
            "attr": "trunk_limb_fat_mass_ratio",
            "data_type": float,
            "units": None,
        },
        "FAT_MASS_HEIGHT_SQUARED": {
            "attr": "fat_mass_height_squared",
            "data_type": float,
            "units": None,
        },
        "TOTAL_FAT_MASS": {"attr": "total_fat_mass", "data_type": float, "units": None},
        "LEAN_MASS_HEIGHT_SQUARED": {
            "attr": "lean_mass_height_squared",
            "data_type": float,
            "units": None,
        },
        "APPENDAGE_LEAN_MASS_HEIGHT_2": {
            "attr": "appendage_lean_mass_height_2",
            "data_type": float,
            "units": None,
        },
        "TOTAL_LEAN_MASS": {
            "attr": "total_lean_mass",
            "data_type": float,
            "units": None,
        },
        "PHYSICIAN_COMMENT_OBESITY_INDICES": {
            "attr": "physician_comment_obesity_indices",
            "data_type": str,
            "units": None,
        },
        # AndroidGynoidComposition
        "ANDROID_FAT": {"attr": "android_fat", "data_type": float, "units": None},
        "ANDROID_LEAN": {"attr": "android_lean", "data_type": float, "units": None},
        "GYNOID_FAT": {"attr": "gynoid_fat", "data_type": float, "units": None},
        "GYNOID_LEAN": {"attr": "gynoid_lean", "data_type": float, "units": None},
        "PHYSICIAN_COMMENT_ANDROID_GYNOID_COMP": {
            "attr": "physician_comment_android_gynoid_comp",
            "data_type": str,
            "units": None,
        },
    }

    def __init__(
        self,
        raw_data: dict,
        wb1_scan_path: Path | None = None,
        wb2_scan_path: Path | None = None,
    ):
        super().__init__(raw_data)

        self.set_wb1_scan_path(wb1_scan_path)
        self.set_wb2_scan_path(wb2_scan_path)

    def set_wb1_scan_path(self, wb1_scan_path: Path | None):
        self.wb1_scan_path = wb1_scan_path

    def set_wb2_scan_path(self, wb2_scan_path: Path | None):
        self.wb2_scan_path = wb2_scan_path

    def is_valid(self):
        if not self.wb1_scan_path:
            return False

        if not self.wb2_scan_path:
            return False

        if not self.wb1_scan_path.exists():
            return False

        if not self.wb1_scan_path.is_file():
            return False

        if not self.wb2_scan_path.exists():
            return False

        if not self.wb2_scan_path.is_file():
            return False

        try:
            pydicom.dcmread(self.wb1_scan_path)
        except Exception as e:
            logger.error(e)
            return False

        try:
            pydicom.dcmread(self.wb2_scan_path)
        except Exception as e:
            logger.error(e)
            return False

        return True


    @staticmethod
    def get_side():
        return Side.BOTH

    @staticmethod
    def get_scan_type():
        return 5

    @staticmethod
    def get_name():
        return "WB"

    @staticmethod
    def get_body_part_name():
        return "WBODY"

    @staticmethod
    def get_ref_type():
        return "W"

    @staticmethod
    def get_ref_source():
        return "NHANES"

    def get_bmd_data(self):
        bmd_data = {}
        for key, value in self.to_dict().items():
            print(key, value)
            if key.endswith("_bmd") and key in self.ranges:
                bmd_data[key] = value
        return bmd_data
