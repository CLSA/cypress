import requests
import logging


def test_participant_report(payload):
  try:
      print(payload)
      response = requests.post("http://127.0.0.1:9000/participant_report", json=payload)
      print(response.status_code)
      print(response.text)

      print("press enter to end session")
      while input() != "":
        pass
      
  except requests.exceptions.RequestException as e:
      logging.error(f"Exception: {e}")


if __name__ == '__main__':
  test_data = {
     "answer_id": "10199",
     "barcode": "100125263",
     "language": "en",
     "interviewer": "SIM",

     "name": "John",
     "sex": "M",
     "age": 52,
     "height": 170,
     "weight": 80,
     "bmi": 30,
     "waist_circ": 100, 
     "hip_circ": 100,
     "bp_systolic": 120,
     "bp_diastolic": 80,

     "fvc": 100,
     "fev1": 100,
     "frax": 100,

     "hearing_left_500": 100,
     "hearing_left_1000": 100,
     "hearing_left_2000": 100,
     "hearing_left_4000": 100,
     "hearing_left_6000": 100,
     "hearing_left_8000": 100,

     "hearing_right_500": 100,
     "hearing_right_1000": 100,
     "hearing_right_2000": 100,
     "hearing_right_4000": 100,
     "hearing_right_6000": 100,
     "hearing_right_8000": 100,
  }

  test_participant_report({ "answer_id": "10199", "barcode": "string", "language": "en", "interviewer": "string" })
  print("press enter to start over")
  while input() != "":
    pass