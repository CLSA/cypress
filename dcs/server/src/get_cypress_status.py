import requests
import time
import argparse
import json


def get_cypress_status(machine, port, path):
    try:
        response = requests.get(f"http://{machine}:{port}/{path}")
        if response.status_code == 200:
            status = response.json()
    except TimeoutError:
        return {
            "error": "timeout"
        }
    except Exception as e:
        return {
            "error": str(e)
        }
    else:
        return status


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="get cypress status",
        description="gets the server status from a dcs workstation"
    )

    parser.add_argument("workstation", type=str)
    parser.add_argument("port", type=str)
    parser.add_argument("--repeat", action="store_true")

    args = parser.parse_args()

    if args.repeat:
        while True:
            status = get_cypress_status(args.workstation, args.port, "status")
            print(json.dumps(status))
            time.sleep(1)
    else:
        status = get_cypress_status(args.workstation, args.port, "status")
        print(json.dumps(status))
