import sys
import logging

from PySide6.QtCore import Qt, QDateTime

logger = logging.getLogger("dxa")

def compute_years_difference(first, second) -> float:
    first_dt = QDateTime.fromString(first, Qt.DateFormat.ISODate)
    second_dt = QDateTime.fromString(second, Qt.DateFormat.ISODate)

    if not first_dt.isValid():
        logger.warning(f"first date time is not valid {first}")
        return 0.0

    if not second_dt.isValid():
        logger.warning(f"second date time is not valid {second}")
        return 0.0

    logger.info(first_dt.toSecsSinceEpoch())
    logger.info(second_dt.toSecsSinceEpoch())

    diff = first_dt.toSecsSinceEpoch() - second_dt.toSecsSinceEpoch()
    logger.info(diff)

    years_diff = diff / (60.0 * 60.0 * 24.0 * 365.25)

    logger.info(years_diff)

    return years_diff


def compute_age_bracket(age: float, age_table: list[float]) -> dict:
    age_min = sys.float_info.min
    age_max = sys.float_info.max

    for i in range(len(age_table) - 1):
        _min = age_table[i]
        _max = age_table[i + 1]

        if age >= _min and age <= _max:
            age_min = _min
            age_max = _min if age == _min else _max
        elif age > _max:
            age_min = _max
            age_max = _max

    if age_min == sys.float_info.min:
        age_min = age

    if age_max == sys.float_info.max:
        age_max = age

    age_span = age_max - age_min

    return {
        "age_min": age_min,
        "age_max": age_max,
        "age_span": age_span
    }