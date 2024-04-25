import requests
import logging


def test_participant_report(payload):
  try:
      print(payload)
      response = requests.post("http://127.0.0.1:9001/general_proxy_consent/", json=payload)

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
     "interviewer": "hoarea",
  }

  test_participant_report({ "answer_id": 10199, "barcode": "123456", "language": "en", "interviewer": "hoarea" })