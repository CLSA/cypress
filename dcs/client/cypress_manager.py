import sys
import requests
import urllib3

from typing import override

from PySide6.QtWidgets import QApplication, QMainWindow, QSystemTrayIcon, QMenu
from PySide6.QtGui import QIcon, QAction
from PySide6.QtCore import QTimer

from ui.ui_main_window import Ui_MainWindow

from config import CypressConfig

cypress_config, errors = CypressConfig.from_ini()
if errors:
    print(errors)
    sys.exit(-1)

host = cypress_config.host
port = cypress_config.port

urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)
#urllib3.disable_warnings(urllib3.exceptions.)

class CypressAPI:
    @staticmethod
    def get_status():
        response = requests.get(f"https://{host}:{port}/status", verify=False, timeout=1)
        response.raise_for_status()
        return response.json()

    @staticmethod
    def restart():
        response = requests.post(f"https://{host}:{port}/restart", verify=False, timeout=1)
        response.raise_for_status()
        return response.text.strip('"')

    @staticmethod
    def launch_device_detached(device: str):
        response = requests.post(f"https://{host}:{port}/detached/{device}", verify=False, timeout=1)
        response.raise_for_status()
        return response.text.strip('"')

class CypressManagerWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

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

        self.ui.restartCypress.clicked.connect(self.restart_cypress)

        self.launch_device_btns = [
            ("hearcon", self.ui.launchAudiometer),
            ("cdtt", self.ui.launchCDTT),
            ("choice_reaction_test", self.ui.launchCRT),
            ("frax", self.ui.launchFRAX),
            ("dxa", self.ui.launchDEXA),
            ("mac5", self.ui.launchECG),
            ("hand_grip", self.ui.launchGripStrength),
            ("tonometer", self.ui.launchTonometer),
            ("spirometer", self.ui.launchSpirometer),
            ("weigh_scale", self.ui.launchWeighScale),
            ("echo", self.ui.launchECHO),
            ("general_proxy", self.ui.launchGeneralProxy)
        ]

        for device_name, btn in self.launch_device_btns:
            btn.clicked.connect(lambda checked, name=device_name: self.launch_device(name))

        self.get_status()

        self.status_timer = QTimer(self)
        self.status_timer.timeout.connect(self.get_status)
        #self.status_timer.start() # ms

    @override
    def show(self):
        self.get_status()
        super().show()

    def tray_activated(self, reason):
        if QSystemTrayIcon.ActivationReason.DoubleClick == reason:
            self.show()
        elif QSystemTrayIcon.ActivationReason.Trigger == reason:
            self.show()

    def launch_device(self, device_name):
        self.enable_device_buttons(False)
        QApplication.processEvents()

        try:
            CypressAPI.launch_device_detached(device_name)
        except requests.HTTPError as e:
            print(e)
        except (requests.ConnectionError, requests.Timeout):
            self._set_connection_error()
        except Exception as e:
            print(e)
        finally:
            self.enable_device_buttons(True)


    def restart_cypress(self):
        try:
            CypressAPI.restart()
        except requests.HTTPError as e:
            print(e)
        except (requests.ConnectionError, requests.Timeout):
            self._set_connection_error()
        except Exception as e:
            print(e)

    def get_status(self):
        try:
            self.status = CypressAPI.get_status()
            version = self.status.get("version", "Unknown")

            if "session" in self.status:
                self.ui.statusValue.setText(f"device in progress")
                self.enable_device_buttons(False)
            else:
                self.ui.statusValue.setText("running")
                self.enable_device_buttons(True)

            self.ui.versionValue.setText(version)
            self.ui.hostValue.setText(f"{cypress_config.host}")
            self.ui.lastUpdatedValue.setText(f"{self.status["last_updated"]}")
            #self.ui.statusValue.setText(json.dumps(self.status, indent=4))
        except requests.HTTPError as e:
            print(e)
        except (requests.ConnectionError, requests.Timeout):
            self._set_connection_error()
        except Exception as e:
            print(e)

    def enable_device_buttons(self, enabled: bool):
        for device_name, btn in self.launch_device_btns:
            btn.setEnabled(enabled if device_name in self.status["installed"] else False)

    def closeEvent(self, event):
        event.accept()  # Closes the window

    def _set_connection_error(self):
        self.ui.statusValue.setText("Could not connect to Cypress")
        self.ui.versionValue.setText("---")
        self.ui.hostValue.setText("---")
        self.ui.lastUpdatedValue.setText("---")

if __name__ == '__main__':
    app = QApplication()
    app.setQuitOnLastWindowClosed(False)

    main_window = CypressManagerWindow()
    main_window.show()

    sys.exit(app.exec())


