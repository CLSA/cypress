from pine_db import PineDB

from datetime import datetime
from zoneinfo import ZoneInfo

import json
import configparser

config = configparser.ConfigParser()
config.read("config.ini")

tz = config["DEFAULT"].get("TZ", "UTC")


def get_today():
    now = datetime.now()

    start = datetime(
        day=now.day,
        month=now.month,
        year=now.year,
        hour=0,
        minute=0,
        second=0,
        tzinfo=ZoneInfo(tz),
    )

    end = datetime(
        day=now.day,
        month=now.month,
        year=now.year,
        hour=23,
        minute=59,
        second=59,
        tzinfo=ZoneInfo(tz),
    )

    return (
        start.astimezone(tz=ZoneInfo("UTC")).isoformat(),
        end.astimezone(tz=ZoneInfo("UTC")).isoformat(),
    )


if __name__ == "__main__":
    db = PineDB(live=True)

    start, end = get_today()
    interviews = db.get_interviews_from_range(start, end)

    if interviews:
        print(json.dumps(interviews, indent=2, default=str))
