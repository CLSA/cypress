import json
import sys
import argparse

from pine_db import PineDB, devices

parser = argparse.ArgumentParser(
    prog="get_recent_device_answers", 
    description=(
        "gets <count> of the most recent <device> responses " 
        "from the database (ghost or live)"
    )
)

parser.add_argument("device", choices=devices)
parser.add_argument("--count", type=int, default=5)
parser.add_argument("--live", action='store_true')

args = parser.parse_args()

db = PineDB(live=args.live)

answers = db.get_device_answers(args.device, limit=args.count)
print(json.dumps(answers, indent=2, default=str))
