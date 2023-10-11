import requests
import logging

def test_audiometer(payload):
  try:
      response = requests.post("http://127.0.0.1:9000/audiometer", json=payload)
      print(response.status_code)
      print(response.text)

      print("Press Enter to continue...")
      while input() != "":
        pass

  except requests.exceptions.RequestException as e:
      logging.error(f"Exception: {e}")


if __name__ == '__main__':
  test_audiometer({ "answer_id": 10199, "barcode": "string", "language": "en", "interviewer": "string" })