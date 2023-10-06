import requests
import logging

def test_frax(payload):
  try:
      response = requests.post("http://127.0.0.1:9000/frax", json=payload)
      print(response.status_code)
      print(response.text)

      print("Press Enter to continue...")
      while input() != "":
        pass

  except requests.exceptions.RequestException as e:
      logging.error(f"Exception: {e}")


if __name__ == '__main__':
  input_data = {
     "type": "t",
     "country_code": "ca",
     "age": 82,
     "sex": "F",
     "bmi": 30,
     "previous_fracture": "n",
     "parent_hip_fracture": "n",
     "current_smoker": "n",
     "glucocorticoid": "n",
     "rheumatoid_arthritis": "n",
     "secondary_osteoporosis": "n",
     "alcohol": "n",
     "t_score": -2.5,
     "z_score": -2.5,
  }

  test_frax({ "answer_id": "10199", "barcode": "string", "language": "en", "interviewer": "string" })