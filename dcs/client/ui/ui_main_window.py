# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'main_window.ui'
##
## Created by: Qt User Interface Compiler version 6.10.2
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QHBoxLayout, QLabel, QMainWindow,
    QMenuBar, QPushButton, QSizePolicy, QSpacerItem,
    QStatusBar, QTabWidget, QVBoxLayout, QWidget)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(436, 544)
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Minimum)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(MainWindow.sizePolicy().hasHeightForWidth())
        MainWindow.setSizePolicy(sizePolicy)
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.verticalLayout_2 = QVBoxLayout(self.centralwidget)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.tabWidget = QTabWidget(self.centralwidget)
        self.tabWidget.setObjectName(u"tabWidget")
        self.tab = QWidget()
        self.tab.setObjectName(u"tab")
        self.verticalLayout_5 = QVBoxLayout(self.tab)
        self.verticalLayout_5.setObjectName(u"verticalLayout_5")
        self.horizontalLayout_2 = QHBoxLayout()
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.statusLabel = QLabel(self.tab)
        self.statusLabel.setObjectName(u"statusLabel")
        sizePolicy1 = QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Fixed)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.statusLabel.sizePolicy().hasHeightForWidth())
        self.statusLabel.setSizePolicy(sizePolicy1)

        self.horizontalLayout_2.addWidget(self.statusLabel)

        self.statusValue = QLabel(self.tab)
        self.statusValue.setObjectName(u"statusValue")
        sizePolicy2 = QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Preferred)
        sizePolicy2.setHorizontalStretch(0)
        sizePolicy2.setVerticalStretch(0)
        sizePolicy2.setHeightForWidth(self.statusValue.sizePolicy().hasHeightForWidth())
        self.statusValue.setSizePolicy(sizePolicy2)

        self.horizontalLayout_2.addWidget(self.statusValue)


        self.verticalLayout_5.addLayout(self.horizontalLayout_2)

        self.horizontalLayout = QHBoxLayout()
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.versionLabel = QLabel(self.tab)
        self.versionLabel.setObjectName(u"versionLabel")
        sizePolicy1.setHeightForWidth(self.versionLabel.sizePolicy().hasHeightForWidth())
        self.versionLabel.setSizePolicy(sizePolicy1)

        self.horizontalLayout.addWidget(self.versionLabel)

        self.versionValue = QLabel(self.tab)
        self.versionValue.setObjectName(u"versionValue")
        sizePolicy2.setHeightForWidth(self.versionValue.sizePolicy().hasHeightForWidth())
        self.versionValue.setSizePolicy(sizePolicy2)

        self.horizontalLayout.addWidget(self.versionValue)


        self.verticalLayout_5.addLayout(self.horizontalLayout)

        self.horizontalLayout_4 = QHBoxLayout()
        self.horizontalLayout_4.setObjectName(u"horizontalLayout_4")
        self.lastUpdatedLabel = QLabel(self.tab)
        self.lastUpdatedLabel.setObjectName(u"lastUpdatedLabel")
        sizePolicy2.setHeightForWidth(self.lastUpdatedLabel.sizePolicy().hasHeightForWidth())
        self.lastUpdatedLabel.setSizePolicy(sizePolicy2)

        self.horizontalLayout_4.addWidget(self.lastUpdatedLabel)

        self.lastUpdatedValue = QLabel(self.tab)
        self.lastUpdatedValue.setObjectName(u"lastUpdatedValue")
        sizePolicy2.setHeightForWidth(self.lastUpdatedValue.sizePolicy().hasHeightForWidth())
        self.lastUpdatedValue.setSizePolicy(sizePolicy2)

        self.horizontalLayout_4.addWidget(self.lastUpdatedValue)


        self.verticalLayout_5.addLayout(self.horizontalLayout_4)

        self.horizontalLayout_3 = QHBoxLayout()
        self.horizontalLayout_3.setObjectName(u"horizontalLayout_3")
        self.hostLabel = QLabel(self.tab)
        self.hostLabel.setObjectName(u"hostLabel")
        sizePolicy1.setHeightForWidth(self.hostLabel.sizePolicy().hasHeightForWidth())
        self.hostLabel.setSizePolicy(sizePolicy1)

        self.horizontalLayout_3.addWidget(self.hostLabel)

        self.hostValue = QLabel(self.tab)
        self.hostValue.setObjectName(u"hostValue")
        sizePolicy2.setHeightForWidth(self.hostValue.sizePolicy().hasHeightForWidth())
        self.hostValue.setSizePolicy(sizePolicy2)

        self.horizontalLayout_3.addWidget(self.hostValue)


        self.verticalLayout_5.addLayout(self.horizontalLayout_3)

        self.restartCypress = QPushButton(self.tab)
        self.restartCypress.setObjectName(u"restartCypress")

        self.verticalLayout_5.addWidget(self.restartCypress)

        self.verticalSpacer = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.verticalLayout_5.addItem(self.verticalSpacer)

        self.tabWidget.addTab(self.tab, "")
        self.tab_2 = QWidget()
        self.tab_2.setObjectName(u"tab_2")
        self.verticalLayout_4 = QVBoxLayout(self.tab_2)
        self.verticalLayout_4.setObjectName(u"verticalLayout_4")
        self.launchCRT = QPushButton(self.tab_2)
        self.launchCRT.setObjectName(u"launchCRT")

        self.verticalLayout_4.addWidget(self.launchCRT)

        self.launchFRAX = QPushButton(self.tab_2)
        self.launchFRAX.setObjectName(u"launchFRAX")

        self.verticalLayout_4.addWidget(self.launchFRAX)

        self.launchAudiometer = QPushButton(self.tab_2)
        self.launchAudiometer.setObjectName(u"launchAudiometer")

        self.verticalLayout_4.addWidget(self.launchAudiometer)

        self.launchCDTT = QPushButton(self.tab_2)
        self.launchCDTT.setObjectName(u"launchCDTT")

        self.verticalLayout_4.addWidget(self.launchCDTT)

        self.launchDEXA = QPushButton(self.tab_2)
        self.launchDEXA.setObjectName(u"launchDEXA")

        self.verticalLayout_4.addWidget(self.launchDEXA)

        self.launchGripStrength = QPushButton(self.tab_2)
        self.launchGripStrength.setObjectName(u"launchGripStrength")

        self.verticalLayout_4.addWidget(self.launchGripStrength)

        self.launchECG = QPushButton(self.tab_2)
        self.launchECG.setObjectName(u"launchECG")

        self.verticalLayout_4.addWidget(self.launchECG)

        self.launchTonometer = QPushButton(self.tab_2)
        self.launchTonometer.setObjectName(u"launchTonometer")

        self.verticalLayout_4.addWidget(self.launchTonometer)

        self.launchSpirometer = QPushButton(self.tab_2)
        self.launchSpirometer.setObjectName(u"launchSpirometer")

        self.verticalLayout_4.addWidget(self.launchSpirometer)

        self.launchECHO = QPushButton(self.tab_2)
        self.launchECHO.setObjectName(u"launchECHO")

        self.verticalLayout_4.addWidget(self.launchECHO)

        self.launchWeighScale = QPushButton(self.tab_2)
        self.launchWeighScale.setObjectName(u"launchWeighScale")

        self.verticalLayout_4.addWidget(self.launchWeighScale)

        self.launchGeneralProxy = QPushButton(self.tab_2)
        self.launchGeneralProxy.setObjectName(u"launchGeneralProxy")

        self.verticalLayout_4.addWidget(self.launchGeneralProxy)

        self.tabWidget.addTab(self.tab_2, "")

        self.verticalLayout_2.addWidget(self.tabWidget)

        self.verticalSpacer_3 = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.verticalLayout_2.addItem(self.verticalSpacer_3)

        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(MainWindow)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 436, 33))
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QStatusBar(MainWindow)
        self.statusbar.setObjectName(u"statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)

        self.tabWidget.setCurrentIndex(0)


        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QCoreApplication.translate("MainWindow", u"Cypress", None))
        self.statusLabel.setText(QCoreApplication.translate("MainWindow", u"Status", None))
        self.statusValue.setText("")
        self.versionLabel.setText(QCoreApplication.translate("MainWindow", u"Version", None))
        self.versionValue.setText("")
        self.lastUpdatedLabel.setText(QCoreApplication.translate("MainWindow", u"Last updated", None))
        self.lastUpdatedValue.setText("")
        self.hostLabel.setText(QCoreApplication.translate("MainWindow", u"Host", None))
        self.hostValue.setText("")
        self.restartCypress.setText(QCoreApplication.translate("MainWindow", u"Restart", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab), QCoreApplication.translate("MainWindow", u"Server", None))
        self.launchCRT.setText(QCoreApplication.translate("MainWindow", u"Choice Reaction Test", None))
        self.launchFRAX.setText(QCoreApplication.translate("MainWindow", u"FRAX", None))
        self.launchAudiometer.setText(QCoreApplication.translate("MainWindow", u"Audiometer", None))
        self.launchCDTT.setText(QCoreApplication.translate("MainWindow", u"CDTT", None))
        self.launchDEXA.setText(QCoreApplication.translate("MainWindow", u"DEXA", None))
        self.launchGripStrength.setText(QCoreApplication.translate("MainWindow", u"Grip Strength", None))
        self.launchECG.setText(QCoreApplication.translate("MainWindow", u"ECG", None))
        self.launchTonometer.setText(QCoreApplication.translate("MainWindow", u"Tonometer", None))
        self.launchSpirometer.setText(QCoreApplication.translate("MainWindow", u"Spirometer", None))
        self.launchECHO.setText(QCoreApplication.translate("MainWindow", u"ECHO", None))
        self.launchWeighScale.setText(QCoreApplication.translate("MainWindow", u"Weigh Scale", None))
        self.launchGeneralProxy.setText(QCoreApplication.translate("MainWindow", u"General Proxy", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_2), QCoreApplication.translate("MainWindow", u"Devices", None))
    # retranslateUi

