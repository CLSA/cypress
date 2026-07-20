import json
import logging

from model import Model

from PySide6.QtCore import QDateTime

from devices.tonometer.settings import DEVICE_NAME
from devices.tonometer.session import TonometerSession
from devices.tonometer.config import TonometerConfig
from devices.tonometer.database import TonometerDatabase

logger = logging.getLogger(DEVICE_NAME)


class EyeMeasure:

    field_map = {
        "measureid": {"attr": "measure_id", "type": int, "units": None},
        "measurenumber": {"attr": "measure_number", "type": int, "units": None},
        "measuredate": {"attr": "measure_date", "type": QDateTime, "units": None},
        "sessiondate": {"attr": "session_date", "type": QDateTime, "units": None},
        "patientid": {"attr": "patient_id", "type": str, "units": None},
        "eye": {"attr": "eye", "type": str, "units": None},
        "oraserialnumber": {"attr": "ora_serial_number", "type": str, "units": None},
        "orasoftware": {"attr": "ora_software", "type": str, "units": None},
        "pcsoftware": {"attr": "pc_software", "type": str, "units": None},
        "iopg": {"attr": "iopg", "type": float, "units": "mmHg"},
        "iopcc": {"attr": "iopcc", "type": float, "units": "mmHg"},
        "crf": {"attr": "crf", "type": float, "units": "mmHg"},
        "cctavg": {"attr": "cct_avg", "type": float, "units": "um"},
        "cctlowest": {"attr": "cct_lowest", "type": float, "units": "um"},
        "cctsd": {"attr": "cct_sd", "type": float, "units": "um"},
        "ch": {"attr": "ch", "type": float, "units": None},
        "tearfilmvalue": {"attr": "tear_film_value", "type": float, "units": None},
        "pressure": {"attr": "pressure", "type": str, "units": "mmHg"},
        "applanation": {"attr": "applanation", "type": str, "units": "mmHg"},
        "timein": {"attr": "time_in", "type": float, "units": "ms"},
        "timeout": {"attr": "time_out", "type": float, "units": "ms"},
        "meds": {"attr": "meds", "type": str, "units": None},
        "conditions": {"attr": "conditions", "type": str, "units": None},
        "notes1": {"attr": "notes1", "type": str, "units": None},
        "notes2": {"attr": "notes2", "type": str, "units": None},
        "notes3": {"attr": "notes3", "type": str, "units": None},
        "m_g2": {"attr": "m_g2", "type": float, "units": None},
        "b_g2": {"attr": "b_g2", "type": float, "units": None},
        "m_g3": {"attr": "m_g3", "type": float, "units": None},
        "b_g3": {"attr": "b_g3", "type": float, "units": None},
        "iop_cc_coef": {"attr": "iop_cc_coef", "type": float, "units": None},
        "crf_coef": {"attr": "crf_coef", "type": float, "units": None},
        "m_abc": {"attr": "m_abc", "type": float, "units": None},
        "b_abc": {"attr": "b_abc", "type": float, "units": None},
        "m_abc_out": {"attr": "m_abc_out", "type": float, "units": None},
        "b_abc_out": {"attr": "b_abc_out", "type": float, "units": None},
        "b_pp": {"attr": "b_pp", "type": float, "units": None},
        "bestweighted": {"attr": "best_weighted", "type": int, "units": None},
        "qualityindex": {"attr": "quality_index", "type": float, "units": None},
        "indexes": {"attr": "indexes", "type": str, "units": None},
        "ora_generation": {"attr": "ora_generation", "type": str, "units": None},
        "ia_iopcc": {"attr": "ia_iopcc", "type": float, "units": None},
        "ia_iopg": {"attr": "ia_iopg", "type": float, "units": None},
        "ia_ch": {"attr": "ia_ch", "type": float, "units": None},
        "ia_crf": {"attr": "ia_crf", "type": float, "units": None},
        "ia_ws": {"attr": "ia_ws", "type": float, "units": None},
        "sa_iopcc": {"attr": "sa_iopcc", "type": float, "units": None},
        "sa_iopg": {"attr": "sa_iopg", "type": float, "units": None},
        "sa_ch": {"attr": "sa_ch", "type": float, "units": None},
        "sa_crf": {"attr": "sa_crf", "type": float, "units": None},
        "sa_ws": {"attr": "sa_ws", "type": float, "units": None},
    }

    def __init__(self, db_row):
        for key, value in db_row.items():
            if key in self.field_map:
                self._set_field(self.field_map[key], value)

    def _set_field(self, field_info: dict, value: any):
        if "units" in field_info and field_info["units"] is not None:
            setattr(
                self, field_info["attr"], {"value": value, "units": field_info["units"]}
            )
        else:
            setattr(self, field_info["attr"], value)

    def __dict__(self):
        res = {}

        for field_info in self.field_map.values():
            res[field_info["attr"]] = getattr(self, field_info["attr"])

        return res

    def __str__(self):
        return json.dumps(self.__dict__(), indent=4)


class TonometerModel(Model):
    def __init__(self, session: TonometerSession, config: TonometerConfig):
        super().__init__(session=session, config=config)
        self.measures = []

    def read_results(self) -> bool:
        self.reset()
        self.measures = []

        database = TonometerDatabase(self.config.database)

        try:
            if database.open():
                patient_id = database.get_patient_id(self.session.barcode)
                if not patient_id:
                    print("not patient id")
                    return False

                left_measure_rows = database.get_measures(
                    patient_id=patient_id, eye="L"
                )
                for row in left_measure_rows:
                    measure = EyeMeasure(row)
                    self.measures.append(measure)

                right_measure_rows = database.get_measures(
                    patient_id=patient_id, eye="R"
                )
                for row in right_measure_rows:
                    measure = EyeMeasure(row)
                    self.measures.append(measure)

            else:
                print("not opened")
                return False

        except Exception as e:
            logger.critical(e)
            return False

        finally:
            database.close()

        return True

    def to_response(self):
        res = super().to_response()

        res["value"]["results"] = [measure.__dict__() for measure in self.measures]

        return res
