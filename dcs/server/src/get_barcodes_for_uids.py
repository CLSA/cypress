import json
import sys

from pine_db import PineDB, devices
from pathlib import Path


def get_input():
    import fileinput

    uids = []
    for line in fileinput.input():
        line = line.rstrip()
        if line:
            try:
                if not len(line) == 7:
                    raise ValueError("")

                if not line[0].isalpha():
                    raise ValueError("")
                try:
                    int(line[1:])
                except ValueError as e:
                    raise e
                uids.append(line)

            except ValueError as e:
                print(e)
                sys.exit(-1)

            except Exception as e:
                print(e)
                sys.exit(-1)
    return uids

uids = get_input()

db = PineDB(live=True)

for uid in uids:
    result = db.get_token_from_uid(uid)
    if not result:
        raise Exception(f"no token found for {uid}")

    token = result.get("token")
    print(f"{uid} {token}")







