import json
import sys
import argparse

from pine_db import PineDB, devices

from utils import valid_uid


parser = argparse.ArgumentParser(
    prog="get_device_answer_by_uid", 
    description=(
        "gets the device response for a given uid " 
        "from the pine database (ghost or live)"
    )
)

parser.add_argument("device", choices=devices)
parser.add_argument("uid", type=valid_uid)
parser.add_argument("--count", type=int, default=5)
parser.add_argument("--live", action='store_true')

args = parser.parse_args()

db = PineDB(live=args.live)

answer = db.get_device_answer_for_uid(args.device, args.uid)

print(json.dumps(answer, indent=2, default=str))

