import requests
import logging

def test_choice_reaction(payload):
  try:
      response = requests.post("http://127.0.0.1:9000/choice_reaction", json=payload)
      print(response.status_code)
      print(response.text)

      data = response.json()
      session_id = data['sessionId']

      print("Press Enter to continue...")
      while input() != "":
        pass

      response = requests.delete(f"http://127.0.0.1:9000/choice_reaction/${session_id}")
      print(response.status_code)
      print(response.text)

  except requests.exceptions.RequestException as e:
      logging.error(f"Exception: {e}")
  

if __name__ == '__main__':
  test_choice_reaction({ "answer_id": "10199", "barcode": "string", "language": "en", "interviewer": "string" })