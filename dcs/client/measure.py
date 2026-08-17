import json
from typing import TypedDict


class Field(TypedDict):
    attr: str
    data_type: type[int | float | str]
    units: str | None


class Record:
    # Maps raw field names (from database, file, etc) to preferred names and their data type + unit (if applicable)
    field_map: dict[str, Field] = {}

    def __init__(self, raw_data: dict):
        self._set_fields(raw_data)

    def _set_fields(self, raw_data):
        for field in list(self.field_map.values()):
            self._set_field(field, None)

        for raw_name, value in raw_data.items():
            if raw_name in self.field_map:
                self._set_field(self.field_map[raw_name], value)

    def _set_field(self, field: Field, value: any):
        if "units" in field and field["units"] is not None:
            setattr(
                self,
                field["attr"],
                {
                    "value": field["data_type"](value) if value is not None else None,
                    "units": field["units"],
                },
            )
        else:
            setattr(
                self,
                field["attr"],
                field["data_type"](value) if value is not None else None,
            )

    def to_dict(self):
        return self.__dict__()

    def __dict__(self):
        res = {}

        for field_info in self.field_map.values():
            res[field_info["attr"]] = getattr(self, field_info["attr"])

        return res

    def __str__(self):
        return json.dumps(self.to_dict(), indent=4)
