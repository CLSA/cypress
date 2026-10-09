import argparse
import math

def round_half_up(num: float, decimals=0) -> float:
    multiplier = 10**decimals
    return math.floor(num * multiplier + 0.5) / multiplier

def valid_uid(value):
    if not isinstance(value, str):
        raise argparse.ArgumentTypeError(f"'{value}' must be a string")

    if len(value) != 7:
        raise argparse.ArgumentTypeError(f"'{value}' must be 7 characters")

    return value

def valid_barcode(value):
    if not isinstance(value, str):
        raise argparse.ArgumentTypeError(f"'{value}' must be a string")

    if len(value) != 8:
        raise argparse.ArgumentTypeError(f"'{value}' must be 8 characters")

    return value
