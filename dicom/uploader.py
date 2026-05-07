import time
import os
import requests

from pathlib import Path

from PySide6.QtCore import QThread, Signal
from PySide6.QtWidgets import QVBoxLayout, QProgressBar, QLabel, QDialog


class FileUploaderThread(QThread):
    # Sends: (current_file_index, current_file_name, percentage)
    progress_updated = Signal(int, str, int)
    all_finished = Signal(bool)

    def __init__(self, file_paths: list[Path], url: str):
        super().__init__()
        self.file_paths = file_paths
        self.url = url

    def run(self):
        try:
            for index, file_path in enumerate(self.file_paths):
                file_name = os.path.basename(file_path)
                file_size = os.path.getsize(file_path)
                bytes_sent = 0

                def file_generator():
                    nonlocal bytes_sent
                    with open(file_path, "rb") as f:
                        while True:
                            chunk = f.read(16384)
                            if not chunk:
                                break

                            time.sleep(0.01)
                            yield chunk
                            bytes_sent += len(chunk)

                            # Emit: which file, its name, and its progress
                            percent = int((bytes_sent / file_size) * 100)
                            self.progress_updated.emit(index + 1, file_name, percent)

                response = requests.post(
                    self.url, data=file_generator()
                )

                if response.status_code != 200:
                    self.all_finished.emit(False)

            self.all_finished.emit(True)
        except Exception as e:
            print(f"Error: {e}")
            self.all_finished.emit(False)


class FileUploaderDialog(QDialog):
    upload_successful = Signal()
    upload_failed = Signal()

    def __init__(self):
        super().__init__()
        self.setWindowTitle("Uploading files..")
        self.setMinimumWidth(400)

        layout = QVBoxLayout()
        self.status_label = QLabel("Waiting..")

        # Individual file progress
        self.progress_bar = QProgressBar()

        layout.addWidget(self.status_label)
        layout.addWidget(self.progress_bar)

        self.setLayout(layout)

    def start_batch_upload(self, files: list[Path], url: str):
        self.total_files = len(files)

        self.thread = FileUploaderThread(files, url)
        self.thread.progress_updated.connect(self.update_ui)
        self.thread.all_finished.connect(self.on_finished)
        self.thread.start()

    def update_ui(self, file_num, name, percent):
        self.status_label.setText(f"Uploading {name} ({file_num} / {self.total_files})")
        self.progress_bar.setValue(percent)

    def on_finished(self, success):
        self.status_label.setText("Upload complete!" if success else "Upload failed.")
        self.progress_bar.setValue(100 if success else 0)

        if success:
            self.upload_successful.emit()
        else:
            self.upload_failed.emit()