import requests
import logging

def test_audiometer(payload):
  try:
      response = requests.post("http://127.0.0.1:9000/audiometer", json=payload)
      print(response.status_code)
      print(response.text)

      data = response.json()
      session_id = data['sessionId']

      print("Press Enter to continue...")
      while input() != "":
        pass

      response = requests.delete(f"http://127.0.0.1:9000/audiometer/${session_id}")
      print(response.status_code)
      print(response.text)

  except requests.exceptions.RequestException as e:
      logging.error(f"Exception: {e}")


if __name__ == '__main__':
  test_audiometer({ "answer_id": "10199", "barcode": "string", "language": "en", "interviewer": "string" })