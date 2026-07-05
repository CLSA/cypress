import unittest

from devices.frax.model import FraxModel

from session import FRAXSession

class TestFraxSession(unittest.TestCase):
    def setUp(self):
        super().setUp()

        # As sent from Pine
        self.request_data = {
            'barcode': '00000000',
            'uid': '00000000',
            'language': 'en',
            'interviewer': 'test',
            'answer_id': 1,


            'age': 67,
            'sex': 'male',
            'previous_fracture': False,
            'mother_hip_fracture': False,
            'father_hip_fracture': False,
            'current_smoker': False,
            'alcohol': False,
            'height': 160.5,
            'weight': 112.4,
            'ra_medications': 'adf adf adf',
            'femoral_neck_bmd': 0.9,
            'glucocorticoid_number': '0',
            'glucocorticoid_age': '0',
            'glucocorticoid_year': None
        }

    def test_pydantic_model(self):
        self.session_data = FRAXSession(**self.request_data)

    def test_parent_fracture_mother_hip(self):
        self.request_data['mother_hip_fracture'] = True

        self.session_data = FRAXSession(**self.request_data).model_dump()

        self.assertTrue('parent_hip_fracture' in self.session_data)
        self.assertTrue(self.session_data['parent_hip_fracture'])

    def test_parent_fracture_father_hip(self):
        self.request_data['father_hip_fracture'] = True

        self.session_data = FRAXSession(**self.request_data).model_dump()

        self.assertTrue('parent_hip_fracture' in self.session_data)
        self.assertTrue(self.session_data['parent_hip_fracture'])

    def test_parent_fracture_both(self):
        self.request_data['father_hip_fracture'] = True
        self.request_data['mother_hip_fracture'] = True

        self.session_data = FRAXSession(**self.request_data).model_dump()

        self.assertTrue('parent_hip_fracture' in self.session_data)
        self.assertTrue(self.session_data['parent_hip_fracture'])

    def test_parent_fracture_neither(self):
        self.request_data['father_hip_fracture'] = False
        self.request_data['mother_hip_fracture'] = False

        self.session_data = FRAXSession(**self.request_data).model_dump()

        self.assertTrue('parent_hip_fracture' in self.session_data)
        self.assertFalse(self.session_data['parent_hip_fracture'])

    def test_bmi(self):
        self.session_data = FRAXSession(**self.request_data).model_dump()
        self.assertTrue('body_mass_index' in self.session_data)
        self.assertAlmostEqual(self.session_data['body_mass_index'], 43.63, places=2)

    def test_tscore(self):
        self.session_data = FRAXSession(**self.request_data).model_dump()

        self.assertTrue('femoral_neck_tscore' in self.session_data)
        self.assertAlmostEqual(
            self.session_data['femoral_neck_tscore'],
            0.46,
            places=2
        )

    def test_tscore_method(self):
        self.assertAlmostEqual(FraxModel.calculate_t_score(femoral_neck_bmd=0.9), 0.46, places=2)
        self.assertAlmostEqual(FraxModel.calculate_t_score(femoral_neck_bmd=1.1), 2.26, places=2)
        self.assertAlmostEqual(FraxModel.calculate_t_score(femoral_neck_bmd=0.5), -3.14, places=2)
        self.assertAlmostEqual(FraxModel.calculate_t_score(femoral_neck_bmd=2.0), 10.37, places=2)

    def test_glucocorticoid_false(self):
        self.session_data = FRAXSession(**self.request_data).model_dump()
        self.assertIn(self.session_data['glucocorticoid_age'], [0, None])
        self.assertIn(self.session_data['glucocorticoid_number'], [0, None])
        self.assertIn(self.session_data['glucocorticoid_year'], [0, None])
        self.assertEqual(self.session_data['glucocorticoid'], 0)

    def test_glucocorticoid_true(self):
        self.request_data['glucocorticoid_age'] = 50
        self.request_data['glucocorticoid_number'] = 2
        self.request_data['glucocorticoid_year'] = 2

    def test_arthritis_true(self):
        self.session_data = FRAXSession(**self.request_data).model_dump()
        self.assertTrue(self.session_data['rheumatoid_arthritis'])

    def test_arthritis_false(self):
        self.request_data['ra_medications'] = None
        self.session_data = FRAXSession(**self.request_data).model_dump()
        self.assertFalse(self.session_data['rheumatoid_arthritis'])


class TestFraxModel(unittest.TestCase):
    def setUp(self):
        super().setUp()

        # As sent from Pine
        self.request_data = {
            'barcode': '00000000',
            'uid': '00000000',
            'language': 'en',
            'interviewer': 'test',
            'answer_id': 1,


            'age': 67,
            'sex': 'male',
            'previous_fracture': False,
            'mother_hip_fracture': False,
            'father_hip_fracture': False,
            'current_smoker': False,
            'alcohol': False,
            'height': 160.5,
            'weight': 112.4,
            'ra_medications': 'adf adf adf',
            'femoral_neck_bmd': 0.9,
            'glucocorticoid_number': 0,
            'glucocorticoid_age': 0,
            'glucocorticoid_year': 0
        }

        self.session_data = FRAXSession(**self.request_data).model_dump()


