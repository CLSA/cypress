import sys
import requests
import tempfile
import psutil

from pathlib import Path
from PySide6.QtCore import QCoreApplication, QProcess

WAIT_CLOSE_TIMEOUT_S = 60

CYPRESS_SERVER_URL = "https://192.168.8.235:9000/update"

def wait_for_process_close(pid: int):
    try:
        process = psutil.Process(pid)
        process.wait(timeout=WAIT_CLOSE_TIMEOUT_S)
    except psutil.NoSuchProcess:
        print("no such process")
        pass

def backup_exe(orig_path: Path, target_path: Path):
    if not orig_path.exists():
        return False

    if not orig_path.is_file():
        return False

    orig_path.move(target_path)

    return True


def download_most_recent_version():
    print(tempfile.gettempdir())
    with tempfile.NamedTemporaryFile(delete=False, suffix=".exe") as temp_exe:
        #response = requests.get(CYPRESS_SERVER_URL, stream=True)
        #response.raise_for_status()

        #for chunk in response.iter_content(chunk_size=8192):
        #    temp_exe.write(chunk)
        temp_exe.write(bytes("abcdefg", encoding="utf-8"))
        temp_exe.close()

    return temp_exe

if __name__ == '__main__':
    print("run update")
    app = QCoreApplication(sys.argv)

    if len(sys.argv) < 2:
        print("error: requires parent pid")
        sys.exit(-1)

    pid = int(sys.argv[1])
    print(pid)

    # check and wait that the main program is closed
    wait_for_process_close(pid)

    # move the existing cypress to a backup program
    backup_exe(orig_path=Path("cypress.exe"), target_path=Path("./cypress_backup.exe"))

    # download the most recent version
    temp_exe = download_most_recent_version()

    # move the most recent version to the Cypress folder
    temp_filepath = Path(temp_exe.name)
    temp_filepath.move(Path("./cypress.exe"))

    # start process

    #cypress = QProcess()
    #cypress.setWorkingDirectory()

    # exit
    app.exit(1)
    #sys.exit(app.exec())
