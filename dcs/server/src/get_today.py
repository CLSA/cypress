from pine_db import PineDB

from datetime import datetime

import json

def get_today():
    now = datetime.now()

    start = datetime(
        day=now.day,
        month=now.month,
        year=now.year,
        hour=0,
        minute=0,
        second=0
    )

    end = datetime(
        day=now.day,
        month=now.month,
        year=now.year,
        hour=23,
        minute=59,
        second=59
    )

    return start.isoformat(), end.isoformat()

if __name__ == '__main__':
    db = PineDB(live=True)

    start, end = get_today()

    interviews = db.get_interviews_from_range(start, end)

    if interviews:
        print(json.dumps(interviews, indent=2, default=str))



