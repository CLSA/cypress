# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'weight_measurements.ui'
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

class Ui_WeightMeasurements(object):
    def setupUi(self, WeightMeasurements):
        if not WeightMeasurements.objectName():
            WeightMeasurements.setObjectName(u"WeightMeasurements")
        WeightMeasurements.resize(443, 339)
        self.verticalLayout = QVBoxLayout(WeightMeasurements)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.scaleBox = QGroupBox(WeightMeasurements)
        self.scaleBox.setObjectName(u"scaleBox")
        self.verticalLayout_2 = QVBoxLayout(self.scaleBox)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.horizontalLayout = QHBoxLayout()
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.zeroButton = QPushButton(self.scaleBox)
        self.zeroButton.setObjectName(u"zeroButton")

        self.horizontalLayout.addWidget(self.zeroButton)


        self.verticalLayout_2.addLayout(self.horizontalLayout)


        self.verticalLayout.addWidget(self.scaleBox)

        self.measurements = QGroupBox(WeightMeasurements)
        self.measurements.setObjectName(u"measurements")
        self.verticalLayout_3 = QVBoxLayout(self.measurements)
        self.verticalLayout_3.setObjectName(u"verticalLayout_3")
        self.testControls = QHBoxLayout()
        self.testControls.setObjectName(u"testControls")
        self.manualEntryButton = QPushButton(self.measurements)
        self.manualEntryButton.setObjectName(u"manualEntryButton")

        self.testControls.addWidget(self.manualEntryButton)

        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.testControls.addItem(self.horizontalSpacer)

        self.measureButton = QPushButton(self.measurements)
        self.measureButton.setObjectName(u"measureButton")

        self.testControls.addWidget(self.measureButton)

        self.submitButton = QPushButton(self.measurements)
        self.submitButton.setObjectName(u"submitButton")

        self.testControls.addWidget(self.submitButton)


        self.verticalLayout_3.addLayout(self.testControls)

        self.table = QTableWidget(self.measurements)
        self.table.setObjectName(u"table")
        self.table.setFrameShape(QFrame.Shape.NoFrame)
        self.table.setFrameShadow(QFrame.Shadow.Plain)
        self.table.setLineWidth(0)
        self.table.setShowGrid(True)
        self.table.setGridStyle(Qt.PenStyle.NoPen)
        self.table.setWordWrap(True)
        self.table.setCornerButtonEnabled(False)
        self.table.setColumnCount(0)
        self.table.setSupportedDragActions(Qt.DropAction.IgnoreAction)
        self.table.horizontalHeader().setVisible(True)
        self.table.horizontalHeader().setCascadingSectionResizes(True)
        self.table.horizontalHeader().setMinimumSectionSize(16)
        self.table.horizontalHeader().setDefaultSectionSize(100)
        self.table.horizontalHeader().setHighlightSections(False)
        self.table.horizontalHeader().setStretchLastSection(False)
        self.table.verticalHeader().setVisible(False)

        self.verticalLayout_3.addWidget(self.table)

        self.tableControls = QHBoxLayout()
        self.tableControls.setObjectName(u"tableControls")
        self.deleteRow = QPushButton(self.measurements)
        self.deleteRow.setObjectName(u"deleteRow")

        self.tableControls.addWidget(self.deleteRow)

        self.horizontalSpacer_2 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.tableControls.addItem(self.horizontalSpacer_2)

        self.addRow = QPushButton(self.measurements)
        self.addRow.setObjectName(u"addRow")

        self.tableControls.addWidget(self.addRow)


        self.verticalLayout_3.addLayout(self.tableControls)


        self.verticalLayout.addWidget(self.measurements)


        self.retranslateUi(WeightMeasurements)

        QMetaObject.connectSlotsByName(WeightMeasurements)
    # setupUi

    def retranslateUi(self, WeightMeasurements):
        WeightMeasurements.setWindowTitle(QCoreApplication.translate("WeightMeasurements", u"Form", None))
        self.scaleBox.setTitle(QCoreApplication.translate("WeightMeasurements", u"Scale Controls", None))
        self.zeroButton.setText(QCoreApplication.translate("WeightMeasurements", u"Zero", None))
        self.measurements.setTitle(QCoreApplication.translate("WeightMeasurements", u"Measurements", None))
        self.manualEntryButton.setText(QCoreApplication.translate("WeightMeasurements", u"Manual Entry", None))
        self.measureButton.setText(QCoreApplication.translate("WeightMeasurements", u"Measure", None))
        self.submitButton.setText(QCoreApplication.translate("WeightMeasurements", u"Submit", None))
        self.deleteRow.setText(QCoreApplication.translate("WeightMeasurements", u"Delete Row", None))
        self.addRow.setText(QCoreApplication.translate("WeightMeasurements", u"Add Row", None))
    # retranslateUi

