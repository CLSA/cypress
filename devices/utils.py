import psutil

from pathlib import Path
from dataclasses import dataclass

@dataclass(kw_only=True)
class FileInfo:
    name: str
    raw_size: int
    readable_size: str
    file_name: str
    file_path: Path
    extension: str

    # pine preferred name (no extension)
    send_name: str | None

@dataclass(kw_only=True)
class DicomFileInfo(FileInfo):
    patient_id: str
    body_part_examined: str
    modality: str
    laterality: str
    study_id: str
    media_storage_uid: str
    study_date: str
    series_number: str
    instance_number: str


def get_file_size(file_path: Path):
    size_bytes = file_path.stat().st_size

    units = ["B", "KB", "MB", "GB", "TB", "PB", "EB"]
    for unit in units:
        if abs(size_bytes) < 1024.0:
            return f"{size_bytes:.2f}{unit}"
        size_bytes /= 1024.0


def get_file_info(file_path: Path):
    if not file_path:
        raise ValueError("file_path parameter is missing")

    if not file_path.exists():
        raise Exception(f"{file_path.name} does not exist")

    if not file_path.is_file():
        raise Exception(f"{file_path.name} is not a file")

    return FileInfo(
        name=file_path.stem,
        raw_size=file_path.stat().st_size,
        readable_size=get_file_size(file_path),
        file_name=file_path.name,
        extension=file_path.suffix[1:], # ignore period
        file_path=file_path,
        send_name=None
    )


def is_process_running(process_name: str):
    for process in psutil.process_iter(["name"]):
        try:
            if process_name.lower() == process.info["name"].lower():
                return True
        except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
            pass
    return False