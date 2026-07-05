import time
import requests
import logging

import settings

from requests import HTTPError, ConnectionError, Timeout, TooManyRedirects

from PySide6.QtCore import QThread, Signal, QObject
from PySide6.QtWidgets import QVBoxLayout, QProgressBar, QLabel, QDialog

from model import Model
from utils import FileInfo

from session import Session


class PineAPI(QObject):
    all_finished = Signal(bool)
    progress_updated = Signal(
        int, str, int
    )  # current_file_index, current_file_name, percentage

    def __init__(self, base_url, auth_token, logger_name):
        super().__init__()

        self.base_url = base_url
        self.logger = logging.getLogger(logger_name)
        self.logger.debug(f"PineAPI - base_url {base_url}")

        self.auth_token = auth_token
        self.chunk_size = settings.TRANSFER_CHUNK_SIZE
        self.sleep_time = settings.TRANSFER_SLEEP_TIME

    def send_device_response(
        self, session_id: str, answer_id: str, json: dict, files: list[FileInfo]
    ):
        self.logger.info(f"sending cypress response")

        if not self._send_json(url=f"{self.base_url}/answer/{answer_id}", json=json):
            self.logger.error("failed to send json response")
            return self.all_finished.emit(False)

        if not self._send_files(url=f"{self.base_url}/answer/{answer_id}", files=files):
            self.logger.error("failed to send files")
            return self.all_finished.emit(False)

        if not self._send_complete(session_id=session_id):
            self.logger.error("failed to send complete status")
            return self.all_finished.emit(False)

        return self.all_finished.emit(True)

    def send_cancel(self, session_id: str) -> bool:
        self.logger.debug(f"sending cancel status: {session_id}")

        return self._send_json(
            url=f"{self.base_url}/answer_device/uuid={session_id}",
            json={"status": "cancelled"},
        )

    def _send_complete(self, session_id: str) -> bool:
        self.logger.debug(f"sending complete status: {session_id}")

        return self._send_json(
            url=f"{self.base_url}/answer_device/uuid={session_id}",
            json={"status": "completed"},
        )

    def _send_json(self, url: str, json: dict) -> bool:
        try:
            response = requests.patch(
                url=url,
                json=json,
                headers={"Authorization": self.auth_token},
            )
            response.raise_for_status()
        except HTTPError as e:
            self.logger.error(e)
            return False
        except ConnectionError as e:
            self.logger.error(e)
            return False
        except Timeout as e:
            self.logger.error(e)
            return False
        except TooManyRedirects as e:
            self.logger.error(e)
            return False
        except Exception as e:
            self.logger.error(e)
            return False
        return True

    def _send_files(self, url: str, files: list[FileInfo]):
        try:
            for index, file_info in enumerate(files):
                file_name = file_info.send_name
                file_size = file_info.raw_size
                bytes_sent = 0

                def file_generator():
                    nonlocal bytes_sent
                    with open(file_info.file_path, "rb") as f:
                        while True:
                            chunk = f.read(self.chunk_size)
                            if not chunk:
                                break

                            time.sleep(self.sleep_time)
                            yield chunk

                            bytes_sent += len(chunk)
                            percent = int((bytes_sent / (max(file_size, 1))) * 100)
                            self.progress_updated.emit(index + 1, file_name, percent)

                self.logger.info(f"sending {file_name}.{file_info.extension} ({index + 1} / {len(files)})")
                response = requests.patch(
                    f"{url}?filename={file_name}.{file_info.extension}",
                    data=file_generator(),
                    headers={
                        "Authorization": self.auth_token,
                        "Content-Type": "application/octet-stream",
                    },
                )
                response.raise_for_status()
        except HTTPError as e:
            self.logger.error(e)
            return False
        except ConnectionError as e:
            self.logger.error(e)
            return False
        except Timeout as e:
            self.logger.error(e)
            return False
        except TooManyRedirects as e:
            self.logger.error(e)
            return False
        except Exception as e:
            self.logger.error(e)
            return False
        return True


class DataUploader(QThread):
    def __init__(self, session: Session, model: Model, api: PineAPI):
        super().__init__()
        self.session = session
        self.model = model
        self.api = api

    def run(self):
        self.api.send_device_response(
            session_id=self.session.session_id,
            answer_id=self.session.answer_id,
            json=self.model.to_response(),
            files=self.model.files,
        )


class DataUploaderDialog(QDialog):
    upload_successful = Signal()
    upload_failed = Signal()

    def __init__(self, session: Session, model: Model, device_name: str, parent=None):
        super().__init__(parent=parent)

        self.session = session
        self.model = model

        self.setWindowTitle("Uploading to Pine..")
        self.setMinimumWidth(400)

        layout = QVBoxLayout()
        self.status_label = QLabel("Waiting..")
        self.progress_bar = QProgressBar()

        layout.addWidget(self.status_label)
        layout.addWidget(self.progress_bar)

        self.setLayout(layout)

        self.thread = DataUploader(
            session=session,
            model=model,
            api=PineAPI(
                base_url=session.origin,
                auth_token=settings.PINE_AUTH_TOKEN,
                logger_name=device_name,
            ),
        )
        self.thread.api.progress_updated.connect(self.update_ui)
        self.thread.api.all_finished.connect(self.on_finished)

    def reset(self):
        self.status_label.setText("Waiting..")
        self.progress_bar.setValue(0)

    def start_upload(self):
        self.thread.start()

    def update_ui(self, file_num, name, percent):
        self.status_label.setText(
            f"Uploading {name} ({file_num} / {len(self.model.files)})"
        )
        self.progress_bar.setValue(percent)

    def on_finished(self, success):
        self.status_label.setText("Upload complete!" if success else "Upload failed.")
        self.progress_bar.setValue(100 if success else 0)

        if success:
            self.upload_successful.emit()
        else:
            self.upload_failed.emit()
