from pine_db import PineDB

from datetime import datetime

import json

def get_today():
    now = datetime.now()

    today = datetime(
        day=now.day,
        month=now.month,
        year=now.year,
        hour=0,
        minute=0,
        second=0
    )

    return today.isoformat()

if __name__ == '__main__':
    db = PineDB(live=True)

    today = get_today()

    interviews = db.get_interviews_from_date(today)

    if interviews:
        print(json.dumps(interviews, indent=2))



