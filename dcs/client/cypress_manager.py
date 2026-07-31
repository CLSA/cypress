import sys
import os
import subprocess
import datetime
import win32api

from typing import override

from PySide6.QtWidgets import QApplication, QMainWindow, QSystemTrayIcon, QMenu
from PySide6.QtGui import QIcon, QAction
from PySide6.QtCore import QTimer

from ui.ui_main_window import Ui_MainWindow

from config import CypressConfig

from utils import stop_process, is_process_running

cypress_config, errors = CypressConfig.from_ini()
if errors:
    print(errors)
    sys.exit(-1)

host = cypress_config.host
port = cypress_config.port

class CypressAPI:
    @staticmethod
    def start():
        if is_process_running(cypress_config.name):
            return True

        if not cypress_config.exe.exists():
            return False

        if not cypress_config.exe.is_file():
            return False

        subprocess.Popen([str(cypress_config.exe.resolve())])

        return True

    @staticmethod
    def stop():
        return stop_process(cypress_config.name)

    @staticmethod
    def last_modified():
        exe_path = cypress_config.exe
        mod_time = os.stat(exe_path).st_mtime
        last_modified = datetime.datetime.fromtimestamp(mod_time)
        return last_modified.strftime("%Y-%m-%d %H:%M:%S")

    @staticmethod
    def get_version():
        # https://stackoverflow.com/questions/68774795/grabbing-full-file-version-of-an-exe-in-python
        try:
            info = win32api.GetFileVersionInfo(str(cypress_config.exe.resolve()), "\\")
        except:
            return

        ms = info["FileVersionMS"]
        ls = info["FileVersionLS"]

        version = [
            str(win32api.HIWORD(ms)),
            str(win32api.LOWORD(ms)),
            str(win32api.HIWORD(ls)),
            str(win32api.LOWORD(ls)),
        ]

        return ".".join(version)


class CypressManagerWindow(QMainWindow):
    def __init__(self):
        super().__init__()

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
        quit_action.triggered.connect(QApplication.instance().quit)

        tray_menu.addAction(quit_action)
        tray_menu.addAction(show_action)

        self.tray_icon.setContextMenu(tray_menu)
        self.tray_icon.show()

        self.ui.stopCypress.clicked.connect(self.stop_cypress)
        self.ui.startCypress.clicked.connect(self.start_cypress)

        self.get_status()

        self.status_timer = QTimer(self)
        self.status_timer.timeout.connect(self.get_status)
        self.status_timer.start(1000)  # ms

    @override
    def show(self):
        self.get_status()
        super().show()

    def tray_activated(self, reason):
        if QSystemTrayIcon.ActivationReason.DoubleClick == reason:
            self.show()
        elif QSystemTrayIcon.ActivationReason.Trigger == reason:
            self.show()

    def start_cypress(self):
        self.ui.startCypress.setEnabled(False)
        QApplication.processEvents()
        CypressAPI.start()

    def stop_cypress(self):
        CypressAPI.stop()

    def get_status(self):
        version = CypressAPI.get_version()
        last_modified = CypressAPI.last_modified()
        host = cypress_config.host

        self.ui.versionValue.setText(version if version else "---")
        self.ui.lastUpdatedValue.setText(last_modified)
        self.ui.hostValue.setText(host)

        if is_process_running(cypress_config.name):
            self.ui.statusValue.setText("running")
            self.ui.startCypress.setEnabled(False)
            self.ui.stopCypress.setEnabled(True)
        else:
            self.ui.statusValue.setText("not running")
            self.ui.startCypress.setEnabled(True)
            self.ui.stopCypress.setEnabled(False)

    def closeEvent(self, event):
        event.accept()  # Closes the window


if __name__ == "__main__":
    app = QApplication()
    app.setQuitOnLastWindowClosed(False)

    main_window = CypressManagerWindow()
    main_window.hide()

    sys.exit(app.exec())
