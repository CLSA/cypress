# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'measurement_table.ui'
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
from PySide6.QtWidgets import (QApplication, QFrame, QGroupBox, QHBoxLayout,
    QHeaderView, QPushButton, QSizePolicy, QSpacerItem,
    QTableWidget, QTableWidgetItem, QVBoxLayout, QWidget)

class Ui_MeasurementTable(object):
    def setupUi(self, MeasurementTable):
        if not MeasurementTable.objectName():
            MeasurementTable.setObjectName(u"MeasurementTable")
        MeasurementTable.resize(445, 293)
        self.verticalLayout = QVBoxLayout(MeasurementTable)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.measurementsInfo = QGroupBox(MeasurementTable)
        self.measurementsInfo.setObjectName(u"measurementsInfo")
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Minimum)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.measurementsInfo.sizePolicy().hasHeightForWidth())
        self.measurementsInfo.setSizePolicy(sizePolicy)
        self.verticalLayout_2 = QVBoxLayout(self.measurementsInfo)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.verticalLayout_2.setContentsMargins(6, 4, 6, 4)
        self.horizontalLayout_3 = QHBoxLayout()
        self.horizontalLayout_3.setSpacing(6)
        self.horizontalLayout_3.setObjectName(u"horizontalLayout_3")
        self.manualEntryToggle = QPushButton(self.measurementsInfo)
        self.manualEntryToggle.setObjectName(u"manualEntryToggle")

        self.horizontalLayout_3.addWidget(self.manualEntryToggle)

        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_3.addItem(self.horizontalSpacer)

        self.measureButton = QPushButton(self.measurementsInfo)
        self.measureButton.setObjectName(u"measureButton")
        self.measureButton.setEnabled(False)

        self.horizontalLayout_3.addWidget(self.measureButton)

        self.submitButton = QPushButton(self.measurementsInfo)
        self.submitButton.setObjectName(u"submitButton")
        self.submitButton.setEnabled(True)

        self.horizontalLayout_3.addWidget(self.submitButton)


        self.verticalLayout_2.addLayout(self.horizontalLayout_3)

        self.measurementTable = QTableWidget(self.measurementsInfo)
        self.measurementTable.setObjectName(u"measurementTable")
        sizePolicy1 = QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.measurementTable.sizePolicy().hasHeightForWidth())
        self.measurementTable.setSizePolicy(sizePolicy1)
        self.measurementTable.setFrameShape(QFrame.Shape.NoFrame)
        self.measurementTable.setFrameShadow(QFrame.Shadow.Plain)
        self.measurementTable.setLineWidth(0)
        self.measurementTable.setGridStyle(Qt.PenStyle.NoPen)
        self.measurementTable.setCornerButtonEnabled(False)

        self.verticalLayout_2.addWidget(self.measurementTable)

        self.verticalLayout_2.setStretch(1, 1)

        self.verticalLayout.addWidget(self.measurementsInfo)


        self.retranslateUi(MeasurementTable)

        QMetaObject.connectSlotsByName(MeasurementTable)
    # setupUi

    def retranslateUi(self, MeasurementTable):
        MeasurementTable.setWindowTitle(QCoreApplication.translate("MeasurementTable", u"Form", None))
        self.measurementsInfo.setTitle(QCoreApplication.translate("MeasurementTable", u"Measurements", None))
        self.manualEntryToggle.setText(QCoreApplication.translate("MeasurementTable", u"Manual Entry", None))
        self.measureButton.setText(QCoreApplication.translate("MeasurementTable", u"Measure", None))
        self.submitButton.setText(QCoreApplication.translate("MeasurementTable", u"Submit", None))
    # retranslateUi

