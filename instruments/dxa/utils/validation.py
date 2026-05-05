import pydicom

from pathlib import Path

###
#  These are the key/value pairs that are expected to be found in the DICOM files
#  received from the DXA machines for each scan type
###

# If value is [""], just check that the tag (key) exists in the file but don't care about the value
# If the value is [ ], ensure that the key/value pair does not exist in the file
# If the value contains 1 or more possible values, does an OR check for a match (exists1 OR exists2 OR ...)
dxa_validation_rules = {
    'hip': {
        'MediaStorageSOPClassUID':              { 'equals': pydicom.uid.SecondaryCaptureImageStorage },
        'Modality':                             { 'equals': 'OT'         },
        'Laterality':                           { 'in'    : [ 'L', 'R' ] },
        'BodyPartExamined':                     { 'equals': 'HIP'        },
        'ImageAndFluoroscopyAreaDoseProduct':   { 'exists': True         },
        'PatientOrientation':                   { 'exists': True         },
        'BitsAllocated':                        { 'equals': 8            },
        'PhotometricInterpretation':            { 'equals': 'RGB'        },
        'PixelSpacing':                         { 'exists': True         },
        'SamplesPerPixel':                      { 'equals': 3            }
    },

    'whole_body_1': {
        'MediaStorageSOPClassUID':              { 'equals': pydicom.uid.SecondaryCaptureImageStorage },
        'Modality':                             { 'equals': 'OT'  },
        'Laterality':                           { 'exists': False },
        'BodyPartExamined':                     { 'exists': True, 'blank': True  },
        'ImageAndFluoroscopyAreaDoseProduct':   { 'exists': True  },
        'PatientOrientation':                   { 'exists': True  },
        'BitsAllocated':                        { 'equals': 8     },
        'PhotometricInterpretation':            { 'equals': 'RGB' },
        'PixelSpacing':                         { 'exists': True  },
        'SamplesPerPixel':                      { 'equals': 3     }
    },

    'whole_body_2': {
        'MediaStorageSOPClassUID':              { 'equals': pydicom.uid.SecondaryCaptureImageStorage },
        'Modality':                             { 'equals': 'OT'  },
        'Laterality':                           { 'exists': False },
        'BodyPartExamined':                     { 'exists': True, 'blank': True  },
        'ImageAndFluoroscopyAreaDoseProduct':   { 'exists': False },
        'PatientOrientation':                   { 'exists': True  },
        'BitsAllocated':                        { 'equals': 8     },
        'PhotometricInterpretation':            { 'equals': 'RGB' },
        'PixelSpacing':                         { 'exists': True  },
        'SamplesPerPixel':                      { 'equals': 3     }
    },

    'forearm': {
        'MediaStorageSOPClassUID':              { 'equals': pydicom.uid.SecondaryCaptureImageStorage },
        'Modality':                             { 'equals': 'OT'         },
        'Laterality':                           { 'in'    : [ 'L', 'R' ] },
        'BodyPartExamined':                     { 'equals': 'ARM'        },
        'ImageAndFluoroscopyAreaDoseProduct':   { 'exists': True         },
        'PatientOrientation':                   { 'exists': True         },
        'BitsAllocated':                        { 'equals': 8            },
        'PhotometricInterpretation':            { 'equals': 'RGB'        },
        'PixelSpacing':                         { 'exists': True         },
        'SamplesPerPixel':                      { 'equals': 3            }
    },

    'ap_spine': {
        'MediaStorageSOPClassUID':              { 'equals': pydicom.uid.SecondaryCaptureImageStorage },
        'Modality':                             { 'equals': 'OT'     },
        'BodyPartExamined':                     { 'equals': 'LSPINE' },
        'ImageAndFluoroscopyAreaDoseProduct':   { 'exists': True     },
        'PatientOrientation':                   { 'exists': True     },
        'BitsAllocated':                        { 'equals': 8        },
        'PhotometricInterpretation':            { 'equals': 'RGB'    },
        'PixelSpacing':                         { 'exists': True     },
        'SamplesPerPixel':                      { 'equals': 3        }
    },

    'iva_measure': {
        'MediaStorageSOPClassUID':              { 'equals': pydicom.uid.SecondaryCaptureImageStorage },
        'Modality':                             { 'equals': 'OT'          },
        'Laterality':                           { 'exists': False         },
        'BodyPartExamined':                     { 'equals': 'LSPINE'      },
        'ImageAndFluoroscopyAreaDoseProduct':   { 'exists': True          },
        'PatientOrientation':                   { 'exists': True          },
        'BitsAllocated':                        { 'equals': 16            },
        'PhotometricInterpretation':            { 'equals': 'MONOCHROME2' },
        'PixelSpacing':                         { 'exists': True          },
        'SamplesPerPixel':                      { 'equals': 1             }
    },

    'iva_pr': {
        'MediaStorageSOPClassUID':              { 'equals': pydicom.uid.GrayscaleSoftcopyPresentationStateStorage },
        'Modality':                             { 'equals': 'PR' }
    },

    'iva_ot': {
        'MediaStorageSOPClassUID':              { 'equals': pydicom.uid.SecondaryCaptureImageStorage },
        'Modality':                             { 'equals': 'OT'          },
        'BitsAllocated':                        { 'equals':  8            },
        'PhotometricInterpretation':            { 'equals': 'MONOCHROME2' },
        'SamplesPerPixel':                      { 'equals': 1             }
    },
}

def is_valid(ds: pydicom.FileDataset, validation_rules: dict):
    for tag, rules in validation_rules.items():
        if tag == "MediaStorageSOPClassUID" and ds.file_meta is not None:
            if rules['equals'] != ds.file_meta[tag].value:
                return False
            continue

        if 'exists' in rules:
            if rules['exists'] and tag not in ds:
                return False

            if rules['exists'] and 'blank' in rules and ds[tag].value != '':
                return False

            if not rules['exists'] and tag in ds:
                return False

        elif 'equals' in rules:
            if tag not in ds:
                return False
            if ds[tag].value != rules['equals']:
                return False

        elif 'in' in rules:
            if tag not in ds:
                return False

            value_list = rules['in']
            if not isinstance(value_list, list):
                raise Exception("'in' rule requires a list")

            ok = False
            for val in value_list:
                if ds[tag].value == val:
                    ok = True

            if not ok:
                return False
        else:
            raise Exception("error: rule not found")

    return True

def print_file_meta(path):
    dcm = pydicom.dcmread(path)

    print("-" * 80)
    print(path.name)
    print(dcm.PatientID)
    print(dcm.Modality)
    print(dcm.file_meta)
    print("-" * 80, "\n")

def get_file_type(file_path: Path) -> str:
    ds = pydicom.dcmread(file_path)
    for file_type, validation_rules in dxa_validation_rules.items():
        if is_valid(ds=ds, validation_rules=validation_rules):
            return file_type