import unittest
import json

from pathlib import Path

from PySide6.QtCore import QCoreApplication

from devices.dxa.apex.patscan_db import PatScanDB
from devices.dxa.apex.reference_db import ReferenceDB

from devices.dxa.measurements.forearm import LeftForearm, RightForearm
from devices.dxa.measurements.hip import LeftHip, RightHip
from devices.dxa.measurements.ap_lumbar_spine import APLumbarSpine

from devices.dxa.measurements.whole_body import WholeBody


class TestScans(unittest.TestCase):
    app = QCoreApplication()

    @classmethod
    def setUpClass(cls):
        cls.patscan_db = PatScanDB(Path(__file__).parent / "fixtures/PatScan.mdb")
        if not cls.patscan_db.open():
            raise Exception("Cannot open PatScan.mdb")

        cls.reference_db = ReferenceDB(Path(__file__).parent / "fixtures/reference.mdb")
        if not cls.reference_db.open():
            raise Exception("Cannot open reference.mdb")

        return super().setUpClass()

    @classmethod
    def tearDownClass(cls):
        cls.patscan_db.close()
        cls.reference_db.close()

        return super().tearDownClass()

    def test_left_hip(self):
        success, patient_info = self.patscan_db.get_patient_info(barcode="30008187")

        self.assertTrue(success)

        success, scan_analysis = self.patscan_db.get_scan_analysis(
            patient_key=patient_info.get("PATIENT_KEY"),
            scan_type=LeftHip.get_scan_type(),
        )
        self.assertTrue(success)
        self.assertTrue(len(scan_analysis) == 1)

        success, hip_scan_data = self.patscan_db.get_scan_data(
            table="Hip",
            patient_key=patient_info.get("PATIENT_KEY"),
            scan_id=scan_analysis[0].get("SCANID"),
        )
        self.assertTrue(success)

        success, hip_hsa_scan_data = self.patscan_db.get_scan_data(
            table="HipHSA",
            patient_key=patient_info.get("PATIENT_KEY"),
            scan_id=scan_analysis[0].get("SCANID"),
        )
        self.assertTrue(success)
        #print(json.dumps({**hip_scan_data, **hip_hsa_scan_data}, indent=4))

        left_hip = LeftHip(raw_data={**hip_scan_data, **hip_hsa_scan_data})
        print(left_hip.get_bmd_data())
        #print(json.dumps(left_hip.to_dict(), indent=4))

    def test_right_hip(self):
        success, patient_info = self.patscan_db.get_patient_info(barcode="30003304")

        self.assertTrue(success)

        success, scan_analysis = self.patscan_db.get_scan_analysis(
            patient_key=patient_info.get("PATIENT_KEY"),
            scan_type=RightHip.get_scan_type(),
        )
        self.assertTrue(success)
        self.assertTrue(len(scan_analysis) == 1)

        success, hip_scan_data = self.patscan_db.get_scan_data(
            table="Hip",
            patient_key=patient_info.get("PATIENT_KEY"),
            scan_id=scan_analysis[0].get("SCANID"),
        )
        self.assertTrue(success)

        success, hip_hsa_scan_data = self.patscan_db.get_scan_data(
            table="HipHSA",
            patient_key=patient_info.get("PATIENT_KEY"),
            scan_id=scan_analysis[0].get("SCANID"),
        )
        self.assertTrue(success)
        #print(json.dumps({**hip_scan_data, **hip_hsa_scan_data}, indent=4))

        right_hip = RightHip(raw_data={**hip_scan_data, **hip_hsa_scan_data})
        print(right_hip.get_bmd_data())
        #print(json.dumps(left_hip.to_dict(), indent=4))

    def test_whole_body(self):
        success, patient_info = self.patscan_db.get_patient_info(barcode="30008187")
        self.assertTrue(success)

        success, scan_analysis = self.patscan_db.get_scan_analysis(
            patient_key=patient_info.get("PATIENT_KEY"),
            scan_type=WholeBody.get_scan_type(),
        )
        self.assertTrue(success)
        self.assertTrue(len(scan_analysis) == 1)

        success, wbody = self.patscan_db.get_scan_data(
            table="Wbody",
            patient_key=patient_info.get("PATIENT_KEY"),
            scan_id=scan_analysis[0].get("SCANID"),
        )
        self.assertTrue(success)

        success, wbody_composition = self.patscan_db.get_scan_data(
            table="WbodyComposition",
            patient_key=patient_info.get("PATIENT_KEY"),
            scan_id=scan_analysis[0].get("SCANID"),
        )
        self.assertTrue(success)

        success, sub_region_bone = self.patscan_db.get_scan_data(
            table="SubRegionBone",
            patient_key=patient_info.get("PATIENT_KEY"),
            scan_id=scan_analysis[0].get("SCANID"),
        )
        self.assertTrue(success)

        success, sub_region_bone_comp = self.patscan_db.get_scan_data(
            table="SubRegionComposition",
            patient_key=patient_info.get("PATIENT_KEY"),
            scan_id=scan_analysis[0].get("SCANID"),
        )
        self.assertTrue(success)

        success, obesity_indices = self.patscan_db.get_scan_data(
            table="ObesityIndices",
            patient_key=patient_info.get("PATIENT_KEY"),
            scan_id=scan_analysis[0].get("SCANID"),
        )
        self.assertTrue(success)

        success, android_gynoid_comp = self.patscan_db.get_scan_data(
            table="AndroidGynoidComposition",
            patient_key=patient_info.get("PATIENT_KEY"),
            scan_id=scan_analysis[0].get("SCANID"),
        )
        self.assertTrue(success)

        wbody_raw_data = {
            **wbody,
            **wbody_composition,
            **sub_region_bone,
            **sub_region_bone_comp,
            **obesity_indices,
            **android_gynoid_comp,
        }

        wbody_measure = WholeBody(raw_data=wbody_raw_data)
        print(wbody_measure.get_bmd_data())

        #print(json.dumps(wbody_measure.to_dict(), indent=4))

    def test_left_forearm(self):
        success, patient_info = self.patscan_db.get_patient_info(barcode="40015524")
        self.assertTrue(success)

        success, scan_analysis = self.patscan_db.get_scan_analysis(
            patient_key=patient_info.get("PATIENT_KEY"),
            scan_type=LeftForearm.get_scan_type(),
        )
        self.assertTrue(success)
        self.assertTrue(len(scan_analysis) == 1)

        success, forearm_data = self.patscan_db.get_scan_data(
            table="Forearm",
            patient_key=patient_info.get("PATIENT_KEY"),
            scan_id=scan_analysis[0].get("SCANID"),
        )
        self.assertTrue(success)

        left_forearm = LeftForearm(raw_data={**forearm_data})
        print(left_forearm.get_bmd_data())
        #print(json.dumps(left_forearm.to_dict(), indent=4))

    def test_right_forearm(self):
        success, patient_info = self.patscan_db.get_patient_info(barcode="20052908")

        self.assertTrue(success)

        success, scan_analysis = self.patscan_db.get_scan_analysis(
            patient_key=patient_info.get("PATIENT_KEY"),
            scan_type=RightForearm.get_scan_type(),
        )
        self.assertTrue(success)
        self.assertTrue(len(scan_analysis) == 1)

        success, raw_forearm_data = self.patscan_db.get_scan_data(
            table="Forearm",
            patient_key=patient_info.get("PATIENT_KEY"),
            scan_id=scan_analysis[0].get("SCANID"),
        )
        self.assertTrue(success)

        right_forearm = RightForearm(raw_data={**raw_forearm_data})
        print(right_forearm.get_bmd_data())

        print(json.dumps(right_forearm.to_dict(), indent=4))

    def test_ap_spine(self):
        success, patient_info = self.patscan_db.get_patient_info(barcode="20052908")
        self.assertTrue(success)

        success, scan_analysis = self.patscan_db.get_scan_analysis(
            patient_key=patient_info.get("PATIENT_KEY"),
            scan_type=APLumbarSpine.get_scan_type(),
        )
        self.assertTrue(success)
        self.assertTrue(len(scan_analysis) == 1)

        success, spine_data = self.patscan_db.get_scan_data(
            table="Spine",
            patient_key=patient_info.get("PATIENT_KEY"),
            scan_id=scan_analysis[0].get("SCANID"),
        )
        self.assertTrue(success)

        spine = APLumbarSpine(raw_data={**spine_data})

        print(spine.get_bmd_data())

        print(json.dumps(spine.to_dict(), indent=4))
