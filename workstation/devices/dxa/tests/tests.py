import pydicom
import sys
import re

from pathlib import Path

from devices.dxa.utils.validation import (
    HipDicom,
    LeftHipDicom,
    RightHipDicom,
    WholeBody1Dicom,
    WholeBody2Dicom,
    ApSpineDicom,
    IvaMeasureDicom,
    IvaOTDicom,
    IvaPRDicom,
    ForearmDicom,
    LeftForearmDicom,
    RightForearmDicom,
    valid,
)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        sys.exit(-1)

    base_dir = Path(sys.argv[1])

    for path in base_dir.iterdir():

        if path.is_dir() and re.match(r"[A-Z]\d{6}", path.name):
            uid = path.name

            for file_path in path.iterdir():
                if not file_path.is_file():
                    continue

                name = file_path.name

                if ".dcm" not in file_path.suffix:
                    continue

                ds = pydicom.dcmread(file_path, stop_before_pixels=True)

                if name == "L_HIP_DICOM.dcm":
                    assert valid(ds, LeftHipDicom), file_path

                    assert not valid(ds, RightHipDicom), file_path
                    assert not valid(ds, ForearmDicom), file_path
                    assert not valid(ds, IvaOTDicom), file_path
                    assert not valid(ds, IvaPRDicom), file_path
                    assert not valid(ds, IvaMeasureDicom), file_path
                    assert not valid(ds, WholeBody1Dicom), file_path
                    assert not valid(ds, WholeBody2Dicom), file_path
                    assert not valid(ds, ApSpineDicom), file_path

                elif name == "R_HIP_DICOM.dcm":
                    assert valid(ds, RightHipDicom), file_path

                    assert not valid(ds, LeftHipDicom), file_path
                    assert not valid(ds, ForearmDicom), file_path
                    assert not valid(ds, IvaOTDicom), file_path
                    assert not valid(ds, IvaPRDicom), file_path
                    assert not valid(ds, IvaMeasureDicom), file_path
                    assert not valid(ds, WholeBody1Dicom), file_path
                    assert not valid(ds, WholeBody2Dicom), file_path
                    assert not valid(ds, ApSpineDicom), file_path

                elif name == "WB_DICOM_1.dcm":
                    assert valid(ds, WholeBody1Dicom), file_path

                    assert not valid(ds, ForearmDicom), file_path
                    assert not valid(ds, IvaOTDicom), file_path
                    assert not valid(ds, IvaPRDicom), file_path
                    assert not valid(ds, IvaMeasureDicom), file_path
                    assert not valid(ds, WholeBody2Dicom), file_path
                    assert not valid(ds, ApSpineDicom), file_path
                    assert not valid(ds, HipDicom), file_path

                elif name == "WB_DICOM_2.dcm":
                    assert valid(ds, WholeBody2Dicom), file_path

                    assert not valid(ds, ForearmDicom), file_path
                    assert not valid(ds, IvaOTDicom), file_path
                    assert not valid(ds, IvaPRDicom), file_path
                    assert not valid(ds, IvaMeasureDicom), file_path
                    assert not valid(ds, WholeBody1Dicom), file_path
                    assert not valid(ds, ApSpineDicom), file_path
                    assert not valid(ds, HipDicom), file_path

                elif name == "SP_DICOM_1.dcm":
                    assert valid(ds, ApSpineDicom), file_path

                    assert not valid(ds, ForearmDicom), file_path
                    assert not valid(ds, IvaOTDicom), file_path
                    assert not valid(ds, IvaPRDicom), file_path
                    assert not valid(ds, IvaMeasureDicom), file_path
                    assert not valid(ds, WholeBody1Dicom), file_path
                    assert not valid(ds, WholeBody2Dicom), file_path
                    assert not valid(ds, HipDicom), file_path

                elif name == "SEL_DICOM_MEASURE.dcm":
                    assert valid(ds, IvaMeasureDicom), file_path

                    assert not valid(ds, ForearmDicom), file_path
                    assert not valid(ds, IvaOTDicom), file_path
                    assert not valid(ds, IvaPRDicom), file_path
                    assert not valid(ds, WholeBody1Dicom), file_path
                    assert not valid(ds, WholeBody2Dicom), file_path
                    assert not valid(ds, ApSpineDicom), file_path
                    assert not valid(ds, HipDicom), file_path

                elif name == "SEL_DICOM_OT.dcm":
                    assert valid(ds, IvaOTDicom), file_path

                    assert not valid(ds, ForearmDicom), file_path
                    assert not valid(ds, IvaPRDicom), file_path
                    assert not valid(ds, IvaMeasureDicom), file_path
                    assert not valid(ds, WholeBody1Dicom), file_path
                    assert not valid(ds, WholeBody2Dicom), file_path
                    assert not valid(ds, ApSpineDicom), file_path
                    assert not valid(ds, HipDicom), file_path

                elif name == "SEL_DICOM_PR.dcm":
                    assert valid(ds, IvaPRDicom), file_path

                    assert not valid(ds, ForearmDicom), file_path
                    assert not valid(ds, IvaOTDicom), file_path
                    assert not valid(ds, IvaMeasureDicom), file_path
                    assert not valid(ds, WholeBody1Dicom), file_path
                    assert not valid(ds, WholeBody2Dicom), file_path
                    assert not valid(ds, ApSpineDicom), file_path
                    assert not valid(ds, HipDicom), file_path

                elif name == "FA_L_DICOM.dcm":
                    assert valid(ds, LeftForearmDicom), file_path

                    assert not valid(ds, RightForearmDicom), file_path
                    assert not valid(ds, IvaOTDicom), file_path
                    assert not valid(ds, IvaPRDicom), file_path
                    assert not valid(ds, IvaMeasureDicom), file_path
                    assert not valid(ds, WholeBody1Dicom), file_path
                    assert not valid(ds, WholeBody2Dicom), file_path
                    assert not valid(ds, ApSpineDicom), file_path
                    assert not valid(ds, HipDicom), file_path

                elif name == "FA_R_DICOM.dcm":
                    assert valid(ds, RightForearmDicom), file_path

                    assert not valid(ds, LeftForearmDicom), file_path
                    assert not valid(ds, IvaOTDicom), file_path
                    assert not valid(ds, IvaPRDicom), file_path
                    assert not valid(ds, IvaMeasureDicom), file_path
                    assert not valid(ds, WholeBody1Dicom), file_path
                    assert not valid(ds, WholeBody2Dicom), file_path
                    assert not valid(ds, ApSpineDicom), file_path
                    assert not valid(ds, HipDicom), file_path

            print(uid, "passed")

    print("All tests passed")
