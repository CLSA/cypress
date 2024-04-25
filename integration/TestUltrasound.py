import requests

def test_ultrasound(payload):
  try:
      response = requests.post("http://127.0.0.1:9001/carotid_intima", json=payload, timeout=5)
      print(response.status_code)
      print(response.text)
  except requests.exceptions.RequestException as e:
    print(e)



if __name__ == '__main__':
  test_ultrasound({ "barcode": "100125263", "answer_id": 10199, "language": "en", "interviewer": "SIM", "origin": "127.0.0.1" })