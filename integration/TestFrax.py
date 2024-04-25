import requests
import logging

def test_frax(payload):
  try:
      response = requests.post("http://127.0.0.1:9001/frax", json=payload)
      print(response.status_code)
      print(response.text)
      print("Press Enter to continue...")

      while input() != "":
        pass

  except requests.exceptions.RequestException as e:
      logging.error(f"Exception: {e}")


if __name__ == '__main__':
  input_data = {
     "answer_id":             10199, 
     "barcode":               "1314300", 
     "language":              "en", 
     "interviewer":           "string",

     "age":                   82,
     "femoral_neck_bmd":      0.4,
     "weight":                90.0,
     "height":                170.0,
     "glucocorticoid_age":    10.0,
     "glucocorticoid_number": 10.0,
     "glucocorticoid_year":   10.0,
     "previous_fracture":     False,
     "ra_medications":        "",
     "alcohol":               False,
     "current_smoker":        False,
     "father_hip_fracture":   False,
     "mother_hip_fracture":   False,
     "sex":                   "m",
  }

  test_frax(input_data)