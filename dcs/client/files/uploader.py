import requests

import settings

from requests import HTTPError, ConnectionError, Timeout, TooManyRedirects

from PySide6.QtCore import Signal, QObject

from utils import FileInfo


class PineAPI(QObject):
    all_finished = Signal(bool)
    progress_updated = Signal(
        int, str, int
    )  # current_file_index, current_file_name, percentage

    def __init__(self, base_url, logger, session=None, json=None, files=None):
        super().__init__()

        self.base_url = base_url
        self.logger = logger
        self.logger.debug(f"PineAPI - base_url {base_url}")

        self.session = session
        self.json = json
        self.files = files

        self.auth_token = settings.PINE_AUTH_TOKEN
        self.chunk_size = settings.TRANSFER_CHUNK_SIZE
        self.sleep_time = settings.TRANSFER_SLEEP_TIME

    def send_device_response(self):
        self.logger.info(f"sending cypress response")

        url = f"{self.base_url}/answer/{self.session.answer_id}"

        if not self._send_json(url=url, json=self.json):
            self.logger.error("failed to send json data")
            return self.all_finished.emit(False)

        if not self._send_files(url=url, files=self.files):
            self.logger.error("failed to send files")
            return self.all_finished.emit(False)

        if not self._send_complete(session_id=self.session.session_id):
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

                            yield chunk

                            bytes_sent += len(chunk)
                            percent = int((bytes_sent / (max(file_size, 1))) * 100)

                            self.progress_updated.emit(index + 1, file_name, percent)

                self.logger.info(f"sending {file_name}{file_info.extension} ({index + 1} / {len(files)})")
                response = requests.patch(
                    f"{url}?filename={file_name}{file_info.extension}",
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