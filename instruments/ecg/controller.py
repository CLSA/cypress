from typing import override
from PySide6.QtCore import Signal
from instruments.controller import Controller

from dicom.receiver import FileReceiver, FileInfo, ReceiverConfig

from instruments.ecg.config import ECGConfig
from instruments.ecg.session import ECGSession
from instruments.ecg.model import ECGModel

from dicom.uploader import FileUploaderDialog


class ECGController(Controller):
    files_received = Signal(object)

    def __init__(
        self,
        session: ECGSession,
        config: ECGConfig,
        model: ECGModel,
        standalone=False,
        parent=None,
    ):
        super().__init__(
            parent=parent,
            session=session,
            config=config,
            model=model,
            standalone=standalone,
        )

        self.file_receiver = FileReceiver(
            ReceiverConfig(
                storage_dir=self.config.storage_path,
                extensions=[".pdf"],
            )
        )
        self.file_receiver.files_received.connect(self._on_files_received)
        self.file_receiver.start()

        self.uploader = FileUploaderDialog()
        self.uploader.upload_successful.connect(self._upload_succeeded)
        self.uploader.upload_failed.connect(self._upload_failed)

    def _on_files_received(self, files: list[FileInfo]):
        self.files = [f.file_path for f in files]
        self.files_received.emit(files)

        self.measured.emit(files)

    @override
    def submit(self):
        self.uploader.show()
        self.uploader.start_batch_upload(files=self.files, url='http://127.0.0.1:5000/upload')

    def _upload_succeeded(self):
        print("Upload success")

    def _upload_failed(self):
        print("Upload failed")




