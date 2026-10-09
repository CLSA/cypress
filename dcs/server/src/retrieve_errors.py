import subprocess
import requests

import time


def scp(machine: str, directory: str, destination: str):
    stdout, stderr = subprocess.Popen(
        f"scp -r -T {machine}:'{directory}' {destination}",
        shell=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    ).communicate()

    return stdout, stderr


def command(machine, command: str):
    stdout, stderr = subprocess.Popen(
        f"ssh {machine} '{command}'",
        shell=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    return stdout, stderr


def get_cypress_status(machine, port, path):
    try:
        response = requests.get(f"{machine}:{port}/{path}")
        if response.status_code == 200:
            status = response.json()
    except TimeoutError:
        print("Timeout")
        return None
    else:
        return status


if __name__ == "__main__":
    while True:
        status = get_cypress_status("", "", "")
        print(status)
        time.sleep(1)