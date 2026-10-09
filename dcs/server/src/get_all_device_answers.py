import json
import sys
import argparse

from pine_db import PineDB, devices

from utils import valid_uid

def get_all_device_answers(live: bool = False):
    db = PineDB(live=live)

    res = {
        "uid": args.uid,
        "rooms": {}
    }

    # Reception
    gp = db.get_device_answer_for_uid("CONSENT_GP", args.uid)

    res["reception"] = {
        "CONSENT_GP": gp
    }
    # Measure 1
    wt = db.get_device_answer_for_uid("WT", args.uid)
    bp = db.get_device_answer_for_uid("BP", args.uid)
    dxa1 = db.get_device_answer_for_uid("DXA1", args.uid)
    frax = db.get_device_answer_for_uid("FRAX", args.uid)
    dxa2 = db.get_device_answer_for_uid("DXA2", args.uid)
    sp_auto = db.get_device_answer_for_uid("SP_AUTO", args.uid)
    ecg = db.get_device_answer_for_uid("ECG", args.uid)
    echo = db.get_device_answer_for_uid("ECHO", args.uid)

    res["measure_1"] = {
        "WT": wt,
        "BP": bp,
        "DXA1": dxa1,
        "FRAX": frax,
        "DXA2": dxa2,
        "SP_AUTO": sp_auto,
        "ECG": ecg,
        "ECHO": echo
    }


    # Interview 1
    hr = db.get_device_answer_for_uid("HR", args.uid)
    cdtt = db.get_device_answer_for_uid("CDTT", args.uid)
    crt = db.get_device_answer_for_uid("CRT", args.uid)

    res["interview_1"] = {
        "HR": hr,
        "CDTT": cdtt,
        "CRT": crt,
    }

    # Measure 2
    grip = db.get_device_answer_for_uid("GRIP", args.uid)
    ton = db.get_device_answer_for_uid("TON", args.uid)
    ret_l = db.get_device_answer_for_uid("RET_L", args.uid)
    ret_r = db.get_device_answer_for_uid("RET_R", args.uid)

    res["measure_2"] = {
        "GRIP": grip,
        "TON": ton,
        "RET_L": ret_l,
        "RET_R": ret_r
    }

    return res


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        prog="get_all_device_answers",
        description=(
            "gets all device responses for a given uid "
            "from the pine database (ghost or live)"
        )
    )

    parser.add_argument("uid", type=valid_uid)
    parser.add_argument("--live", action='store_true')
    args = parser.parse_args()

    answers = get_all_device_answers(live=args.live)
    print(json.dumps(answers, indent=2, default=str))
