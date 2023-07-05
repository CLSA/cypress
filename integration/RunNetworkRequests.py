from flask import Flask, request

import requests
import time
import json
import logging
import threading
import os

from TestAudiometer import test_audiometer
from TestBodyComposition import test_body_composition
from TestBPM import test_bpm 
from TestCDTT import test_cdtt
from TestChoiceReaction import test_choice_reaction
from TestDxa import test_dxa
from TestEcg import test_ecg
from TestFrax import test_frax
from TestGripStrength import test_grip_strength
from TestRetinalCamera import test_retinal_camera
from TestTemperature import test_temperature
from TestTonometer import test_tonometer
from TestWeightScale import test_weight_scale

# Set up logging
logging.basicConfig(level=logging.INFO)

app = Flask(__name__)

BASE_URL = 'http://127.0.0.1:9000'
# Replace this with the path to your directory
UPLOAD_DIRECTORY = "./file_uploads"


# Store job completion status and response data in a dictionary
job_data = {}


@app.route('/health')
def health_check():
    return 'OK', 200


@app.route('/api/answer/<answer_id>', methods=['POST'])
def receive_data(answer_id):
    # Check if the request's content type is 'application/octet-stream'
    if request.content_type == 'application/octet-stream':
        data = request.data
        job_url = request.base_url

        # Get filename from query parameters. Default to a name based on answer_id and timestamp if not provided
        filename = request.args.get(
            'filename', f'{answer_id}_{int(time.time())}.bin')

        # Write the data to a file in the upload directory
        file_path = os.path.join(UPLOAD_DIRECTORY, filename)
        with open(file_path, 'wb') as file:
            file.write(data)

        return 'Data received and written to disk', 200

    elif request.content_type == 'application/json':
        data = request.json
        job_url = request.base_url

        logging.info(f"Answer for: {job_url} with ID: {answer_id}")
        logging.info(json.dumps(data, indent=4))
        logging.info(f"Job complete for {job_url}")

        return "OK", 200
    else:
        return 'Invalid content type', 400


def wait_for_server_up():
  # Wait until the Flask server is running
  while True:
      try:
          response = requests.get('http://127.0.0.1:5000/health')
          if response.status_code == 200:
              break
      except requests.exceptions.ConnectionError:
          pass  # server not yet started
      time.sleep(1)  # wait a bit before trying again

if __name__ == '__main__':
    # Start the Flask app in a separate thread
    threading.Thread(target=lambda: app.run(port=5000)).start()

    wait_for_server_up()  

    test_audiometer({
        "answer_id": "10199",
        "barcode": "string",
        "language": "en",
        "interviewer": "string"
    })

    test_cdtt({
        "answer_id": "10199",
        "barcode": "string",
        "language": "en",
        "interviewer": "string"
    })
