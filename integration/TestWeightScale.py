import requests
import logging

def test_weight_scale(payload):
  try:
      response = requests.post("http://127.0.0.1:9000/weight_scale", json=payload)
      print(response.status_code)
      print(response.text)

      data = response.json()
      session_id = data['sessionId']

      print("Press Enter to continue...")
      while input() != "":
        pass

      response = requests.delete(f"http://127.0.0.1:9000/weight_scale/${session_id}")
      print(response.status_code)
      print(response.text)

  except requests.exceptions.RequestException as e:
      logging.error(f"Exception: {e}")



if __name__ == '__main__':
  test_weight_scale({ "answer_id": "10199", "barcode": "string", "language": "en", "interviewer": "string" })