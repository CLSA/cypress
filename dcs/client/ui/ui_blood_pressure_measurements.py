# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'blood_pressure_measurements.ui'
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
from PySide6.QtWidgets import (QApplication, QHBoxLayout, QHeaderView, QPushButton,
    QSizePolicy, QSpacerItem, QTableWidget, QTableWidgetItem,
    QVBoxLayout, QWidget)

class Ui_BloodPressureMeasurements(object):
    def setupUi(self, BloodPressureMeasurements):
        if not BloodPressureMeasurements.objectName():
            BloodPressureMeasurements.setObjectName(u"BloodPressureMeasurements")
        BloodPressureMeasurements.resize(436, 332)
        self.verticalLayout = QVBoxLayout(BloodPressureMeasurements)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.testControls = QHBoxLayout()
        self.testControls.setObjectName(u"testControls")
        self.manualEntryButton = QPushButton(BloodPressureMeasurements)
        self.manualEntryButton.setObjectName(u"manualEntryButton")

        self.testControls.addWidget(self.manualEntryButton)

        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.testControls.addItem(self.horizontalSpacer)

        self.measureButton = QPushButton(BloodPressureMeasurements)
        self.measureButton.setObjectName(u"measureButton")

        self.testControls.addWidget(self.measureButton)

        self.submitButton = QPushButton(BloodPressureMeasurements)
        self.submitButton.setObjectName(u"submitButton")

        self.testControls.addWidget(self.submitButton)


        self.verticalLayout.addLayout(self.testControls)

        self.table = QTableWidget(BloodPressureMeasurements)
        self.table.setObjectName(u"table")
        self.table.setShowGrid(True)
        self.table.setWordWrap(True)
        self.table.setCornerButtonEnabled(True)
        self.table.setColumnCount(0)
        self.table.setSupportedDragActions(Qt.DropAction.IgnoreAction)
        self.table.horizontalHeader().setVisible(True)
        self.table.horizontalHeader().setCascadingSectionResizes(True)
        self.table.horizontalHeader().setMinimumSectionSize(16)
        self.table.horizontalHeader().setDefaultSectionSize(100)
        self.table.horizontalHeader().setHighlightSections(True)
        self.table.horizontalHeader().setStretchLastSection(False)
        self.table.verticalHeader().setVisible(False)

        self.verticalLayout.addWidget(self.table)

        self.tableControls = QHBoxLayout()
        self.tableControls.setObjectName(u"tableControls")
        self.deleteRow = QPushButton(BloodPressureMeasurements)
        self.deleteRow.setObjectName(u"deleteRow")

        self.tableControls.addWidget(self.deleteRow)

        self.horizontalSpacer_2 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.tableControls.addItem(self.horizontalSpacer_2)

        self.addRow = QPushButton(BloodPressureMeasurements)
        self.addRow.setObjectName(u"addRow")

        self.tableControls.addWidget(self.addRow)


        self.verticalLayout.addLayout(self.tableControls)


        self.retranslateUi(BloodPressureMeasurements)

        QMetaObject.connectSlotsByName(BloodPressureMeasurements)
    # setupUi

    def retranslateUi(self, BloodPressureMeasurements):
        BloodPressureMeasurements.setWindowTitle(QCoreApplication.translate("BloodPressureMeasurements", u"Form", None))
        self.manualEntryButton.setText(QCoreApplication.translate("BloodPressureMeasurements", u"Manual Entry", None))
        self.measureButton.setText(QCoreApplication.translate("BloodPressureMeasurements", u"Measure", None))
        self.submitButton.setText(QCoreApplication.translate("BloodPressureMeasurements", u"Submit", None))
        self.deleteRow.setText(QCoreApplication.translate("BloodPressureMeasurements", u"Delete Row", None))
        self.addRow.setText(QCoreApplication.translate("BloodPressureMeasurements", u"Add Row", None))
    # retranslateUi

