import json
from devices.spirometer.model import SpirometerModel

from pathlib import Path


if __name__ == "__main__":

    model = SpirometerModel(None, None)

    model.read_results(Path())

    print(model.to_response()["value"]["metadata"])
    #print(json.dumps(model.to_response(), indent=4))