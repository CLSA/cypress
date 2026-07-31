import json

from pathlib import Path
from enum import Enum

ONE_POUND_FORCE_IN_NEWTONS = 4.4482216152605
ONE_POUND_FORCE_IN_KILOGRAMS = ONE_POUND_FORCE_IN_NEWTONS / 9.80665


def as_newtons(thousanthsPounds: int):
    return float(thousanthsPounds) * ONE_POUND_FORCE_IN_NEWTONS / 1000.0


def as_kg(thousanthsPounds: int):
    return float(thousanthsPounds) * ONE_POUND_FORCE_IN_KILOGRAMS / 1000.0


def read_short(data: bytes, offset: int) -> int:
    return int.from_bytes(data[offset : offset + 2], byteorder="little", signed=True)


def read_ushort(data: bytes, offset: int) -> int:
    return int.from_bytes(data[offset : offset + 2], byteorder="little", signed=False)


def read_ubyte(data: bytes, offset: int) -> int:
    return int.from_bytes(data[offset : offset + 1], byteorder="little", signed=False)


def read_int(data: bytes, offset: int) -> int:
    return int.from_bytes(data[offset : offset + 4], byteorder="little", signed=False)


def read_ptr(data: bytes, offset: int) -> int:
    return int.from_bytes(data[offset : offset + 4], byteorder="little", signed=True)


def read_alpha(data: bytes) -> str:
    print("read alpha")

    val = ""

    for b in data:
        val += chr(b)

    return val


class ParadoxFieldType(Enum):
    Alpha = 0x01
    Date = 0x02
    ShortInteger = 0x03
    LongInteger = 0x04
    Currency = 0x05
    Number = 0x06
    Logical = 0x09
    MemoBlob = 0x0C
    Blob = 0x0D
    FormattedMemoBlob = 0x0E
    Ole = 0x0F
    GraphicBlob = 0x10
    Time = 0x14
    Timestamp = 0x15
    AutoInc = 0x16
    Bcd = 0x17
    Bytes = 0x18


class ParadoxFieldInfo:
    def __init__(
        self, name: str = None, size: int = None, field_type: ParadoxFieldType = None
    ):
        self.name = name
        self.size = size
        self.field_type = field_type

    def __str__(self):
        return f"{self.name} {self.field_type} of size {self.size}B "


class ParadoxDbHeader:
    def __init__(self, data: bytes):
        if len(data) < 0x0055:
            raise Exception("Invalid header length")

        self.field_info = []

        self.record_size = read_short(data, 0x0000)
        self.header_size = read_short(data, 0x0002)
        self.file_type = read_ubyte(data, 0x0004)
        self.max_table_size = read_ubyte(data, 0x0005)
        self.num_records = read_int(data, 0x0006)
        self.next_block = read_ushort(data, 0x000A)
        self.file_blocks = read_ushort(data, 0x000C)
        self.first_block = read_ushort(data, 0x000E)
        self.last_block = read_ushort(data, 0x0010)
        self.modified_flags_1 = read_ubyte(data, 0x0014)
        self.index_field_number = read_ubyte(data, 0x0015)
        self.primary_index_workspace = read_ptr(data, 0x0016)
        self.num_fields = read_short(data, 0x0021)
        self.primary_key_fields = read_short(data, 0x0023)
        self.encryption_1 = read_int(data, 0x0025)
        self.sort_order = read_ubyte(data, 0x0029)
        self.modified_flags_2 = read_ubyte(data, 0x002A)
        self.change_count_1 = read_ubyte(data, 0x002D)
        self.change_count_2 = read_ubyte(data, 0x002E)
        self.table_name_ptr = read_ptr(data, 0x0030)
        self.field_info_ptr = read_ptr(data, 0x0034)
        self.write_protected = read_ubyte(data, 0x0038)
        self.file_version_id = read_ubyte(data, 0x0039)
        self.max_blocks = read_ushort(data, 0x003A)
        self.aux_passwords = read_ubyte(data, 0x003D)
        self.crypt_info_start_ptr = read_ptr(data, 0x0040)
        self.crypt_info_end_ptr = read_ptr(data, 0x0044)
        self.auto_inc = read_int(data, 0x0049)
        self.index_update_required = read_ubyte(data, 0x004F)
        self.ref_integrity = read_ubyte(data, 0x0055)

        self._read_fields(data)

    def _read_fields(self, data):
        offset = 0x0056

        if self.file_version_id > 0x04:
            #print("file_version_id is greater than 4")
            offset = 0x0078

        for i in range(self.num_fields):
            field = ParadoxFieldInfo()
            field.field_type = ParadoxFieldType(read_ubyte(data, offset))
            field.size = read_ubyte(data, offset + 1)

            offset += 2

            self.field_info.append(field)

        # Skip tableNamePtr
        offset += 4

        # Skip fieldNamePtrArray
        offset += 4 * self.num_fields

        # Skip tableName
        offset += 261 if self.file_version_id > 4 else 79

        for field in self.field_info:
            name = ""
            char = data[offset]

            while char > 0:
                name += chr(char)
                offset += 1
                char = data[offset]

            offset += 1
            field.name = name


