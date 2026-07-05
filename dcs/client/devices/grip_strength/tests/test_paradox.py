from pathlib import Path

from devices.grip_strength.paradox import ParadoxDb

if __name__ == '__main__':
    test_data = ParadoxDb(Path("./devices/grip_strength/tests/ZGripTestData.DB"))

    print(test_data.header)