import pydicom
import json
import argparse
import sys
import csv

from pathlib import Path

from ..utils.validation import is_valid, dxa_validation_rules, get_file_type

class DXAExportReader:
    input_dir: Path = None
    recursive: bool = False

    status: dict = {}
    scans: dict = {}
    patients: dict = {}

    def __init__(self, input_dir: Path, recursive=False):
        self.input_dir = input_dir
        self.recursive = recursive

    def read_patients(self):
        if not self.input_dir.exists():
            raise Exception("error: input_dir does not exist")

        if not self.input_dir.is_dir():
            raise Exception("error: input_dir is not a directory")

        self._read_patient_files(self.input_dir)

    def read_status_csv(self, status_csv_path: str):
        with open(status_csv_path, mode='r', newline='', encoding='utf-8') as f:
            csv_reader = csv.reader(f)

            for row in csv_reader:
                if len(row) < 2:
                    continue
                patient_id = row[0].strip()
                status = row[1].strip().lower()

                if "done" in status:
                    self.status[patient_id] = 'Complete'
                else:
                    self.status[patient_id] = 'Error'

    def read_scans_csv(self, scans_csv_path: str):
        with open(scans_csv_path, mode='r', newline='', encoding='utf-8') as f:
            csv_reader = csv.reader(f)

            for row in csv_reader:
                self.scans[row[0]] = {
                    'wbody_scanid': row[1],
                    'spine_scanid': row[2],
                    'hip_scanid': row[3],
                    'forearm_scanid': row[4],
                    'wbody_comp_scanid': row[5],
                    'hip_hsa_scanid': row[6]
                }

    """
    Copy directories in input_path to the output folder

    """
    def copy_and_rename(self, input_path, output_path: Path) -> None:
        for path in input_path.iterdir():
            if path.is_dir():
                patient_ids = self._get_patient_ids_in_dir(path)
                # if all the files in the folder belong to one patient_id
                if len(set(patient_ids)) == 1:
                    patient_id = patient_ids[0]
                    print(patient_id)

                    files_renamed = {}

                    new_path = output_path / patient_id
                    new_path.mkdir(parents=True, exist_ok=True)

                    for file_path in path.iterdir():
                        if file_path.is_file() and file_path.suffix == '.dcm':
                            file_type = get_file_type(file_path)
                            if file_type and file_type not in files_renamed:
                                copy_path = file_path.copy_into(new_path)
                                files_renamed[file_type] = copy_path.resolve()
                                try:
                                    copy_path.rename(copy_path.parent / f"{file_type}.dcm")
                                except FileExistsError:
                                    copy_path.unlink()
                else:
                    print("error: ", str(path.resolve()), " has mixed ids..")


    def _get_patient_ids_in_dir(self, dir: Path) -> list[str]:
        patient_ids = []
        for path in dir.iterdir():
            if path.suffix == '.dcm':
                ds = pydicom.dcmread(path)
                patient_ids.append(ds.PatientID)
        return patient_ids

    def _read_patient_files(self, input_path: Path) -> None:
        for path in input_path.iterdir():
            if path.is_dir() and self.recursive:
                self._read_patient_files(path)
                continue

            if not path.is_file():
                continue

            if path.suffix != ".dcm":
                continue

            ds = pydicom.dcmread(path)
            for file_type, validation_rules in dxa_validation_rules.items():
                if is_valid(ds, validation_rules):
                    self._update_patient_files(file_type, path, ds)

    def _update_patient_files(self, file_type: str, file_path: Path, ds: pydicom.FileDataset) -> None:
        if ds.PatientID in self.patients:
            files = self.patients[ds.PatientID]
            if file_path in files:
                print("error: file_path already in patient files", self.patients[ds.PatientID])
            files[file_type] = str(file_path.resolve())
        else:
            self.patients[ds.PatientID] = { file_type: str(file_path.resolve()) }


    def __str__(self):
        return json.dumps(patient_reader.scans, indent=4)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Hologic Apex utility program')
    parser.add_argument('input_dir', help='The input directory containing the .dcm files')
    parser.add_argument('output_dir', help='The output directory to put the organized files')

    parser.add_argument('-sf', '--status_file', type=str, default=None, help='A csv of patient_id / status messages that shows export status')
    parser.add_argument('-se', '--scans-export', type=str, default=None, help='A csv of scan keys for each patient id')

    args = parser.parse_args()

    input_path = Path(args.input_dir)
    output_path = Path(args.output_dir)

    if not input_path.exists():
        print("error: input path does not exist")
        sys.exit()

    if not input_path.is_dir():
        print("error: input path is not a directory")
        sys.exit()

    if output_path.exists():
        print("error: output path already exists")
        sys.exit()

    output_path.mkdir(parents=True, exist_ok=False)

    patient_reader = DXAExportReader(input_path, recursive=True)
    try:
        patient_reader.copy_and_rename(input_path=input_path, output_path=output_path)
    except Exception as e:
        print(f"error: {e}")

    print(patient_reader)