# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'echo_scans.ui'
##
## Created by: Qt User Interface Compiler version 6.11.1
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
from PySide6.QtWidgets import (QApplication, QGroupBox, QHBoxLayout, QLabel,
    QPushButton, QSizePolicy, QSpacerItem, QTabWidget,
    QVBoxLayout, QWidget)

class Ui_ECHOScansWidget(object):
    def setupUi(self, ECHOScansWidget):
        if not ECHOScansWidget.objectName():
            ECHOScansWidget.setObjectName(u"ECHOScansWidget")
        ECHOScansWidget.resize(400, 300)
        self.verticalLayout = QVBoxLayout(ECHOScansWidget)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.controls = QHBoxLayout()
        self.controls.setObjectName(u"controls")
        self.manual_entry_button = QPushButton(ECHOScansWidget)
        self.manual_entry_button.setObjectName(u"manual_entry_button")

        self.controls.addWidget(self.manual_entry_button)

        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.controls.addItem(self.horizontalSpacer)

        self.measure_button = QPushButton(ECHOScansWidget)
        self.measure_button.setObjectName(u"measure_button")

        self.controls.addWidget(self.measure_button)

        self.submit_button = QPushButton(ECHOScansWidget)
        self.submit_button.setObjectName(u"submit_button")

        self.controls.addWidget(self.submit_button)


        self.verticalLayout.addLayout(self.controls)

        self.tabWidget = QTabWidget(ECHOScansWidget)
        self.tabWidget.setObjectName(u"tabWidget")
        self.tab = QWidget()
        self.tab.setObjectName(u"tab")
        self.verticalLayout_10 = QVBoxLayout(self.tab)
        self.verticalLayout_10.setObjectName(u"verticalLayout_10")
        self.scans_received = QGroupBox(self.tab)
        self.scans_received.setObjectName(u"scans_received")
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.scans_received.sizePolicy().hasHeightForWidth())
        self.scans_received.setSizePolicy(sizePolicy)
        self.scans_received.setFlat(True)
        self.verticalLayout_2 = QVBoxLayout(self.scans_received)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.total = QHBoxLayout()
        self.total.setObjectName(u"total")
        self.scans_received_label = QLabel(self.scans_received)
        self.scans_received_label.setObjectName(u"scans_received_label")

        self.total.addWidget(self.scans_received_label)

        self.scans_received_value = QLabel(self.scans_received)
        self.scans_received_value.setObjectName(u"scans_received_value")
        self.scans_received_value.setAlignment(Qt.AlignmentFlag.AlignLeading|Qt.AlignmentFlag.AlignLeft|Qt.AlignmentFlag.AlignVCenter)

        self.total.addWidget(self.scans_received_value)


        self.verticalLayout_2.addLayout(self.total)

        self.us = QWidget(self.scans_received)
        self.us.setObjectName(u"us")
        self.verticalLayout_3 = QVBoxLayout(self.us)
        self.verticalLayout_3.setObjectName(u"verticalLayout_3")
        self.verticalLayout_3.setContentsMargins(0, 0, 0, 0)
        self.layout = QHBoxLayout()
        self.layout.setObjectName(u"layout")
        self.us_label = QLabel(self.us)
        self.us_label.setObjectName(u"us_label")

        self.layout.addWidget(self.us_label)

        self.us_value = QLabel(self.us)
        self.us_value.setObjectName(u"us_value")

        self.layout.addWidget(self.us_value)


        self.verticalLayout_3.addLayout(self.layout)


        self.verticalLayout_2.addWidget(self.us)

        self.usm = QWidget(self.scans_received)
        self.usm.setObjectName(u"usm")
        self.verticalLayout_4 = QVBoxLayout(self.usm)
        self.verticalLayout_4.setObjectName(u"verticalLayout_4")
        self.verticalLayout_4.setContentsMargins(0, 0, 0, 0)
        self.layout_2 = QHBoxLayout()
        self.layout_2.setObjectName(u"layout_2")
        self.usm_label = QLabel(self.usm)
        self.usm_label.setObjectName(u"usm_label")

        self.layout_2.addWidget(self.usm_label)

        self.usm_value = QLabel(self.usm)
        self.usm_value.setObjectName(u"usm_value")

        self.layout_2.addWidget(self.usm_value)


        self.verticalLayout_4.addLayout(self.layout_2)


        self.verticalLayout_2.addWidget(self.usm)

        self.src = QWidget(self.scans_received)
        self.src.setObjectName(u"src")
        self.verticalLayout_5 = QVBoxLayout(self.src)
        self.verticalLayout_5.setObjectName(u"verticalLayout_5")
        self.verticalLayout_5.setContentsMargins(0, 0, 0, 0)
        self.layout_3 = QHBoxLayout()
        self.layout_3.setObjectName(u"layout_3")
        self.src_label = QLabel(self.src)
        self.src_label.setObjectName(u"src_label")

        self.layout_3.addWidget(self.src_label)

        self.src_value = QLabel(self.src)
        self.src_value.setObjectName(u"src_value")

        self.layout_3.addWidget(self.src_value)


        self.verticalLayout_5.addLayout(self.layout_3)


        self.verticalLayout_2.addWidget(self.src)

        self.verticalSpacer = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.verticalLayout_2.addItem(self.verticalSpacer)


        self.verticalLayout_10.addWidget(self.scans_received)

        self.tabWidget.addTab(self.tab, "")
        self.tab_2 = QWidget()
        self.tab_2.setObjectName(u"tab_2")
        self.verticalLayout_12 = QVBoxLayout(self.tab_2)
        self.verticalLayout_12.setObjectName(u"verticalLayout_12")
        self.tabWidget.addTab(self.tab_2, "")

        self.verticalLayout.addWidget(self.tabWidget)


        self.retranslateUi(ECHOScansWidget)

        self.tabWidget.setCurrentIndex(0)


        QMetaObject.connectSlotsByName(ECHOScansWidget)
    # setupUi

    def retranslateUi(self, ECHOScansWidget):
        ECHOScansWidget.setWindowTitle(QCoreApplication.translate("ECHOScansWidget", u"Form", None))
        self.manual_entry_button.setText(QCoreApplication.translate("ECHOScansWidget", u"Manual Entry", None))
        self.measure_button.setText(QCoreApplication.translate("ECHOScansWidget", u"Measure", None))
        self.submit_button.setText(QCoreApplication.translate("ECHOScansWidget", u"Submit", None))
        self.scans_received.setTitle("")
        self.scans_received_label.setText(QCoreApplication.translate("ECHOScansWidget", u"Scans received", None))
        self.scans_received_value.setText(QCoreApplication.translate("ECHOScansWidget", u"0", None))
        self.us_label.setText(QCoreApplication.translate("ECHOScansWidget", u"US", None))
        self.us_value.setText(QCoreApplication.translate("ECHOScansWidget", u"0", None))
        self.usm_label.setText(QCoreApplication.translate("ECHOScansWidget", u"USm", None))
        self.usm_value.setText(QCoreApplication.translate("ECHOScansWidget", u"0", None))
        self.src_label.setText(QCoreApplication.translate("ECHOScansWidget", u"SRc", None))
        self.src_value.setText(QCoreApplication.translate("ECHOScansWidget", u"0", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab), QCoreApplication.translate("ECHOScansWidget", u"Scans", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_2), QCoreApplication.translate("ECHOScansWidget", u"Log", None))
    # retranslateUi

