import logging

from typing import override
from typing import Literal

from model import Model
from measure import Record

from devices.tonometer.settings import DEVICE_NAME
from devices.tonometer.session import TonometerSession
from devices.tonometer.config import TonometerConfig
from devices.tonometer.database import TonometerDatabase

logger = logging.getLogger(DEVICE_NAME)


class EyeMeasure(Record):
    field_map = {
        "measureid": {"attr": "measure_id", "data_type": int, "units": None},
        "measurenumber": {"attr": "measure_number", "data_type": int, "units": None},
        "measuredate": {"attr": "measure_date", "data_type": str, "units": None},
        "sessiondate": {"attr": "session_datetime", "data_type": str, "units": None},
        "patientid": {"attr": "patient_id", "data_type": str, "units": None},
        "eye": {"attr": "eye", "data_type": str, "units": None},
        "oraserialnumber": {
            "attr": "ora_serial_number",
            "data_type": str,
            "units": None,
        },
        "orasoftware": {"attr": "ora_software", "data_type": str, "units": None},
        "pcsoftware": {"attr": "pc_software", "data_type": str, "units": None},
        "iopg": {"attr": "iopg", "data_type": float, "units": "mmHg"},
        "iopcc": {"attr": "iopcc", "data_type": float, "units": "mmHg"},
        "crf": {"attr": "crf", "data_type": float, "units": "mmHg"},
        "cctavg": {"attr": "cct_avg", "data_type": float, "units": "um"},
        "cctlowest": {"attr": "cct_lowest", "data_type": float, "units": "um"},
        "cctsd": {"attr": "cct_sd", "data_type": float, "units": "um"},
        "ch": {"attr": "ch", "data_type": float, "units": None},
        "tearfilmvalue": {"attr": "tear_film_value", "data_type": float, "units": None},
        "pressure": {"attr": "pressure", "data_type": str, "units": "mmHg"},
        "applanation": {"attr": "applanation", "data_type": str, "units": "mmHg"},
        "timein": {"attr": "time_in", "data_type": float, "units": "ms"},
        "timeout": {"attr": "time_out", "data_type": float, "units": "ms"},
        "meds": {"attr": "meds", "data_type": str, "units": None},
        "conditions": {"attr": "conditions", "data_type": str, "units": None},
        "notes1": {"attr": "notes_1", "data_type": str, "units": None},
        "notes2": {"attr": "notes_2", "data_type": str, "units": None},
        "notes3": {"attr": "notes_3", "data_type": str, "units": None},
        "m_g2": {"attr": "m_g2", "data_type": float, "units": None},
        "b_g2": {"attr": "b_g2", "data_type": float, "units": None},
        "m_g3": {"attr": "m_g3", "data_type": float, "units": None},
        "b_g3": {"attr": "b_g3", "data_type": float, "units": None},
        "iop_cc_coef": {"attr": "iop_cc_coef", "data_type": float, "units": None},
        "crf_coef": {"attr": "crf_coef", "data_type": float, "units": None},
        "m_abc": {"attr": "m_abc", "data_type": float, "units": None},
        "b_abc": {"attr": "b_abc", "data_type": float, "units": None},
        "m_abc_out": {"attr": "m_abc_out", "data_type": float, "units": None},
        "b_abc_out": {"attr": "b_abc_out", "data_type": float, "units": None},
        "b_pp": {"attr": "b_pp", "data_type": float, "units": None},
        "bestweighted": {"attr": "best_weighted", "data_type": int, "units": None},
        "qualityindex": {"attr": "quality_index", "data_type": float, "units": None},
        "indexes": {"attr": "indexes", "data_type": str, "units": None},
        "ora_generation": {"attr": "ora_generation", "data_type": str, "units": None},
        "ia_iopcc": {"attr": "ia_iopcc", "data_type": float, "units": None},
        "ia_iopg": {"attr": "ia_iopg", "data_type": float, "units": None},
        "ia_ch": {"attr": "ia_ch", "data_type": float, "units": None},
        "ia_crf": {"attr": "ia_crf", "data_type": float, "units": None},
        "ia_ws": {"attr": "ia_ws", "data_type": float, "units": None},
        "sa_iopcc": {"attr": "sa_iopcc", "data_type": float, "units": None},
        "sa_iopg": {"attr": "sa_iopg", "data_type": float, "units": None},
        "sa_ch": {"attr": "sa_ch", "data_type": float, "units": None},
        "sa_crf": {"attr": "sa_crf", "data_type": float, "units": None},
        "sa_ws": {"attr": "sa_ws", "data_type": float, "units": None},
    }


