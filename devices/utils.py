import psutil

from pathlib import Path

def get_file_size(file_path: Path):
    size_bytes = file_path.stat().st_size

    units = ["B", "KB", "MB", "GB", "TB", "PB", "EB"]
    for unit in units:
        if abs(size_bytes) < 1024.0:
            return f"{size_bytes:.2f}{unit}"
        size_bytes /= 1024.0


def is_process_running(process_name: str):
    for process in psutil.process_iter(["name"]):
        try:
            if process_name.lower() == process.info["name"].lower():
                return True
        except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
            pass
    return False
