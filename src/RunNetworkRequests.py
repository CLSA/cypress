from flask import Flask, request

import requests
import time
import json
import logging
import threading
import os

# Set up logging
logging.basicConfig(level=logging.INFO)

app = Flask(__name__)

BASE_URL = 'http://127.0.0.1:9000'
# Replace this with the path to your directory
UPLOAD_DIRECTORY = "./file_uploads"

urls = [
    {
        'url': f'{BASE_URL}/tonometer',
        'payload': {
            'answer_id': 7,
            'barcode': 'string',
            'language': 'en',
            'interviewer': 'string',
        }
    }
]

# Define your list of URLs and their payloads
# Add more URLs and payloads as required
# urls = [
#    {
#        'url': f'{BASE_URL}/audiometer',
#        'payload': {
#            'answer_id': 1,
#            'barcode': 'string',
#            'language': 'en',
#            'interviewer': 'string',
#        }
#    },
#    {
#        'url': f'{BASE_URL}/blood_pressure',
#        'payload': {
#            'answer_id': 2,
#            'barcode': 'string',
#            'language': 'fr',
#            'interviewer': 'string',
#        }
#    },
#    {
#        'url': f'{BASE_URL}/body_composition',
#        'payload': {
#            'answer_id': 3,
#            'barcode': 'string',
#            'language': 'fr',
#            'interviewer': 'string',
#        }
#    },
#    {
#        'url': f'{BASE_URL}/cdtt',
#        'payload': {
#            'answer_id': 4,
#            'barcode': 'string',
#            'language': 'fr',
#            'interviewer': 'string',
#        }
#    },
#    {
#        'url': f'{BASE_URL}/choice_reaction',
#        'payload': {
#            'answer_id': 5,
#            'barcode': 'string',
#            'language': 'fr',
#            'interviewer': 'string',
#        }
#    },
#    {
#        'url': f'{BASE_URL}/dexa',
#        'payload': {
#            'answer_id': 6,
#            'barcode': 'string',
#            'language': 'fr',
#            'interviewer': 'string',
#        }
#    },
#    {
#        'url': f'{BASE_URL}/ecg',
#        'payload': {
#            'answer_id': 7,
#            'barcode': 'string',
#            'language': 'fr',
#            'interviewer': 'string',
#        }
#    },
#    {
#        'url': f'{BASE_URL}/frax',
#        'payload': {
#            'answer_id': 8,
#            'barcode': 'string',
#            'language': 'fr',
#            'interviewer': 'string',
#        }
#    },
#    {
#        'url': f'{BASE_URL}/grip_strength',
#        'payload': {
#            'answer_id': 9,
#            'barcode': 'string',
#            'language': 'fr',
#            'interviewer': 'string',
#        }
#    },
#    {
#        'url': f'{BASE_URL}/hearing',
#        'payload': {
#            'answer_id': 10,
#            'barcode': 'string',
#            'language': 'fr',
#            'interviewer': 'string',
#        }
#    },
#    {
#        'url': f'{BASE_URL}/retinal_camera',
#        'payload': {
#            'answer_id': 11,
#            'barcode': 'string',
#            'language': 'fr',
#            'interviewer': 'string',
#        }
#    },
#    {
#        'url': f'{BASE_URL}/spirometer',
#        'payload': {
#            'answer_id': 12,
#            'barcode': 'string',
#            'language': 'fr',
#            'interviewer': 'string',
#        }
#    },
#    {
#        'url': f'{BASE_URL}/temperature',
#        'payload': {
#            'answer_id': 13,
#            'barcode': 'string',
#            'language': 'fr',
#            'interviewer': 'string',
#        }
#    },
#    {
#        'url': f'{BASE_URL}/tonometer',
#        'payload': {
#            'answer_id': 14,
#            'barcode': 'string',
#            'language': 'fr',
#            'interviewer': 'string',
#        }
#    },
#    {
#        'url': f'{BASE_URL}/weigh_scale',
#        'payload': {
#            'answer_id': 15,
#            'barcode': 'string',
#            'language': 'fr',
#            'interviewer': 'string',
#        }
#    },
# ]

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

        # Add the file path to the job_data
        if job_url not in job_data:
            job_data[job_url] = {'file_paths': []}

        job_data[job_url]['file_paths'].append(file_path)

        return 'Data received and written to disk', 200
    elif request.content_type == 'application/json':
        data = request.json
        job_url = request.base_url

        # Update job status
        job_data[job_url] = {
            'status': 'complete',
            'data': data,
        }

        logging.info(f"Answer for: {job_url} with ID: {answer_id}")
        logging.info(json.dumps(data, indent=4))

        logging.info(f"Job complete for {job_url}")
        return "OK", 200
    else:
        return 'Invalid content type', 400


if __name__ == '__main__':
    # Start the Flask app in a separate thread
    threading.Thread(target=lambda: app.run(port=5000)).start()

    # Wait until the Flask server is running
    while True:
        try:
            response = requests.get('http://127.0.0.1:5000/health')
            if response.status_code == 200:
                break
        except requests.exceptions.ConnectionError:
            pass  # server not yet started
        time.sleep(1)  # wait a bit before trying again

    def call_url(url, payload):
        logging.info(f"Starting job for {url}")

        try:
            response = requests.post(url, json=payload)

            print(response.status_code)
            print(response.text)

            job_data[url] = {
                'status': 'pending',
                'data': None
            }

            while True:
                if job_data.get(url, {}).get('status') == 'complete':
                    break
                time.sleep(1)

        except requests.exceptions.RequestException as e:
            logging.error(f"Request to {url} failed. Exception: {e}")

    # Call the URLs sequentially
    call_url(urls[0]['url'], urls[0]['payload'])
