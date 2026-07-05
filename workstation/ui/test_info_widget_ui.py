# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'test_info_widget.ui'
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
from PySide6.QtWidgets import (QApplication, QGroupBox, QHBoxLayout, QLabel,
    QLayout, QPushButton, QSizePolicy, QVBoxLayout,
    QWidget)

class Ui_TestInfoWidget(object):
    def setupUi(self, TestInfoWidget):
        if not TestInfoWidget.objectName():
            TestInfoWidget.setObjectName(u"TestInfoWidget")
        TestInfoWidget.resize(434, 239)
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(TestInfoWidget.sizePolicy().hasHeightForWidth())
        TestInfoWidget.setSizePolicy(sizePolicy)
        self.verticalLayout = QVBoxLayout(TestInfoWidget)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.groupBox = QGroupBox(TestInfoWidget)
        self.groupBox.setObjectName(u"groupBox")
        sizePolicy1 = QSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Minimum)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.groupBox.sizePolicy().hasHeightForWidth())
        self.groupBox.setSizePolicy(sizePolicy1)
        self.verticalLayout_2 = QVBoxLayout(self.groupBox)
        self.verticalLayout_2.setSpacing(9)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.barcode = QHBoxLayout()
        self.barcode.setObjectName(u"barcode")
        self.barcode.setSizeConstraint(QLayout.SizeConstraint.SetMinimumSize)
        self.barcode.setContentsMargins(0, -1, -1, -1)
        self.barcodeLabel = QLabel(self.groupBox)
        self.barcodeLabel.setObjectName(u"barcodeLabel")
        sizePolicy2 = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Minimum)
        sizePolicy2.setHorizontalStretch(0)
        sizePolicy2.setVerticalStretch(0)
        sizePolicy2.setHeightForWidth(self.barcodeLabel.sizePolicy().hasHeightForWidth())
        self.barcodeLabel.setSizePolicy(sizePolicy2)
        self.barcodeLabel.setMargin(4)

        self.barcode.addWidget(self.barcodeLabel)

        self.barcodeValue = QLabel(self.groupBox)
        self.barcodeValue.setObjectName(u"barcodeValue")
        sizePolicy2.setHeightForWidth(self.barcodeValue.sizePolicy().hasHeightForWidth())
        self.barcodeValue.setSizePolicy(sizePolicy2)
        self.barcodeValue.setAlignment(Qt.AlignmentFlag.AlignLeading|Qt.AlignmentFlag.AlignLeft|Qt.AlignmentFlag.AlignVCenter)
        self.barcodeValue.setMargin(4)

        self.barcode.addWidget(self.barcodeValue)


        self.verticalLayout_2.addLayout(self.barcode)

        self.interviewer = QHBoxLayout()
        self.interviewer.setObjectName(u"interviewer")
        self.interviewer.setSizeConstraint(QLayout.SizeConstraint.SetMinimumSize)
        self.interviewerLabel = QLabel(self.groupBox)
        self.interviewerLabel.setObjectName(u"interviewerLabel")
        sizePolicy2.setHeightForWidth(self.interviewerLabel.sizePolicy().hasHeightForWidth())
        self.interviewerLabel.setSizePolicy(sizePolicy2)
        self.interviewerLabel.setMargin(4)

        self.interviewer.addWidget(self.interviewerLabel)

        self.interviewerValue = QLabel(self.groupBox)
        self.interviewerValue.setObjectName(u"interviewerValue")
        sizePolicy2.setHeightForWidth(self.interviewerValue.sizePolicy().hasHeightForWidth())
        self.interviewerValue.setSizePolicy(sizePolicy2)
        self.interviewerValue.setAlignment(Qt.AlignmentFlag.AlignLeading|Qt.AlignmentFlag.AlignLeft|Qt.AlignmentFlag.AlignVCenter)
        self.interviewerValue.setMargin(4)

        self.interviewer.addWidget(self.interviewerValue)


        self.verticalLayout_2.addLayout(self.interviewer)

        self.horizontalLayout = QHBoxLayout()
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.startedLabel = QLabel(self.groupBox)
        self.startedLabel.setObjectName(u"startedLabel")
        self.startedLabel.setMargin(4)

        self.horizontalLayout.addWidget(self.startedLabel)

        self.startedValue = QLabel(self.groupBox)
        self.startedValue.setObjectName(u"startedValue")
        self.startedValue.setMargin(4)

        self.horizontalLayout.addWidget(self.startedValue)


        self.verticalLayout_2.addLayout(self.horizontalLayout)

        self.deviceStatus = QHBoxLayout()
        self.deviceStatus.setObjectName(u"deviceStatus")
        self.deviceStatus.setSizeConstraint(QLayout.SizeConstraint.SetMinimumSize)
        self.deviceStatusLabel = QLabel(self.groupBox)
        self.deviceStatusLabel.setObjectName(u"deviceStatusLabel")
        sizePolicy2.setHeightForWidth(self.deviceStatusLabel.sizePolicy().hasHeightForWidth())
        self.deviceStatusLabel.setSizePolicy(sizePolicy2)
        self.deviceStatusLabel.setMargin(4)

        self.deviceStatus.addWidget(self.deviceStatusLabel)

        self.deviceStatusValue = QLabel(self.groupBox)
        self.deviceStatusValue.setObjectName(u"deviceStatusValue")
        sizePolicy2.setHeightForWidth(self.deviceStatusValue.sizePolicy().hasHeightForWidth())
        self.deviceStatusValue.setSizePolicy(sizePolicy2)
        self.deviceStatusValue.setAlignment(Qt.AlignmentFlag.AlignLeading|Qt.AlignmentFlag.AlignLeft|Qt.AlignmentFlag.AlignVCenter)
        self.deviceStatusValue.setMargin(4)

        self.deviceStatus.addWidget(self.deviceStatusValue)


        self.verticalLayout_2.addLayout(self.deviceStatus)

        self.horizontalLayout_2 = QHBoxLayout()
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.statusLabel = QLabel(self.groupBox)
        self.statusLabel.setObjectName(u"statusLabel")
        self.statusLabel.setMargin(4)

        self.horizontalLayout_2.addWidget(self.statusLabel)

        self.statusValue = QLabel(self.groupBox)
        self.statusValue.setObjectName(u"statusValue")
        self.statusValue.setMargin(4)

        self.horizontalLayout_2.addWidget(self.statusValue)


        self.verticalLayout_2.addLayout(self.horizontalLayout_2)

        self.startButton = QPushButton(self.groupBox)
        self.startButton.setObjectName(u"startButton")

        self.verticalLayout_2.addWidget(self.startButton)


        self.verticalLayout.addWidget(self.groupBox)


        self.retranslateUi(TestInfoWidget)

        QMetaObject.connectSlotsByName(TestInfoWidget)
    # setupUi

    def retranslateUi(self, TestInfoWidget):
        TestInfoWidget.setWindowTitle(QCoreApplication.translate("TestInfoWidget", u"Form", None))
        self.groupBox.setTitle(QCoreApplication.translate("TestInfoWidget", u"Test Information", None))
        self.barcodeLabel.setText(QCoreApplication.translate("TestInfoWidget", u"Participant ID", None))
        self.barcodeValue.setText(QCoreApplication.translate("TestInfoWidget", u"---", None))
        self.interviewerLabel.setText(QCoreApplication.translate("TestInfoWidget", u"Interviewer", None))
        self.interviewerValue.setText(QCoreApplication.translate("TestInfoWidget", u"---", None))
        self.startedLabel.setText(QCoreApplication.translate("TestInfoWidget", u"Started", None))
        self.startedValue.setText(QCoreApplication.translate("TestInfoWidget", u"---", None))
        self.deviceStatusLabel.setText(QCoreApplication.translate("TestInfoWidget", u"Device", None))
        self.deviceStatusValue.setText(QCoreApplication.translate("TestInfoWidget", u"---", None))
        self.statusLabel.setText(QCoreApplication.translate("TestInfoWidget", u"Status", None))
        self.statusValue.setText(QCoreApplication.translate("TestInfoWidget", u"---", None))
        self.startButton.setText(QCoreApplication.translate("TestInfoWidget", u"Start", None))
    # retranslateUi

