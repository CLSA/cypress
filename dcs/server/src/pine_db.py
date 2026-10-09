import sys
import json
import mariadb

import configparser

from enum import Enum
from pathlib import Path


config = configparser.ConfigParser()
config.read("config.ini")

devices = {
    # Reception
    "CONSENT_GP",

    # Measure 1
    "WT",
    "BP",
    "ECG",
    "ECHO",
    "SP_AUTO",
    "DXA1",
    "FRAX",
    "DXA2",

    # Interview 1
    "HR",
    "CDTT",
    "CRT",

    # Measure 2
    "GRIP",
    "TON",
    "RET_L",
    "RET_R"
}


class PineDB:
    def __init__(self, live=False):
        self.live = live

        db_config = config["DEFAULT"]
        self.conn_params = {
            "user": db_config["LIVE_USER"] if live else config["GHOST_USER"],
            "database": db_config["LIVE_DB"] if live else config["GHOST_DB"],
            "password": db_config["PASS"],
            "host": db_config["HOST"]
        }

    def get_answer(self, answer_id):
        try:
            with mariadb.connect(**self.conn_params) as conn:
                with conn.cursor() as cursor:
                    cursor.execute("SELECT * FROM answer WHERE id = ?", (answer_id,))
                    columns = [col[0] for col in cursor.description]
                    row = cursor.fetchone()
                    return dict(zip(columns, row))
        except Exception as e:
            print(e)

    def get_token_from_uid(self, uid):
        try:
            with mariadb.connect(**self.conn_params) as conn:
                with conn.cursor() as cursor:
                    cursor.execute(
                        "SELECT token from respondent "
                        "JOIN live_cenozo.participant ON participant.id = participant_id "
                        "WHERE uid = ?", (uid,)
                    )

                    columns = [col[0] for col in cursor.description]
                    row = cursor.fetchone()
                    return dict(zip(columns, row))
        except Exception as e:
            print(e)

    def get_device_answer_for_barcode(self, device: str, barcode: str):
        try:
            with mariadb.connect(**self.conn_params) as conn:
                with conn.cursor() as cursor:
                    cursor.execute(
                        "SELECT answer.id, question.name, uid, respondent.token, response.comments, answer.value "
                        f"FROM {'live_cenozo' if self.live else 'ghost_cenozo'}.participant "
                        "JOIN respondent ON participant.id = respondent.participant_id "
                        "JOIN response ON respondent.id = response.respondent_id "
                        "JOIN answer ON response.id = answer.response_id "
                        "JOIN question ON answer.question_id = question.id "
                        "WHERE question.name = ? "
                        "AND respondent.token = ?",
                        (device, barcode)
                    )
                    columns = [col[0] for col in cursor.description]
                    row = cursor.fetchone()

                    res = dict(zip(columns, row))
                    res["value"] = json.loads(res.get("value", {}))

                    return res

        except Exception as e:
            print(e)

    def get_device_answer_for_uid(self, device: str, uid: str):
        try:
            with mariadb.connect(**self.conn_params) as conn:
                with conn.cursor() as cursor:
                    cursor.execute(
                        "SELECT answer.id, question.name, uid, respondent.token, response.comments, answer.value "
                        f"FROM {'live_cenozo' if self.live else 'ghost_cenozo'}.participant "
                        "JOIN respondent ON participant.id = respondent.participant_id "
                        "JOIN response ON respondent.id = response.respondent_id "
                        "JOIN answer ON response.id = answer.response_id "
                        "JOIN question ON answer.question_id = question.id "
                        "WHERE question.name = ? "
                        "AND uid = ?",
                        (device, uid)
                    )
                    columns = [col[0] for col in cursor.description]
                    row = cursor.fetchone()

                    res = dict(zip(columns, row))
                    res["value"] = json.loads(res.get("value", {}))

                    return res
        except Exception as e:
            print(e)

    def get_device_answers(self, device: str, limit=1):
        """
        Returns a limited number of device answers, ordered by answer id "

        """
        try:
            with mariadb.connect(**self.conn_params) as conn:
                with conn.cursor() as cursor:
                    cursor.execute(
                        "SELECT * FROM (SELECT answer.id, question.name, respondent.token, uid, answer.update_timestamp, response.comments, answer.value "
                        f"FROM {'live_cenozo' if self.live else 'ghost_cenozo'}.participant "
                        "JOIN respondent ON participant.id = respondent.participant_id "
                        "JOIN response on respondent.id = response.respondent_id "
                        "JOIN answer ON response.id = answer.response_id "
                        "JOIN question ON answer.question_id = question.id "
                        "WHERE question.name = ? "
                        "ORDER BY answer.update_timestamp DESC "
                        "LIMIT ? ) AS recent_records ORDER BY update_timestamp ASC",
                        (device, limit)
                    )
                    columns = [col[0] for col in cursor.description]
                    res = []
                    for row in cursor:
                        row_data = dict(zip(columns, row))
                        row_data["value"] = json.loads(row_data.get("value", {}))
                        res.append(row_data)

                    return res
        except Exception as e:
            print(e)


    def update_device_answer(self, answer_id: int, new_response: dict):
        try:
            with mariadb.connect(**self.conn_params) as conn:
                with conn.cursor() as cursor:
                    cursor.execute(
                        "UPDATE answer SET value = ? WHERE id = ?",
                        (json.dumps(new_response), answer_id)

                    )

                    conn.commit()
                    return True
        except Exception as e:
            print(e)
            return False
