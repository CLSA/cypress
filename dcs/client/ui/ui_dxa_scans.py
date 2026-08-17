# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'dxa_scans.ui'
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
from PySide6.QtWidgets import (QApplication, QHBoxLayout, QLabel, QPushButton,
    QSizePolicy, QSpacerItem, QTabWidget, QVBoxLayout,
    QWidget)

class Ui_Form(object):
    def setupUi(self, Form):
        if not Form.objectName():
            Form.setObjectName(u"Form")
        Form.resize(596, 385)
        self.verticalLayout = QVBoxLayout(Form)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.tabWidget = QTabWidget(Form)
        self.tabWidget.setObjectName(u"tabWidget")
        self.tabWidget.setTabShape(QTabWidget.TabShape.Rounded)
        self.tabWidget.setDocumentMode(False)
        self.tabWidget.setTabBarAutoHide(False)
        self.tab = QWidget()
        self.tab.setObjectName(u"tab")
        self.tab.setAutoFillBackground(True)
        self.verticalLayout_10 = QVBoxLayout(self.tab)
        self.verticalLayout_10.setSpacing(0)
        self.verticalLayout_10.setObjectName(u"verticalLayout_10")
        self.verticalLayout_10.setContentsMargins(6, 6, 6, 6)
        self.controls = QHBoxLayout()
        self.controls.setObjectName(u"controls")
        self.manual_entry_button = QPushButton(self.tab)
        self.manual_entry_button.setObjectName(u"manual_entry_button")
        self.manual_entry_button.setEnabled(False)

        self.controls.addWidget(self.manual_entry_button)

        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.controls.addItem(self.horizontalSpacer)

        self.measure_button = QPushButton(self.tab)
        self.measure_button.setObjectName(u"measure_button")

        self.controls.addWidget(self.measure_button)

        self.submit_button = QPushButton(self.tab)
        self.submit_button.setObjectName(u"submit_button")

        self.controls.addWidget(self.submit_button)


        self.verticalLayout_10.addLayout(self.controls)

        self.total = QHBoxLayout()
        self.total.setObjectName(u"total")
        self.total.setContentsMargins(4, 4, 4, 4)
        self.scans_received_label = QLabel(self.tab)
        self.scans_received_label.setObjectName(u"scans_received_label")
        self.scans_received_label.setMargin(4)

        self.total.addWidget(self.scans_received_label)

        self.scans_received_value = QLabel(self.tab)
        self.scans_received_value.setObjectName(u"scans_received_value")
        self.scans_received_value.setAlignment(Qt.AlignmentFlag.AlignLeading|Qt.AlignmentFlag.AlignLeft|Qt.AlignmentFlag.AlignVCenter)
        self.scans_received_value.setMargin(4)

        self.total.addWidget(self.scans_received_value)


        self.verticalLayout_10.addLayout(self.total)

        self.left_hip = QHBoxLayout()
        self.left_hip.setObjectName(u"left_hip")
        self.left_hip.setContentsMargins(4, 4, 4, 4)
        self.left_hip_label = QLabel(self.tab)
        self.left_hip_label.setObjectName(u"left_hip_label")
        self.left_hip_label.setMargin(4)

        self.left_hip.addWidget(self.left_hip_label)

        self.left_hip_value = QLabel(self.tab)
        self.left_hip_value.setObjectName(u"left_hip_value")
        self.left_hip_value.setMargin(4)

        self.left_hip.addWidget(self.left_hip_value)


        self.verticalLayout_10.addLayout(self.left_hip)

        self.right_hip = QHBoxLayout()
        self.right_hip.setObjectName(u"right_hip")
        self.right_hip.setContentsMargins(4, 4, 4, 4)
        self.right_hip_label = QLabel(self.tab)
        self.right_hip_label.setObjectName(u"right_hip_label")
        self.right_hip_label.setMargin(4)

        self.right_hip.addWidget(self.right_hip_label)

        self.right_hip_value = QLabel(self.tab)
        self.right_hip_value.setObjectName(u"right_hip_value")
        self.right_hip_value.setMargin(4)

        self.right_hip.addWidget(self.right_hip_value)


        self.verticalLayout_10.addLayout(self.right_hip)

        self.horizontalLayout_2 = QHBoxLayout()
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.horizontalLayout_2.setContentsMargins(4, 4, 4, 4)
        self.left_forearm_label = QLabel(self.tab)
        self.left_forearm_label.setObjectName(u"left_forearm_label")
        self.left_forearm_label.setMargin(4)

        self.horizontalLayout_2.addWidget(self.left_forearm_label)

        self.left_forearm_value = QLabel(self.tab)
        self.left_forearm_value.setObjectName(u"left_forearm_value")
        self.left_forearm_value.setMargin(4)

        self.horizontalLayout_2.addWidget(self.left_forearm_value)


        self.verticalLayout_10.addLayout(self.horizontalLayout_2)

        self.right_forearm = QHBoxLayout()
        self.right_forearm.setObjectName(u"right_forearm")
        self.right_forearm.setContentsMargins(4, 4, 4, 4)
        self.right_forearm_label = QLabel(self.tab)
        self.right_forearm_label.setObjectName(u"right_forearm_label")
        self.right_forearm_label.setMargin(4)

        self.right_forearm.addWidget(self.right_forearm_label)

        self.right_forearm_value = QLabel(self.tab)
        self.right_forearm_value.setObjectName(u"right_forearm_value")
        self.right_forearm_value.setMargin(4)

        self.right_forearm.addWidget(self.right_forearm_value)


        self.verticalLayout_10.addLayout(self.right_forearm)

        self.spine = QHBoxLayout()
        self.spine.setObjectName(u"spine")
        self.spine.setContentsMargins(4, 4, 4, 4)
        self.spine_label = QLabel(self.tab)
        self.spine_label.setObjectName(u"spine_label")
        self.spine_label.setMargin(4)

        self.spine.addWidget(self.spine_label)

        self.spine_value = QLabel(self.tab)
        self.spine_value.setObjectName(u"spine_value")
        self.spine_value.setMargin(4)

        self.spine.addWidget(self.spine_value)


        self.verticalLayout_10.addLayout(self.spine)

        self.lateral_spine = QHBoxLayout()
        self.lateral_spine.setObjectName(u"lateral_spine")
        self.lateral_spine.setContentsMargins(4, 4, 4, 4)
        self.lateral_spine_label = QLabel(self.tab)
        self.lateral_spine_label.setObjectName(u"lateral_spine_label")
        self.lateral_spine_label.setMargin(4)

        self.lateral_spine.addWidget(self.lateral_spine_label)

        self.lateral_spine_value = QLabel(self.tab)
        self.lateral_spine_value.setObjectName(u"lateral_spine_value")
        self.lateral_spine_value.setMargin(4)

        self.lateral_spine.addWidget(self.lateral_spine_value)


        self.verticalLayout_10.addLayout(self.lateral_spine)

        self.whole_body = QHBoxLayout()
        self.whole_body.setObjectName(u"whole_body")
        self.whole_body.setContentsMargins(4, 4, 4, 4)
        self.whole_body_label = QLabel(self.tab)
        self.whole_body_label.setObjectName(u"whole_body_label")
        self.whole_body_label.setMargin(4)

        self.whole_body.addWidget(self.whole_body_label)

        self.whole_body_value = QLabel(self.tab)
        self.whole_body_value.setObjectName(u"whole_body_value")
        self.whole_body_value.setMargin(4)

        self.whole_body.addWidget(self.whole_body_value)


        self.verticalLayout_10.addLayout(self.whole_body)

        self.verticalSpacer = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.verticalLayout_10.addItem(self.verticalSpacer)

        self.tabWidget.addTab(self.tab, "")
        self.tab_2 = QWidget()
        self.tab_2.setObjectName(u"tab_2")
        self.verticalLayout_12 = QVBoxLayout(self.tab_2)
        self.verticalLayout_12.setObjectName(u"verticalLayout_12")
        self.verticalLayout_12.setContentsMargins(0, 0, 0, 0)
        self.tabWidget.addTab(self.tab_2, "")

        self.verticalLayout.addWidget(self.tabWidget)


        self.retranslateUi(Form)

        self.tabWidget.setCurrentIndex(0)


        QMetaObject.connectSlotsByName(Form)
    # setupUi

    def retranslateUi(self, Form):
        Form.setWindowTitle(QCoreApplication.translate("Form", u"Form", None))
        self.manual_entry_button.setText(QCoreApplication.translate("Form", u"Manual Entry", None))
        self.measure_button.setText(QCoreApplication.translate("Form", u"Measure", None))
        self.submit_button.setText(QCoreApplication.translate("Form", u"Submit", None))
        self.scans_received_label.setText(QCoreApplication.translate("Form", u"Total", None))
        self.scans_received_value.setText(QCoreApplication.translate("Form", u"---", None))
        self.left_hip_label.setText(QCoreApplication.translate("Form", u"Left hip", None))
        self.left_hip_value.setText(QCoreApplication.translate("Form", u"0 / 1", None))
        self.right_hip_label.setText(QCoreApplication.translate("Form", u"Right hip", None))
        self.right_hip_value.setText(QCoreApplication.translate("Form", u"0 / 1", None))
        self.left_forearm_label.setText(QCoreApplication.translate("Form", u"Left forearm", None))
        self.left_forearm_value.setText(QCoreApplication.translate("Form", u"0 / 1", None))
        self.right_forearm_label.setText(QCoreApplication.translate("Form", u"Right forearm", None))
        self.right_forearm_value.setText(QCoreApplication.translate("Form", u"0 / 1", None))
        self.spine_label.setText(QCoreApplication.translate("Form", u"Spine", None))
        self.spine_value.setText(QCoreApplication.translate("Form", u"0 / 1", None))
        self.lateral_spine_label.setText(QCoreApplication.translate("Form", u"Lateral spine", None))
        self.lateral_spine_value.setText(QCoreApplication.translate("Form", u"0 / 3", None))
        self.whole_body_label.setText(QCoreApplication.translate("Form", u"Whole body", None))
        self.whole_body_value.setText(QCoreApplication.translate("Form", u"0 / 2", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab), QCoreApplication.translate("Form", u"Scans", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_2), QCoreApplication.translate("Form", u"Log", None))
    # retranslateUi

