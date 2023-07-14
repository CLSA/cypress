import requests
import logging


def test_participant_report(payload):
  try:
      print(payload)
      response = requests.post("http://127.0.0.1:9000/participant_report", json=payload)
      print(response.status_code)
      print(response.text)

      data = response.json()
      session_id = data['sessionId']

      print("press enter to end session")
      while input() != "":
        pass

      response = requests.delete(f"http://127.0.0.1:9000/participant_report/${session_id}")
      print(response.status_code)
      print(response.text)
  except requests.exceptions.RequestException as e:
      logging.error(f"Exception: {e}")


if __name__ == '__main__':
  while True:
    test_participant_report({ "answer_id": "10199", "barcode": "string", "language": "en", "interviewer": "string" })
    print("press enter to start over")
    while input() != "":
      pass