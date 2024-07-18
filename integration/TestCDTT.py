import requests
import logging
import time


def test_cdtt(payload):
  try:
      response = requests.post("http://127.0.0.1:9000/cdtt", json=payload, headers={"Origin": "127.0.0.1"})
      print(response.status_code)
      print(response.text)

      data = response.json()
      session_id = data['sessionId']

  except requests.exceptions.RequestException as e:
      logging.error(f"Exception: {e}")


if __name__ == '__main__':
  test_cdtt({ "answer_id": 10199, "barcode": "string", "language": "en", "interviewer": "string", "origin": "127.0.0.1" })