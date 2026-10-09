import requests
import time
import argparse


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
    parser = argparse.ArgumentParser(
        prog="get cypress status",
        description="gets the server status from a dcs workstation"
    )

    parser.add_argument("workstation", type=str)
    parser.add_argument("port", type=str)

    args = parser.parse_args()

    while True:
        status = get_cypress_status(args.workstation, args.port, "status")
        print(status)
        time.sleep(1)