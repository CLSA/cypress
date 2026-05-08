from pathlib import Path
from devices.model import Model


class GripStrengthModel(Model):
    pass


class ParadoxReader:
    def __init__(self, database_path: Path = None):
        # with open(database_path, 'rb') as f:
        #     self.binary_data = f.read()
        pass

    def read_header(self, file):
        pass

    def read_blocks(self):
        pass

    def read_records(self):
        pass

    def _read_short(self):
        data1 = bytes(b"\x01\x02\x03")
        data2 = bytes(b"\x03\x02\x01")

        print(data1[0])
        print(int.from_bytes(data1, "big", signed=True))

        print(data2[2])
        print(int.from_bytes(data2, "little", signed=True))

    def _read_int(self):
        pass


if __name__ == "__main__":
    reader = ParadoxReader()
    reader._read_short()
