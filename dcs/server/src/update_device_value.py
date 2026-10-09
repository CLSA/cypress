import json
import sys
import argparse

from pine_db import PineDB, devices
from pathlib import Path

def valid_barcode(value):
    if not isinstance(value, str):
        raise argparse.ArgumentTypeError(f"'{value}' must be a string")

    if len(value) != 8:
        raise argparse.ArgumentTypeError(f"'{value}' must be 8 characters")

    return value

parser = argparse.ArgumentParser(
    prog="update_device_value",
    description=(
        "updates the device answer 'value' field with a new json response"
    )
)

parser.add_argument("device", choices=devices)
parser.add_argument("barcode", type=valid_barcode)
parser.add_argument("json_path", type=Path)

parser.add_argument("--live", action='store_true')
parser.add_argument("--dry-run", action='store_true')

args = parser.parse_args()

db = PineDB(live=args.live)

answer = db.get_device_answer_for_barcode(args.device, args.barcode)

answer_id = answer.get("id")
if answer_id is None:
    raise Exception(f"no answer id found for {args.barcode}")

if args.dry_run:
    print(json.dumps(answer, indent=2, default=str))
    print("-->")

with open(args.json_path, "r") as json_file:
    raw_data = json_file.read()
    json_data = json.loads(raw_data)

    value = json_data.get("value")
    if value is None:
        raise ValueError("json data requires 'value' attr")

    if args.dry_run:
        print(json.dumps(value, indent=2, default=str))
        sys.exit(0)
    else:
        success = db.update_device_answer(answer_id, value)
        print(success)
