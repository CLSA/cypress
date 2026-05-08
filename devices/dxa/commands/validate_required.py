import csv
import json

from pathlib import Path

required = {}

if __name__ == '__main__':
    with open(Path('C:/Users/hoarea/data/dexa_reexport/dalhousie/files/scans_required.csv'), mode='r') as scans_required_csv:
        reader = csv.reader(scans_required_csv)
        for row in reader:
            uid = row[3]
            file_name = row[4]

            required.setdefault(uid, []).append(file_name)

        print(json.dumps(required, indent=2))

    exported_renamed = Path("C:/Users/hoarea/data/dexa_reexport/dalhousie/files/output")

    exported_uids = []

    for uid_path in exported_renamed.iterdir():
        exported_uids.append(uid_path.name)


    not_found = set([])
    for uid in required.keys():
        if uid not in exported_uids:
            not_found.add(int(uid))


    for required_uid in required.keys():
        if required_uid not in not_found:
            print("processing", required_uid)

            files_dir = Path(exported_renamed / required_uid)

            if not files_dir.exists():
                not_found.add(int(required_uid))
                continue

            files_needed = required[required_uid]

            print(files_needed)
            for file_found in files_dir.iterdir():
                print(file_found.name)
                pass



    #print(json.dumps(sorted(list(not_found)), indent=4))