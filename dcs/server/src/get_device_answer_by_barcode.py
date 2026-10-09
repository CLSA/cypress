import json
import sys
import argparse

from pine_db import PineDB, devices

from utils import valid_barcode


parser = argparse.ArgumentParser(
    prog="get_device_answer_by_barcode",
    description=(
        "gets a device answer for a given barcode "
        "from the pine database (ghost or live)"
    )
)

parser.add_argument("device", choices=devices)
parser.add_argument("barcode", type=valid_barcode)
parser.add_argument("--live", action='store_true')

args = parser.parse_args()

db = PineDB(live=args.live)

answer = db.get_device_answer_for_barcode(args.device, args.barcode)

print(json.dumps(answer, indent=2, default=str))
