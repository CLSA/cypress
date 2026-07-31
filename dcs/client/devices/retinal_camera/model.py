import logging

from pathlib import Path
from typing import override

from model import Model

from devices.retinal_camera.session import RetinalCameraSession
from devices.retinal_camera.config import RetinalCameraConfig

from utils import get_file_info

logger = logging.getLogger("retinal_camera")


class OCTMeasure:
    def __init__(self, name, image_type, create_time, sequence_number, file_path):
        self.name = name
        self.image_type = image_type
        self.create_time = int(create_time)
        self.sequence_number = int(sequence_number)
        self.file_path = file_path
        self.file_info = get_file_info(self.file_path)

    def to_dict(self):
        return {
            "name": self.name,
            "image_type": self.image_type,
            "create_time": self.create_time,
            "sequence_number": self.sequence_number,
            "path": str(self.file_path.resolve()),
            "size": self.file_info.readable_size,
        }


class RetinalCameraModel(Model):
    def __init__(self, session: RetinalCameraSession, config: RetinalCameraConfig):
        super().__init__(session, config)
        self.measures = []

    def read_results(self, output_directory: Path) -> tuple[bool, str]:
        if not output_directory.exists():
            logger.error(f"{str(output_directory.resolve())} does not exist")
            return False, "something went wrong"

        if not output_directory.is_dir():
            logger.error(f"{str(output_directory.resolve())} is not a directory")
            return False, "something went wrong"

        self.reset()

        for file_path in output_directory.iterdir():
            logger.debug(f"processing {str(file_path.resolve())}...")

            if file_path.suffix != ".dcm":
                logger.warning(f"file ext: {file_path.suffix} is not .dcm")
                continue

            name = file_path.name.replace(".dcm", "")
            name_parts = name.strip().split("_")

            if len(name_parts) < 5:
                logger.warning(f"filename: is incorrect {name}")
                continue

            barcode = name_parts[0]
            if barcode != self.session.barcode:
                logger.warning(
                    f"filename: barcode {barcode} does not match session {self.session.barcode}"
                )
                continue

            side = name_parts[1]
            if side != self.session.side:
                logger.warning(
                    f"filename: side ({side}) does not match session {self.session.side}"
                )
                continue

            image_type = name_parts[2]
            if image_type != "OP" and image_type != "OPT":
                logger.warning(
                    f"filename: image type {image_type} does not match OP or OPT"
                )
                continue

            create_time = name_parts[3]
            sequence_number = name_parts[4]

            name = None
            if image_type == "OPT":
                name = f"OCT_LEFT_{sequence_number}" if side == "L" else f"OCT_RIGHT_{int(sequence_number)}"

            if image_type == "OP":
                name = f"EYE_LEFT_{sequence_number}" if side == "L" else f"EYE_RIGHT_{int(sequence_number)}"

            self.measures.append(
                OCTMeasure(
                    name=name,
                    image_type=image_type,
                    create_time=create_time,
                    sequence_number=sequence_number,
                    file_path=file_path,
                )
            )
            self.add_file(file_path=file_path, send_name=name)

        return True, None

    @override
    def reset(self):
        super().reset()
        self.measures = []

    @override
    def to_response(self):
        res = super().to_response()

        results = sorted(self.measures, key=lambda measure: measure.sequence_number)

        res["value"]["results"] = [measure.to_dict() for measure in results]

        return res
