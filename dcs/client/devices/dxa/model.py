from copy import deepcopy

from model import Model

from utils import DicomFileInfo


class DXAModel(Model):
    def __init__(self, session, config):
        super().__init__(session, config)

    def set_apex_db_paths(self, patscan_path, reference_path):
        #self.apex = ApexDatabaseReader(patscan_path, reference_path)
        pass

    def get_patient_info(self):
        #self.patient_info = self.apex.get_patient_info(self.session.barcode)
        #return deepcopy(self.patient_info)
        pass

    def get_scans(self, patient_data):
        #self.patient_scans = self.apex.get_scan_analysis(patient_data)
        #return deepcopy(self.patient_scans)
        pass

    def update_files(self, files: list[DicomFileInfo]):
        #self.files = deepcopy(files)
        pass
