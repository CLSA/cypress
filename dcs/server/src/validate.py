import json
import sys
import argparse
import math

from pine_db import PineDB, devices

from utils import valid_uid

def round_half_up(num: float, decimals=0) -> float:
    multiplier = 10**decimals
    return math.floor(num * multiplier + 0.5) / multiplier


def specific_validations(uid, device: str, value: dict):
    if device == "BP":
        return check_bp(uid, value)

    elif device == "HR":
        return check_hr(uid, value)

    elif device == "SP_AUTO":
        return check_spiro(uid, value)

    return True


def check_spiro(uid: str, value: dict):
    metadata = value.get("metadata")
    return True


def check_hr(uid: str, value: dict):
    results = value.get("results")

    left_results = results.get("left")
    if left_results is None:
        print(f"{uid}: left results is null")
        return False

    if len(left_results) < 7:
        print(f"{uid}: left results len < 7")
        return False

    if left_results[0]["test"] != "1000 Hz":
        print(f"{uid}: left results out of order")
        return False

    if left_results[4]["test"] != "500 Hz":
        print(f"{uid}: left results out of order")
        return False

    right_results = results.get("right")
    if right_results is None:
        print(f"{uid}: right results is null")
        return False

    if len(right_results) < 7:
        print(f"{uid}: right results len < 7")
        return False

    if right_results[0]["test"] != "1000 Hz":
        print(f"{uid}: right results out of order")
        return False

    if right_results[4]["test"] != "500 Hz":
        print(f"{uid}: right results out of order")
        return False

    return True




def check_bp(uid: str, value: dict):
    session = value.get("session")
    metadata = value.get("metadata")

    results = value.get("results")

    systolic_sum = 0
    diastolic_sum = 0
    pulse_sum = 0

    valid = True

    n = 0

    for result in results[1:]:
        systolic = result.get("systolic", {}).get("value")
        if not systolic:
            valid = False
            print(f"{uid}: no systolic")
            continue

        diastolic = result.get("diastolic", {}).get("value")
        if not diastolic:
            valid = False
            print(f"{uid}: no diastolic")
            continue

        pulse = result.get("pulse", {}).get("value")
        if not pulse:
            valid = False
            print(f"{uid}: no pulse")
            continue

        n += 1

        systolic_sum += systolic
        diastolic_sum += diastolic
        pulse_sum += pulse

    calc_sys_avg = int(round_half_up(systolic_sum / n))
    sys_avg = metadata.get("avg_systolic").get("value")
    if sys_avg != calc_sys_avg:
        print(f"{uid}: systolic average incorrect {sys_avg} != {calc_sys_avg}")
        return False

    calc_dia_avg = int(round_half_up(diastolic_sum / n))
    dia_avg = metadata.get("avg_diastolic").get("value")
    if dia_avg != calc_dia_avg:
        print(f"{uid}: diastolic average incorrect {dia_avg} != {calc_dia_avg}")
        return False

    calc_pulse_avg = int(round_half_up(pulse_sum / n))
    hr_avg = metadata.get("avg_pulse").get("value")
    if hr_avg != calc_pulse_avg:
        print(f"{uid}: pulse average incorrect {hr_avg} != {calc_pulse_avg}")
        return False

    return valid


def validate(device: str, count: bool, live: bool):
    db = PineDB(live=live)

    answers = db.get_device_answers(device, limit=count)

    invalid_answers = []

    for answer in answers:
        uid = answer.get("uid")
        token = answer.get("token")

        response = answer.get("value")
        if not response:
            print(f"{uid}: no response")
            invalid_answers.append(answer)
            continue

        session = response.get("session")
        if not session:
            print(f"{uid}: no session")
            invalid_answers.append(answer)
            continue

        cypress_version = session.get("cypress_version", "1.0.0")

        session_uid = session.get("uid")
        if session_uid != uid:
            print(f"{uid}: session uid '{session_uid}' does not match")
            invalid_answers.append(answer)
            continue

        session_token = session.get("barcode")
        if session_token != token:
            print(f"{uid}: session token '{session_token}' does not match {token}")
            invalid_answers.append(answer)
            continue

        metadata = response.get("metadata")
        if metadata is None:
            print(f"{uid}: no metadata")
            invalid_answers.append(answer)
            continue

        results = response.get("results")
        if results is None:
            print(f"{uid}: no results")
            invalid_answers.append(answer)
            continue

        if not specific_validations(uid, device, response):
            invalid_answers.append(answer)

        print(f"OK: {uid}, {token}, {cypress_version}")

    return invalid_answers


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        prog="validate responses",
        description=(
            "gets the most recent <device> responses and performs validation"
        )
    )

    parser.add_argument("device", choices=devices)
    parser.add_argument("--count", type=int, default=5)
    parser.add_argument("--live", action='store_true')

    args = parser.parse_args()

    invalid_answers = validate(args.device, args.count, args.live)

    if invalid_answers:
        #print(json.dumps(invalid_answers, indent=2, default=str))
        pass
    else:
        pass
        #print("OK")
