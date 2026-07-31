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


class TestSpine(unittest.TestCase):
    app = QCoreApplication()

    @classmethod
    def setUpClass(cls):
        cls.patscan_db = PatScanDB(Path(__file__).parent / "fixtures/PatScan.mdb")
        if not cls.patscan_db.open():
            raise Exception("Cannot open PatScan.mdb")

        cls.reference_db = ReferenceDB(Path(__file__).parent / "fixtures/reference.mdb")
        if not cls.reference_db.open():
            raise Exception("Cannot open reference.mdb")

        success, patient_info = cls.patscan_db.get_patient_info(barcode="20052908")
        if not success:
            raise Exception("Couldn't get patient info")

        success, scan_analysis = cls.patscan_db.get_scan_analysis(
            patient_key=patient_info.get("PATIENT_KEY"),
            scan_type=APLumbarSpine.get_scan_type(),
        )
        if not success:
            raise Exception("Couldn't get scan analysis")

        success, spine_data = cls.patscan_db.get_scan_data(
            table="Spine",
            patient_key=patient_info.get("PATIENT_KEY"),
            scan_id=scan_analysis[0].get("SCANID"),
        )
        if not success:
            raise Exception("Couldn't get spine data")

        print(json.dumps(spine_data, indent=4))

        cls.spine = APLumbarSpine(raw_data={**spine_data})

        return super().setUpClass()

    @classmethod
    def tearDownClass(cls):
        cls.patscan_db.close()
        cls.reference_db.close()

        return super().tearDownClass()


    def test_get_bone_range_key(self):
        self.assertEqual(APLumbarSpine.get_bone_range_key(l1=True, l2=False, l3=False, l4=False), "tot_l1_bmd")
        self.assertEqual(APLumbarSpine.get_bone_range_key(l1=False, l2=True, l3=False, l4=False), "tot_l2_bmd")
        self.assertEqual(APLumbarSpine.get_bone_range_key(l1=False, l2=False, l3=True, l4=False), "tot_l3_bmd")
        self.assertEqual(APLumbarSpine.get_bone_range_key(l1=False, l2=False, l3=False, l4=True), "tot_l4_bmd")
        self.assertEqual(APLumbarSpine.get_bone_range_key(l1=True, l2=True, l3=False, l4=False), "tot_l1l2_bmd")
        self.assertEqual(APLumbarSpine.get_bone_range_key(l1=True, l2=False, l3=True, l4=False), "tot_l1l3_bmd")
        self.assertEqual(APLumbarSpine.get_bone_range_key(l1=True, l2=False, l3=False, l4=True), "tot_l1l4_bmd")
        self.assertEqual(APLumbarSpine.get_bone_range_key(l1=False, l2=True, l3=True, l4=False), "tot_l2l3_bmd")
        self.assertEqual(APLumbarSpine.get_bone_range_key(l1=False, l2=True, l3=False, l4=True), "tot_l2l4_bmd")
        self.assertEqual(APLumbarSpine.get_bone_range_key(l1=False, l2=False, l3=True, l4=True), "tot_l3l4_bmd")
        self.assertEqual(APLumbarSpine.get_bone_range_key(l1=True, l2=True, l3=True, l4=False), "tot_l1l2l3_bmd")
        self.assertEqual(APLumbarSpine.get_bone_range_key(l1=True, l2=True, l3=False, l4=True), "tot_l1l2l4_bmd")
        self.assertEqual(APLumbarSpine.get_bone_range_key(l1=True, l2=False, l3=True, l4=True), "tot_l1l3l4_bmd")
        self.assertEqual(APLumbarSpine.get_bone_range_key(l1=False, l2=True, l3=True, l4=True), "tot_l2l3l4_bmd")
        self.assertEqual(APLumbarSpine.get_bone_range_key(l1=True, l2=True, l3=True, l4=True), "tot_bmd")

    def test_get_bmd_data(self):
        print(self.spine.to_dict())
        print(self.spine.get_bmd_data())