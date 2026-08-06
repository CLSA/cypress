import time

from PySide6.QtCore import QObject, QThread, Signal, Slot
from PySide6.QtWidgets import QWidget

from pathlib import Path

from ui.ui_log import Ui_LogWidget


class LogMonitor(QObject):
    line_read = Signal(str)
    finished = Signal()

    def __init__(self, log_file: Path):
        super().__init__()
        self.log_file = log_file
        self._running = True

    @Slot()
    def run(self):
        with self.log_file.open("r", encoding="utf-8", errors="replace") as f:
            f.seek(0, 2)

            while self._running:
                line = f.readline()

                if line:
                    self.line_read.emit(line.rstrip("\n"))
                else:
                    time.sleep(0.1)

    def stop(self):
        self._running = False

class LogWidget(QWidget, Ui_LogWidget):
    def __init__(self, parent):
        super().__init__(parent)

        self.setupUi(self)

        self.log.document().setMaximumBlockCount(5000)

        self.thread = QThread()

        self.log_monitor = LogMonitor(Path.cwd() / "logs" / "server.log")
        self.log_monitor.moveToThread(self.thread)

        self.thread.started.connect(self.log_monitor.run)
        self.log_monitor.line_read.connect(self.line_read)

        self.log_monitor.finished.connect(self.thread.quit)
        self.log_monitor.finished.connect(self.log_monitor.deleteLater)
        self.thread.finished.connect(self.thread.deleteLater)

        self.thread.start()

    def line_read(self, line: str):
        self.log.appendPlainText(line)

        scrollbar = self.log.verticalScrollBar()
        scrollbar.setValue(scrollbar.maximum())

    def closeEvent(self, event):
        self.log_monitor.stop()
        self.thread.quit()
        self.thread.wait()
        super().closeEvent(event)


