import unittest
import json

from pathlib import Path

from PySide6.QtCore import QCoreApplication

from devices.dxa.apex.patscan_db import PatScanDB
from devices.dxa.apex.reference_db import ReferenceDB

from devices.dxa.measurements.ap_lumbar_spine import APLumbarSpine, APLumbarSpineTest
from devices.dxa.measurements.forearm import LeftForearm, RightForearm, ForearmTest
from devices.dxa.measurements.hip import LeftHip, RightHip, HipTest
from devices.dxa.measurements.whole_body import WholeBody


class TestReferenceDB(unittest.TestCase):
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

    def test_hip_select_curve(self):
        print("\n====== LEFT HIP ======")
        for name, bone_range in HipTest.ranges.items():
            print(name, bone_range)
            success, result = self.reference_db.select_curve(
                method="NULL",
                ref_type=LeftHip.get_ref_type(),
                ref_source=LeftHip.get_ref_source(),
                bone_range=bone_range,
            )

            self.assertTrue(success)

            print(result)

        print("\n====== RIGHT HIP ======")
        for name, bone_range in HipTest.ranges.items():
            print(name, bone_range)
            success, result = self.reference_db.select_curve(
                method="NULL",
                ref_type=RightHip.get_ref_type(),
                ref_source=RightHip.get_ref_source(),
                bone_range=bone_range,
            )

            self.assertTrue(success)

            print(result)

    def test_forearm_select_curve(self):
        print("\n====== LEFT FOREARM ======")
        for name, bone_range in ForearmTest.ranges.items():
            print(name, bone_range)
            success, result = self.reference_db.select_curve(
                method="NULL",
                ref_type=LeftForearm.get_ref_type(),
                ref_source=LeftForearm.get_ref_source(),
                bone_range=bone_range,
            )

            self.assertTrue(success)

            print(result)

        print("\n====== RIGHT FOREARM ======")
        for name, bone_range in ForearmTest.ranges.items():
            print(name, bone_range)
            success, result = self.reference_db.select_curve(
                method="NULL",
                ref_type=LeftForearm.get_ref_type(),
                ref_source=LeftForearm.get_ref_source(),
                bone_range=bone_range,
            )

            self.assertTrue(success)

            print(result)

    def test_spine_select_curve(self):
        print("\n====== AP SPINE ======")
        for name, bone_range in APLumbarSpineTest.ranges.items():
            print(name, bone_range)

            success, result = self.reference_db.select_curve(
                method="APEX" if ("L1_" in name or "L4_" in name) else "NULL",
                ref_type=APLumbarSpine.get_ref_type(),
                ref_source=APLumbarSpine.get_ref_source(),
                bone_range=bone_range,
            )

            self.assertTrue(success)

            print(result)

    def test_select_points(self):
        pass
