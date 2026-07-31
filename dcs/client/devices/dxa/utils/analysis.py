import sys

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

def compute_tz_score(reference_db, patient_data: dict, scan_date: str):
    #
    # INPUT
    # refType
    # sex
    # ethnicity
    # method
    # ageYoung (x-value)
    # curveId
    # UNIQUE_ID
    # AGE_YOUNG
    # X_VALUE
    # Y_VALUE
    # L_VALUE
    # STD
    #

    # AP lumbar spine:
    # - identify the included vertebral levels
    # - sum the area and sum the bmc of the included vertebral levels
    # - compute the revised total bmd from summed bmc / summed area
    # - provide the proper bone range code for total bmd
    pass


