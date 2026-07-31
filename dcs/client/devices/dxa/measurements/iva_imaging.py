from devices.dxa.utils.validation import Side


class IVATest:
    def __init__(self):
        pass


class IVAImagingMeasurement:
    def __init__(self):
        pass

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