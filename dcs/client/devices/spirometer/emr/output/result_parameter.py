from typing import override

from devices.spirometer.emr.output.base import SpirometerXML

result_map = {
    "AmbHumidity": "ambient_humidity",
    "AmbPressure": "ambient_pressure",
    "AmbTemp": "ambient_temperature",
    "AmbTemp_Fahr": "ambient_temperature_fahr",
    "BEV": "bev",
    "BTPSex": "btps_ex",
    "BTPSin": "btps_in",
    "EOTV": "eotv",
    "FEF10": "fef_10",
    "FEF25": "fef_25",
    "FEF2575": "fef_25_75",
    "FEF2575_6": "fef_25_75_6",
    "FEF2575_FVC": "fef_25_75_fvc",
    "FEF40": "fef_40",
    "FEF50": "fef_50",
    "FEF50_FVC": "fef_50_fvc",
    "FEF50_VCmax": "fef_50_vcmax",
    "FEF60": "fef_60",
    "FEF75": "fef_75",
    "FEF7585": "fef_75_85",
    "FEF80": "fef_80",
    "FET": "fet",
    "FET2575": "fet_25_75",
    "FEV1": "fev_1",
    "FEV1_FEV6": "fev_1_fev_6",
    "FEV1_FVC": "fev_1_fvc",
    "FEV1_VCext": "fev_1_vcext",
    "FEV1_VCmax": "fev_1_vcmax",
    "FEV3": "fev_3",
    "FEV3_FVC": "fev_3_fvc",
    "FEV3_VCmax": "fev_3_vcmax",
    "FEV6": "fev_6",
    "FEV_25": "fev_25",
    "FEV_5": "fev_5",
    "FEV_5_FVC": "fev_5_fvc",
    "FEV_75": "fev_75",
    "FEV_75_FEV6": "fev_75_fev_6",
    "FEV_75_FVC": "fev_75_fvc",
    "FEV_75_VCmax": "fev_75_vcmax",
    "FVC": "fvc",
    "MEF20": "mef_20",
    "MEF25": "mef_25",
    "MEF40": "mef_40",
    "MEF50": "mef_50",
    "MEF60": "mef_60",
    "MEF75": "mef_75",
    "MEF90": "mef_90",
    "MMEF": "mmef",
    "MTC1": "mtc_1",
    "MTC2": "mtc_2",
    "MTC3": "mtc_3",
    "MTCR": "mtc_r",
    "MsgNo": "msg_no",
    "PEF": "pef",
    "PEFT": "peft",
    "PEF_L_Min": "pef_l_min",
    "T0": "t0",
    "VCext": "vcext",
    "VCmax": "vcmax",

    # new with F4
    "PIF_L_Min": "pif_l_min",
    "FIF25": "fif_25",
    "FIVC": "fiv_c",
    "FIV1": "fiv_1",
    "VC": "vc",
    "FIF50": "fif_50",
    "FIF2575": "fif_25_75",
    "FIF50_FEF50": "fif_50_fef_50",
    "FIV_25": "fiv_25",
    "FEV1_FIV1": "fev_1_fiv_1",
    "ATI": "ati",
    "FIV_5": "fiv_5",
    "E50_I50": "e_50_i_50",
    "MIF25": "mif_25",
    "MIF50": "mif_50",
    "MIF75": "mif_75",
    "FEV1_VC": "fev_1_vc",
    "MMIF": "mmif",
    "PIF": "pif",
    "CVI": "cvi",
    "FEV1_FIVC": "fev_1_fiv_c",
    "FIF75": "fif_75"
}

class ResultParameter(SpirometerXML):
    field_map = {
        "ID": {"key": "id", "type": str},
        "DataValue": {"key": "data_value", "type": float},
        "Unit": {"key": "unit", "type": str},
        "PredictedValue": {"key": "predicted_value", "type": float},
        "LLNormalValue": {"key": "ll_normal_value", "type": float},
        "Z-Score": {"key": "z_score", "type": float},
    }

    @override
    def _parse_xml(self, root):
        id_val = root.attrib.get("ID")

        self.set_attribute("ID", result_map.get(id_val, id_val))

        for child in root:
            if child.tag in self.field_map:
                self.set_attribute(child.tag, child.text)
