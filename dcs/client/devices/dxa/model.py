from model import Model

from utils import DicomFileInfo

from devices.dxa.utils.apex import ApexDatabaseReader

from copy import deepcopy


class DXAModel(Model):
    def __init__(self, session, config):
        super().__init__(session, config)

    def set_apex_db_paths(self, patscan_path, reference_path):
        self.apex = ApexDatabaseReader(patscan_path, reference_path)

    def get_patient_info(self):
        self.patient_info = self.apex.get_patient_info(self.session.barcode)
        return deepcopy(self.patient_info)

    def get_scans(self, patient_data):
        self.patient_scans = self.apex.get_scan_analysis(patient_data)
        return deepcopy(self.patient_scans)

    def update_files(self, files: list[DicomFileInfo]):
        self.files = deepcopy(files)
