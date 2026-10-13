import argparse

from src.utils import copy_to_machine

from pathlib import Path

if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        prog="copy to machine",
        description="pushes <src> to <machine>:<destination>"
    )

    parser.add_argument("src", type=Path)
    parser.add_argument("machine", type=str)
    parser.add_argument("destination", type=str)

    args = parser.parse_args()

    stdout, stderr = copy_to_machine(args.machine, args.cypress_path, args.destination)

    print(stdout)
    print(stderr)


