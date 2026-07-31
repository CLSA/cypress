import logging

from measure import Record

from devices.dxa.utils.validation import Side

logger = logging.getLogger("DXA")


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
        # dicomFile.name
        "NAME": {"attr": "name", "data_type": str, "units": None},
        # dicomFile.patientId
        "PATIENT_ID": {
            "attr": "patient_id",
            "data_type": str,
            "units": None,
        },
        # dicomFile.absFilePath
        "FILEPATH": {
            "attr": "filepath",
            "data_type": str,
            "units": None,
        },
        # dicomFile.studyId
        "STUDY_ID": {
            "attr": "study_id",
            "data_type": str,
            "units": None,
        },
        # dicomFile.mediaStorageUID
        "MEDIA_STORAGE_UID": {
            "attr": "media_storage_uid",
            "data_type": str,
            "units": None,
        },
        # dicomFile.size
        "SIZE": {"attr": "size", "data_type": None, "units": None},
        "PHYSICIAN_COMMENT": {
            "attr": "physician_comment",
            "data_type": str,
            "units": None,
        },
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

    @staticmethod
    def get_side():
        return Side.BOTH

    @staticmethod
    def get_scan_type():
        return 1

    @staticmethod
    def get_name():
        return "SP"

    @staticmethod
    def get_body_part_name():
        return "SPINE"

    @staticmethod
    def get_ref_type():
        return "S"

    @staticmethod
    def get_ref_source():
        return "Hologic"

    @staticmethod
    def get_bone_range_key(l1: bool, l2: bool, l3: bool, l4: bool) -> str | None:
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
            tot_bmc += self.l3_bmc
            tot_area += self.l3_area

        if tot_area != 0.0:
            last_tot_bmd = tot_bmd
            tot_bmd = tot_bmc / tot_area

            logger.debug(
                f"updating ap lumbar spine total bmd from {last_tot_bmd} to {tot_bmd}"
            )

        tot_key = self.get_bone_range_key(
            l1=self.l1_included if self.l1_included else False,
            l2=self.l2_included if self.l2_included else False,
            l3=self.l3_included if self.l3_included else False,
            l4=self.l4_included if self.l4_included else False,
        )

        return {tot_key: tot_bmd}
