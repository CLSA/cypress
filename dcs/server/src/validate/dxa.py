import sys
import argparse

from pathlib import Path

def check_dxa_files(root_dir):
    if not root_dir.exists() or not root_dir.is_dir():
        print("No root dir found")
        sys.exit(-1)

    for p in root_dir.iterdir():
        if p.exists() and p.is_dir():
            uid = p.name

            wb_file_paths = list(p.glob("WB_*.dcm"))
            sel_file_paths = list(p.glob("SEL_*.dcm"))

            ok = True

            if len(sel_file_paths) > 0 and len(sel_file_paths) != 3:
                print(f"{uid}: missing sel file")
                ok = False

            if len(wb_file_paths) == 1:
                print(f"{uid}: missing wb file")
                ok = False

            if ok:
                print(f"{uid}: OK")

if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        prog="validate dxa",
        description=(
            "check that all dxa report files are present"
        )
    )

    parser.add_argument("dxa_dir", type=Path)

    args = parser.parse_args()

    dxa_dir = args.dxa_dir

