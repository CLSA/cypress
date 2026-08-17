import pydicom

from typing import Literal, List, Any
from enum import Enum
from pathlib import Path

from pydantic import BaseModel, Field, ValidationError


class Side(Enum):
    LEFT = 0
    RIGHT = 1
    BOTH = 2


###
#  These are the key/value pairs that are expected to be found in the DICOM files metadata
#  received from the DXA machines for each scan type
###
class HipDicom(BaseModel):
    MediaStorageSOPClassUID: Literal["1.2.840.10008.5.1.4.1.1.7"]

    Modality: Literal["OT"]
    BodyPartExamined: Literal["HIP"]
    BitsAllocated: Literal[8]
    PhotometricInterpretation: Literal["RGB"]
    SamplesPerPixel: Literal[3]

    Laterality: Literal["L", "R"]

    ImageAndFluoroscopyAreaDoseProduct: Any
    PatientOrientation: Any
    PixelSpacing: Any


class LeftHipDicom(HipDicom):
    Laterality: Literal["L"]


class RightHipDicom(HipDicom):
    Laterality: Literal["R"]


class WholeBody1Dicom(BaseModel):
    MediaStorageSOPClassUID: Literal["1.2.840.10008.5.1.4.1.1.7"]
    Modality: Literal["OT"]
    Laterality: None
    BodyPartExamined: str = Field(min_length=0, max_length=0)
    ImageAndFluoroscopyAreaDoseProduct: float
    PatientOrientation: List[str]
    BitsAllocated: Literal[8]
    PhotometricInterpretation: Literal["RGB"]
    PixelSpacing: None
    SamplesPerPixel: Literal[3]


class WholeBody2Dicom(BaseModel):
    MediaStorageSOPClassUID: Literal["1.2.840.10008.5.1.4.1.1.7"]
    Modality: Literal["OT"]
    Laterality: None
    BodyPartExamined: str = Field(min_length=0, max_length=0)
    ImageAndFluoroscopyAreaDoseProduct: None
    PatientOrientation: List[str]
    BitsAllocated: Literal[8]
    PhotometricInterpretation: Literal["RGB"]
    PixelSpacing: None
    SamplesPerPixel: Literal[3]


class ForearmDicom(BaseModel):
    MediaStorageSOPClassUID: Literal["1.2.840.10008.5.1.4.1.1.7"]
    Modality: Literal["OT"]
    Laterality: Literal["L", "R"]
    BodyPartExamined: Literal["ARM"]

    ImageAndFluoroscopyAreaDoseProduct: float
    PatientOrientation: List[str]
    BitsAllocated: Literal[8]
    PhotometricInterpretation: Literal["RGB"]
    PixelSpacing: Any
    SamplesPerPixel: Literal[3]


class LeftForearmDicom(ForearmDicom):
    Laterality: Literal["L"]


class RightForearmDicom(ForearmDicom):
    Laterality: Literal["R"]


class ApSpineDicom(BaseModel):
    MediaStorageSOPClassUID: Literal["1.2.840.10008.5.1.4.1.1.7"]
    Modality: Literal["OT"]
    BodyPartExamined: Literal["LSPINE"]
    ImageAndFluoroscopyAreaDoseProduct: float
    PatientOrientation: List[str]
    BitsAllocated: Literal[8]
    PhotometricInterpretation: Literal["RGB"]
    PixelSpacing: None
    SamplesPerPixel: Literal[3]


class IvaMeasureDicom(BaseModel):
    MediaStorageSOPClassUID: Literal["1.2.840.10008.5.1.4.1.1.7"]
    Modality: Literal["OT"]
    BodyPartExamined: Literal["LSPINE"]
    ImageAndFluoroscopyAreaDoseProduct: float
    PatientOrientation: List[str]
    BitsAllocated: Literal[16]
    PhotometricInterpretation: Literal["MONOCHROME2"]
    PixelSpacing: List[float]
    SamplesPerPixel: Literal[1]


class IvaPRDicom(BaseModel):
    MediaStorageSOPClassUID: Literal["1.2.840.10008.5.1.4.1.1.11.1"]
    Modality: Literal["PR"]
    Laterality: None


class IvaOTDicom(BaseModel):
    MediaStorageSOPClassUID: Literal["1.2.840.10008.5.1.4.1.1.7"]
    Modality: Literal["OT"]
    Laterality: None
    BitsAllocated: Literal[8]
    PhotometricInterpretation: Literal["MONOCHROME2"]
    SamplesPerPixel: Literal[1]


class StructuredReportDicom(BaseModel):
    MediaStorageSOPClassUID: Literal["1.2.840.10008.5.1.4.1.1.88.22"]
    Modality: Literal["SR"]


dxa_validation_rules = {
    "L_HIP_DICOM": LeftHipDicom,
    "R_HIP_DICOM": RightHipDicom,
    "WB_DICOM_1": WholeBody1Dicom,
    "WB_DICOM_2": WholeBody2Dicom,
    "FA_L_DICOM": LeftForearmDicom,
    "FA_R_DICOM": RightForearmDicom,
    "SP_DICOM_1": ApSpineDicom,
    "SEL_DICOM_MEASURE": IvaMeasureDicom,
    "SEL_DICOM_OT": IvaOTDicom,
    "SEL_DICOM_PR": IvaPRDicom,
    "SR_DICOM": StructuredReportDicom,
}


def get_dicom_dict(ds):
    return {
        "MediaStorageSOPClassUID": getattr(
            ds.file_meta, "MediaStorageSOPClassUID", None
        ),
        "PatientID": getattr(ds, "PatientID", None),
        "Modality": getattr(ds, "Modality", None),
        "Laterality": getattr(ds, "Laterality", None),
        "BodyPartExamined": getattr(ds, "BodyPartExamined", None),
        "ImageAndFluoroscopyAreaDoseProduct": getattr(
            ds, "ImageAndFluoroscopyAreaDoseProduct", None
        ),
        "PatientOrientation": getattr(ds, "PatientOrientation", None),
        "BitsAllocated": getattr(ds, "BitsAllocated", None),
        "PhotometricInterpretation": getattr(ds, "PhotometricInterpretation", None),
        "PixelSpacing": getattr(ds, "PixelSpacing", None),
        "SamplesPerPixel": getattr(ds, "SamplesPerPixel", None),
    }


def valid(ds, model: type[BaseModel]):
    try:
        model(**get_dicom_dict(ds))
        return True
    except ValidationError as e:
        return False


def print_file_meta(path):
    dcm = pydicom.dcmread(path)

    print("-" * 80)
    print(path.name)
    print(dcm.PatientID)
    print(dcm.Modality)
    print(dcm.file_meta)
    print("-" * 80, "\n")


def get_file_type(ds) -> str | None:
    for file_name, pydantic_model in dxa_validation_rules.items():
        if valid(ds=ds, model=pydantic_model):
            return file_name


def get_scan_type(path: Path) -> str:
    ds = pydicom.dcmread(path)
    return get_file_type(ds)
