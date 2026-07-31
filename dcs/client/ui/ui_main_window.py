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
        MainWindow.resize(436, 498)
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

        self.horizontalLayout_5 = QHBoxLayout()
        self.horizontalLayout_5.setObjectName(u"horizontalLayout_5")
        self.startCypress = QPushButton(self.tab)
        self.startCypress.setObjectName(u"startCypress")

        self.horizontalLayout_5.addWidget(self.startCypress)

        self.stopCypress = QPushButton(self.tab)
        self.stopCypress.setObjectName(u"stopCypress")

        self.horizontalLayout_5.addWidget(self.stopCypress)


        self.verticalLayout_5.addLayout(self.horizontalLayout_5)

        self.verticalSpacer = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.verticalLayout_5.addItem(self.verticalSpacer)

        self.tabWidget.addTab(self.tab, "")

        self.verticalLayout_2.addWidget(self.tabWidget)

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
        self.startCypress.setText(QCoreApplication.translate("MainWindow", u"Start Server", None))
        self.stopCypress.setText(QCoreApplication.translate("MainWindow", u"Stop Server", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab), QCoreApplication.translate("MainWindow", u"Server", None))
    # retranslateUi