class TonometerTest(Record):
    field_map = {
        "birthdate": {"attr": "birth_date", "data_type": str, "units": None},
        "id": {"attr": "id", "data_type": int, "units": None},
        "sex": {"attr": "sex", "data_type": int, "units": None},
        "raceid": {"attr": "race_id", "data_type": int, "units": None},
    }

    metadata_attrs_from_measure = set(
        [
            "ora_serial_number",
            "ora_software",
            "pc_software",
            "b_abc",
            "b_g2",
            "b_g3",
            "b_pp",
            "best_weighted",
            "conditions",
            "crf_coef",
            "date_of_birth",
            "iop_cc_coef",
            "m_abc",
            "m_g2",
            "m_g3",
            "measure_number",
            "meds",
            "notes_1",
            "notes_2",
            "notes_3",
            "patient_id",
            "session_datetime",
        ]
    )

    def __init__(self, db_row, measures: list[EyeMeasure]):
        super().__init__(raw_data=db_row)
        self.measures = measures

    def get_metadata(self):
        measure_metadata = self.to_dict()

        for key in list(self.metadata_attrs_from_measure):
            for measure in self.measures:
                measure_dict = measure.to_dict()
                value = measure_dict.get(key, None)

                if key in measure_metadata and measure_metadata[key] != value:
                    logger.warning(f"{key}: {value} does not match")

                measure_metadata[key] = value

        patient_metadata = self.to_dict()

        return {**patient_metadata, **measure_metadata}

    def get_measures(self, eye: Literal["L", "R", "Both"]):
        results = []
        for measure in self.measures:
            if eye == "Both" or measure.eye == eye:
                results.append(measure)
        return results


class TonometerModel(Model):
    def __init__(self, session: TonometerSession, config: TonometerConfig):
        super().__init__(session=session, config=config)

    def read_results(self) -> tuple[bool, str | None]:
        self.reset()

        database = TonometerDatabase(self.config.database)

        try:
            if database.open():
                success, result = database.get_patient_id(self.session.barcode)
                if not success:
                    return False, result

                patient_id = result

                success, result = database.get_participant(self.session.barcode)
                if not success:
                    return False, result

                participant = result

                success, result = database.get_measures(patient_id=patient_id)
                if not success:
                    return False, result

                measures = [EyeMeasure(row) for row in result]

                if len(measures) == 0:
                    return False, "no measures found"

                self.test = TonometerTest(
                    db_row=participant,
                    measures=measures,
                )
            else:
                return False, "could not open database"

        except Exception as e:
            logger.critical(e)
            return False, "something went wrong"
        finally:
            database.close()

        return True, None

    @override
    def reset(self):
        super().reset()

        self.test = None

    def to_response(self):
        res = super().to_response()

        metadata = self.test.get_metadata()
        device_data = {
            "ora_serial_number": metadata["ora_serial_number"],
            "ora_software": metadata["ora_software"],
            "pc_software": metadata["pc_software"],
        }

        left_results = [
            measure.to_dict() for measure in self.test.get_measures(eye="L")
        ]

        right_results = [
            measure.to_dict() for measure in self.test.get_measures(eye="R")
        ]

        res["value"]["results"] = left_results + right_results
        res["value"]["metadata"] = metadata
        res["value"]["device_data"] = device_data

        return res
