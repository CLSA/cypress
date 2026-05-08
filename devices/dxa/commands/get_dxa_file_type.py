import argparse
import sys

from utils.validation import get_file_type

from pathlib import Path


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Determines the type of the DXA dicom file')
    parser.add_argument('file', help='The .dcm file to identify')

    args = parser.parse_args()

    file_path = Path(args.file)
    if not file_path.is_file():
        print("error: file_path is not a file")
        sys.exit()

    if file_path.suffix != '.dcm':
        print("error: file does not have .dcm extension")
        sys.exit()

    file_type = get_file_type(file_path)

    print(f'{file_type}')