class ParadoxDbBlock:
    def __init__(
        self,
        header: ParadoxDbHeader,
        data: bytes,
        block_number: int,
        next_block: int,
        prev_block: int,
        offset_to_last_record: int,
        file_offset: int,
    ):

        self.header = header
        self.data = data
        self.block_number = block_number
        self.next_block = next_block
        self.prev_block = prev_block
        self.offset_to_last_record = offset_to_last_record
        self.file_offset = file_offset

    def num_records(self):
        return int(self.offset_to_last_record / self.header.record_size + 1)

    def read_records(self):
        records = []

        offset = self.file_offset

        #print(chr(0x00))

        length = 0
        for field_info in self.header.field_info:
            length += field_info.size

        def fix_sign(data):
            data = bytearray(data)
            if data[0] & 0x80:
                data[0] &= 0x7F

            return data

        record = {}

        for field_info in self.header.field_info:
            name = field_info.name
            size = field_info.size
            field_type = field_info.field_type

            #print(name, field_type, size)

            if field_type == ParadoxFieldType.LongInteger:
                integer = int.from_bytes(fix_sign(self.data[offset : offset + size]), byteorder='big', signed=True)

                #if "Rep" in name or "Average" in name or "Maximum" in name:
                #    print(as_kg(integer))
                #else:
                #    print(integer)

                record[name] = integer

            elif field_type == ParadoxFieldType.Alpha:
                data = self.data[offset:]
                res = ""
                i = 0

                while data[i] != chr(0x00) and i < size:
                    res += chr(data[i])
                    i += 1

                #print(res)
                record[name] = res.replace('\x00', "").strip()

            elif field_type == ParadoxFieldType.Logical:
                data = bytearray(self.data[offset : offset + size])
                val = None
                if data[0] & 0x80:
                    val = bool(data[0] & 0x7F)
                elif data[0] == 0:
                    val = bool(data[0] & 0x7F)

                #print(val)
                record[name] = val

            offset += field_info.size

        records.append(record)

            #print(name, size, field_type, data_bytes)
            # print(name, size, field_type, data_bytes)

            # if name == 'Position':
            #     print(self.data[offset - size : offset + size + 5])

            # if field_type == ParadoxFieldType.Alpha:
            #     print(read_alpha(data_bytes))

        return records


class ParadoxDb:
    def __init__(self, database_path: Path):
        with open(database_path, "rb") as file:
            self.data = file.read()
            self.header = ParadoxDbHeader(self.data)
            self.blocks = self._read_blocks()

    def _read_blocks(self):
        offset = self.header.header_size

        blocks = []

        for i in range(self.header.file_blocks):
            next_block = read_ushort(self.data, offset)
            prev_block = read_ushort(self.data, offset + 2)
            offset_to_last_record = read_ushort(self.data, offset + 4)
            offset += 6

            block = ParadoxDbBlock(
                header=self.header,
                data=self.data,
                block_number=i + 1,
                next_block=next_block,
                prev_block=prev_block,
                offset_to_last_record=offset_to_last_record,
                file_offset=offset,
            )
            blocks.append(block)

            #print("num records: ", block.num_records())

        return blocks

    def read_records(self):
        records = []
        for block in self.blocks:
            block_records = block.read_records()
            for record in block_records:
                print(record)
                records.append(record)
        return records


