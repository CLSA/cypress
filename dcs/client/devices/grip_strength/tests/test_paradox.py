from pathlib import Path

from devices.grip_strength.paradox import ParadoxDb

if __name__ == '__main__':
    test_data = ParadoxDb(Path(""))

    print(test_data.read_records())