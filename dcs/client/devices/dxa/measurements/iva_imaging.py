import pydicom
import logging

from pathlib import Path

from devices.dxa.utils.validation import Side

logger = logging.getLogger("dxa")


class IVAImagingMeasurement:
    def __init__(
        self,
        ot_scan_path: Path | None = None,
        pr_scan_path: Path | None = None,
        measure_scan_path: Path | None = None,
    ):
        self.set_ot_scan(ot_scan_path)
        self.set_pr_scan(pr_scan_path)
        self.set_measure_scan(measure_scan_path)

    @staticmethod
    def get_side():
        return Side.BOTH

    @staticmethod
    def get_scan_type():
        return 29

    @staticmethod
    def get_name():
        return "DEL"

    @staticmethod
    def get_body_part_name():
        return "LSPINE"

    @staticmethod
    def get_ref_type():
        return "L"

    @staticmethod
    def get_ref_source():
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
