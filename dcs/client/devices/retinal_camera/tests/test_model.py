from devices.retinal_camera.session import RetinalCameraSession
from devices.retinal_camera.config import RetinalCameraConfig

from devices.retinal_camera.settings import logger

from utils import FileInfo, get_file_info


class OCTMeasure:
    def __init__(self, name: str, path: str, size: str):
        self.name = name
        self.path = path
        self.size = size

    def __dict__(self):
        return {
            "name": self.name,
            "path": self.path,
            "size": self.size,
        }


class RetinalCameraModel:
    def __init__(session: RetinalCameraSession, config: RetinalCameraConfig):
        super().__init__(session=session, config=config)

    def read_results(self) -> bool:
        self.measures = []
        for path in self.config.export_path.iterdir():
            if not path.is_file():
                continue

            file_info: FileInfo = get_file_info(path)

            filename = path.name
            filename_parts = filename.split("_")
            if len(filename_parts) < 2:
                logger.warning(f"{filename} file name is an invalid format")

            barcode = filename_parts[0]
            if barcode != self.session.barcode:
                continue

            side = filename_parts[1]
            if side != self.session.side:
                continue

            name = None
            if "_OPT_" in filename:
                name = f"OCT_{"LEFT" if side == "L" else "RIGHT"}.dcm"
            elif "_OP_" in filename:
                name = f"EYE_{"LEFT" if side == "L" else "RIGHT"}.dcm"
            else:
                continue

            measure = OCTMeasure(
                name=name, path=str(path.resolve()), size=file_info.readable_size
            )

            file_info.send_name = name

            self.measures.append(measure)
            self.add_file(file_info)
