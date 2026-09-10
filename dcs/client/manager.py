import sys
import os
import subprocess
import datetime
import win32api
import requests
import json
import time
import traceback

from typing import override
from pathlib import Path

from PySide6.QtCore import Qt, QObject, QTimer, QRunnable, QThreadPool, Signal, Slot, QEvent
from PySide6.QtWidgets import QApplication, QMainWindow, QSystemTrayIcon, QMenu, QTabWidget
from PySide6.QtGui import QIcon, QAction

from ui.ui_main_window import Ui_MainWindow

from log import LogWidget

from config import config

class CypressAPI:
    @staticmethod
    def start():
        try:
            subprocess.Popen(
                [str(config.exe.resolve())],
                cwd=str(config.exe.parent.resolve()),
                creationflags=subprocess.DETACHED_PROCESS
                | subprocess.CREATE_NEW_PROCESS_GROUP,
                close_fds=True,
            )
            return True
        except Exception as e:
            return False

    @staticmethod
    def get_status() -> dict | None:
        response = requests.get(f"http://localhost:{config.port}/status", timeout=0.10)
        response.raise_for_status()
        return response.json()

    @staticmethod
    def health():
        response = requests.get(f"http://localhost:{config.port}/health", timeout=0.10)
        response.raise_for_status()
        return response.json()

    @staticmethod
    def stop():
        response = requests.post(f"http://localhost:{config.port}/stop", timeout=0.10)
        response.raise_for_status()
        return response.json()

class WorkerSignals(QObject):
    finished = Signal(dict)
    error = Signal(str)


class HttpWorker(QRunnable):
    def __init__(self, api_call):
        super().__init__()
        self.api_call = api_call
        self.signals = WorkerSignals()

    @Slot()
    def run(self):
        try:
            response = self.api_call()
            self.signals.finished.emit(response)
        except Exception as e:
            self.signals.error.emit(str(e))


class CypressManagerWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.thread_pool = QThreadPool.globalInstance()

        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        self.setWindowTitle("Cypress manager")
        self.setWindowIcon(QIcon("favicon.ico"))

        self.tray_icon = QSystemTrayIcon(self)
        self.tray_icon.setIcon(QIcon("favicon.ico"))
        self.tray_icon.setVisible(True)

        self.tray_icon.activated.connect(self.tray_activated)

        tray_menu = QMenu()
        show_action = QAction("Show", self)
        show_action.triggered.connect(self.show)

        quit_action = QAction("Exit", self)
        quit_action.triggered.connect(self.exit)

        tray_menu.addAction(quit_action)
        tray_menu.addAction(show_action)

        self.tray_icon.setContextMenu(tray_menu)
        self.tray_icon.show()

        self.ui.stopCypress.clicked.connect(self.stop_cypress)
        self.ui.startCypress.clicked.connect(self.start_cypress)

        self.log_widget = LogWidget(self, config.exe.parent / "logs" / "server.log")

        tabs = self.findChild(QTabWidget, "tabWidget")

        current_tab = tabs.currentWidget()
        current_tab.layout().insertWidget(4, self.log_widget)
        #current_tab.layout().setStretch(4, 1)

        self.health_check_timer = QTimer(self)
        self.health_check_timer.timeout.connect(self.health_check)
        self.health_check_timer.setInterval(1000)
        self.health_check_timer.start()

        self.stopping = False
        self.starting = False

        self.online = False


    @override
    def show(self):
        self.setWindowState(self.windowState() & ~Qt.WindowState.WindowMinimized | Qt.WindowState.WindowActive)
        self.raise_()
        self.activateWindow()

        self.get_status()

        super().show()

    def health_check(self):
        worker = HttpWorker(CypressAPI.get_status)
        worker.signals.finished.connect(self.set_online)
        worker.signals.error.connect(self.set_offline)
        self.thread_pool.start(worker)

    def set_online(self, status):
        if self.stopping:
            return

        self.starting = False
        self.online = True
        self.update_status(status)

    def set_offline(self):
        if self.starting:
            return

        self.stopping = False
        self.online = False
        self.update_status(None)

    def update_status(self, status: dict | None):
        self.ui.statusValue.setText("Online" if self.online else "Offline")
        self.ui.startCypress.setEnabled(not self.online)
        self.ui.stopCypress.setEnabled(self.online)

        self.health_check_running = False

        self.set_status(status)

    def get_status(self):
        worker = HttpWorker(CypressAPI.get_status)
        worker.signals.finished.connect(self.set_status)
        worker.signals.error.connect(self.status_error)
        self.thread_pool.start(worker)

    def tray_activated(self, reason):
        if QSystemTrayIcon.ActivationReason.DoubleClick == reason:
            self.show()
        elif QSystemTrayIcon.ActivationReason.Trigger == reason:
            self.show()

    @override
    def changeEvent(self, event):
        # if event.type() == QEvent.WindowStateChange:
        #     self.get_status()
        return super().changeEvent(event)

    def start_cypress(self):
        self.ui.startCypress.setEnabled(False)
        self.starting = True
        self.stopping = False

        QApplication.processEvents()
        CypressAPI.start()

    def stop_cypress(self):
        self.ui.stopCypress.setEnabled(False)
        self.stopping = True
        self.starting = False

        worker = HttpWorker(CypressAPI.stop)
        worker.signals.finished.connect(self.stop_status)
        self.thread_pool.start(worker)

    def set_status(self, status):
        self.ui.versionValue.setText(status.get("version", "---") if status else "---")
        self.ui.lastUpdatedValue.setText(status.get("last_updated", "---") if status else "---")
        self.ui.hostValue.setText("---")

    def stop_status(self, response):
        status = response.get("status")

        if status == "stopping":
            print("stopping")

        elif status == "in_progress":
            print("session in progress")

        else:
            print("unknown status")

    def status_error(self, error):
        self.ui.statusValue.setText("Offline")
        self.ui.stopCypress.setEnabled(False)

    def closeEvent(self, event):
        event.accept()

    def exit(self):
        self.log_widget.close()
        QApplication.instance().quit()

if __name__ == "__main__":
    app = QApplication()
    app.setQuitOnLastWindowClosed(False)

    main_window = CypressManagerWindow()
    main_window.hide()

    sys.exit(app.exec())
