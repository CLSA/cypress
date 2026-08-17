# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'hearing_measurements.ui'
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
from PySide6.QtWidgets import (QApplication, QFormLayout, QGroupBox, QHBoxLayout,
    QLabel, QLayout, QPushButton, QSizePolicy,
    QSpacerItem, QSpinBox, QVBoxLayout, QWidget)

class Ui_HearingMeasurements(object):
    def setupUi(self, HearingMeasurements):
        if not HearingMeasurements.objectName():
            HearingMeasurements.setObjectName(u"HearingMeasurements")
        HearingMeasurements.resize(492, 294)
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(HearingMeasurements.sizePolicy().hasHeightForWidth())
        HearingMeasurements.setSizePolicy(sizePolicy)
        HearingMeasurements.setMaximumSize(QSize(16777215, 16777215))
        self.verticalLayout = QVBoxLayout(HearingMeasurements)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout_2 = QHBoxLayout()
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.horizontalLayout_2.setSizeConstraint(QLayout.SizeConstraint.SetDefaultConstraint)
        self.manualEntryButton = QPushButton(HearingMeasurements)
        self.manualEntryButton.setObjectName(u"manualEntryButton")

        self.horizontalLayout_2.addWidget(self.manualEntryButton)

        self.saveButton = QPushButton(HearingMeasurements)
        self.saveButton.setObjectName(u"saveButton")

        self.horizontalLayout_2.addWidget(self.saveButton)

        self.horizontalSpacer = QSpacerItem(40, 10, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_2.addItem(self.horizontalSpacer)

        self.measureButton = QPushButton(HearingMeasurements)
        self.measureButton.setObjectName(u"measureButton")

        self.horizontalLayout_2.addWidget(self.measureButton)

        self.submitButton = QPushButton(HearingMeasurements)
        self.submitButton.setObjectName(u"submitButton")

        self.horizontalLayout_2.addWidget(self.submitButton)


        self.verticalLayout.addLayout(self.horizontalLayout_2)

        self.horizontalLayout = QHBoxLayout()
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.horizontalLayout.setSizeConstraint(QLayout.SizeConstraint.SetDefaultConstraint)
        self.leftGroup = QGroupBox(HearingMeasurements)
        self.leftGroup.setObjectName(u"leftGroup")
        sizePolicy.setHeightForWidth(self.leftGroup.sizePolicy().hasHeightForWidth())
        self.leftGroup.setSizePolicy(sizePolicy)
        self.leftGroup.setMinimumSize(QSize(0, 0))
        self.leftGroup.setMaximumSize(QSize(16777215, 16777215))
        self.leftGroup.setFlat(False)
        self.verticalLayout_2 = QVBoxLayout(self.leftGroup)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.leftFormLayout = QFormLayout()
        self.leftFormLayout.setObjectName(u"leftFormLayout")
        self.Left500HzLabel = QLabel(self.leftGroup)
        self.Left500HzLabel.setObjectName(u"Left500HzLabel")

        self.leftFormLayout.setWidget(0, QFormLayout.ItemRole.LabelRole, self.Left500HzLabel)

        self.Left500HzSpinBox = QSpinBox(self.leftGroup)
        self.Left500HzSpinBox.setObjectName(u"Left500HzSpinBox")
        self.Left500HzSpinBox.setSingleStep(1)

        self.leftFormLayout.setWidget(0, QFormLayout.ItemRole.FieldRole, self.Left500HzSpinBox)

        self.Left1000HzLabel = QLabel(self.leftGroup)
        self.Left1000HzLabel.setObjectName(u"Left1000HzLabel")

        self.leftFormLayout.setWidget(1, QFormLayout.ItemRole.LabelRole, self.Left1000HzLabel)

        self.Left1000HzSpinBox = QSpinBox(self.leftGroup)
        self.Left1000HzSpinBox.setObjectName(u"Left1000HzSpinBox")

        self.leftFormLayout.setWidget(1, QFormLayout.ItemRole.FieldRole, self.Left1000HzSpinBox)

        self.Left2000HzLabel = QLabel(self.leftGroup)
        self.Left2000HzLabel.setObjectName(u"Left2000HzLabel")

        self.leftFormLayout.setWidget(2, QFormLayout.ItemRole.LabelRole, self.Left2000HzLabel)

        self.Left2000HzSpinBox = QSpinBox(self.leftGroup)
        self.Left2000HzSpinBox.setObjectName(u"Left2000HzSpinBox")

        self.leftFormLayout.setWidget(2, QFormLayout.ItemRole.FieldRole, self.Left2000HzSpinBox)

        self.Left3000HzLabel = QLabel(self.leftGroup)
        self.Left3000HzLabel.setObjectName(u"Left3000HzLabel")

        self.leftFormLayout.setWidget(3, QFormLayout.ItemRole.LabelRole, self.Left3000HzLabel)

        self.Left3000HzSpinBox = QSpinBox(self.leftGroup)
        self.Left3000HzSpinBox.setObjectName(u"Left3000HzSpinBox")

        self.leftFormLayout.setWidget(3, QFormLayout.ItemRole.FieldRole, self.Left3000HzSpinBox)

        self.Left4000HzLabel = QLabel(self.leftGroup)
        self.Left4000HzLabel.setObjectName(u"Left4000HzLabel")

        self.leftFormLayout.setWidget(4, QFormLayout.ItemRole.LabelRole, self.Left4000HzLabel)

        self.Left4000HzSpinBox = QSpinBox(self.leftGroup)
        self.Left4000HzSpinBox.setObjectName(u"Left4000HzSpinBox")

        self.leftFormLayout.setWidget(4, QFormLayout.ItemRole.FieldRole, self.Left4000HzSpinBox)

        self.Left6000HzLabel = QLabel(self.leftGroup)
        self.Left6000HzLabel.setObjectName(u"Left6000HzLabel")

        self.leftFormLayout.setWidget(5, QFormLayout.ItemRole.LabelRole, self.Left6000HzLabel)

        self.Left6000HzSpinBox = QSpinBox(self.leftGroup)
        self.Left6000HzSpinBox.setObjectName(u"Left6000HzSpinBox")

        self.leftFormLayout.setWidget(5, QFormLayout.ItemRole.FieldRole, self.Left6000HzSpinBox)

        self.Left8000HzLabel = QLabel(self.leftGroup)
        self.Left8000HzLabel.setObjectName(u"Left8000HzLabel")

        self.leftFormLayout.setWidget(6, QFormLayout.ItemRole.LabelRole, self.Left8000HzLabel)

        self.Left8000HzSpinBox = QSpinBox(self.leftGroup)
        self.Left8000HzSpinBox.setObjectName(u"Left8000HzSpinBox")

        self.leftFormLayout.setWidget(6, QFormLayout.ItemRole.FieldRole, self.Left8000HzSpinBox)


        self.verticalLayout_2.addLayout(self.leftFormLayout)


        self.horizontalLayout.addWidget(self.leftGroup)

        self.rightGroup = QGroupBox(HearingMeasurements)
        self.rightGroup.setObjectName(u"rightGroup")
        sizePolicy.setHeightForWidth(self.rightGroup.sizePolicy().hasHeightForWidth())
        self.rightGroup.setSizePolicy(sizePolicy)
        self.rightGroup.setMinimumSize(QSize(0, 0))
        self.rightGroup.setMaximumSize(QSize(16777215, 16777215))
        self.verticalLayout_3 = QVBoxLayout(self.rightGroup)
        self.verticalLayout_3.setObjectName(u"verticalLayout_3")
        self.rightFormLayout = QFormLayout()
        self.rightFormLayout.setObjectName(u"rightFormLayout")
        self.Right500HzLabel = QLabel(self.rightGroup)
        self.Right500HzLabel.setObjectName(u"Right500HzLabel")

        self.rightFormLayout.setWidget(0, QFormLayout.ItemRole.LabelRole, self.Right500HzLabel)

        self.Right500HzSpinBox = QSpinBox(self.rightGroup)
        self.Right500HzSpinBox.setObjectName(u"Right500HzSpinBox")

        self.rightFormLayout.setWidget(0, QFormLayout.ItemRole.FieldRole, self.Right500HzSpinBox)

        self.Right1000HzLabel = QLabel(self.rightGroup)
        self.Right1000HzLabel.setObjectName(u"Right1000HzLabel")

        self.rightFormLayout.setWidget(1, QFormLayout.ItemRole.LabelRole, self.Right1000HzLabel)

        self.Right1000HzSpinBox = QSpinBox(self.rightGroup)
        self.Right1000HzSpinBox.setObjectName(u"Right1000HzSpinBox")

        self.rightFormLayout.setWidget(1, QFormLayout.ItemRole.FieldRole, self.Right1000HzSpinBox)

        self.Right2000HzLabel = QLabel(self.rightGroup)
        self.Right2000HzLabel.setObjectName(u"Right2000HzLabel")

        self.rightFormLayout.setWidget(2, QFormLayout.ItemRole.LabelRole, self.Right2000HzLabel)

        self.Right2000HzSpinBox = QSpinBox(self.rightGroup)
        self.Right2000HzSpinBox.setObjectName(u"Right2000HzSpinBox")

        self.rightFormLayout.setWidget(2, QFormLayout.ItemRole.FieldRole, self.Right2000HzSpinBox)

        self.Right3000HzLabel = QLabel(self.rightGroup)
        self.Right3000HzLabel.setObjectName(u"Right3000HzLabel")

        self.rightFormLayout.setWidget(3, QFormLayout.ItemRole.LabelRole, self.Right3000HzLabel)

        self.Right3000HzSpinBox = QSpinBox(self.rightGroup)
        self.Right3000HzSpinBox.setObjectName(u"Right3000HzSpinBox")

        self.rightFormLayout.setWidget(3, QFormLayout.ItemRole.FieldRole, self.Right3000HzSpinBox)

        self.Right4000HzLabel = QLabel(self.rightGroup)
        self.Right4000HzLabel.setObjectName(u"Right4000HzLabel")

        self.rightFormLayout.setWidget(4, QFormLayout.ItemRole.LabelRole, self.Right4000HzLabel)

        self.Right4000HzSpinBox = QSpinBox(self.rightGroup)
        self.Right4000HzSpinBox.setObjectName(u"Right4000HzSpinBox")

        self.rightFormLayout.setWidget(4, QFormLayout.ItemRole.FieldRole, self.Right4000HzSpinBox)

        self.Right6000HzLabel = QLabel(self.rightGroup)
        self.Right6000HzLabel.setObjectName(u"Right6000HzLabel")

        self.rightFormLayout.setWidget(5, QFormLayout.ItemRole.LabelRole, self.Right6000HzLabel)

        self.Right6000HzSpinBox = QSpinBox(self.rightGroup)
        self.Right6000HzSpinBox.setObjectName(u"Right6000HzSpinBox")

        self.rightFormLayout.setWidget(5, QFormLayout.ItemRole.FieldRole, self.Right6000HzSpinBox)

        self.Right8000HzLabel = QLabel(self.rightGroup)
        self.Right8000HzLabel.setObjectName(u"Right8000HzLabel")

        self.rightFormLayout.setWidget(6, QFormLayout.ItemRole.LabelRole, self.Right8000HzLabel)

        self.Right8000HzSpinBox = QSpinBox(self.rightGroup)
        self.Right8000HzSpinBox.setObjectName(u"Right8000HzSpinBox")

        self.rightFormLayout.setWidget(6, QFormLayout.ItemRole.FieldRole, self.Right8000HzSpinBox)


        self.verticalLayout_3.addLayout(self.rightFormLayout)


        self.horizontalLayout.addWidget(self.rightGroup)


        self.verticalLayout.addLayout(self.horizontalLayout)

        self.verticalLayout.setStretch(1, 1)

        self.retranslateUi(HearingMeasurements)

        QMetaObject.connectSlotsByName(HearingMeasurements)
    # setupUi

    def retranslateUi(self, HearingMeasurements):
        HearingMeasurements.setWindowTitle(QCoreApplication.translate("HearingMeasurements", u"Form", None))
        self.manualEntryButton.setText(QCoreApplication.translate("HearingMeasurements", u"Manual Entry", None))
        self.saveButton.setText(QCoreApplication.translate("HearingMeasurements", u"Save", None))
        self.measureButton.setText(QCoreApplication.translate("HearingMeasurements", u"Measure", None))
        self.submitButton.setText(QCoreApplication.translate("HearingMeasurements", u"Submit", None))
        self.leftGroup.setTitle(QCoreApplication.translate("HearingMeasurements", u"Left Ear", None))
        self.Left500HzLabel.setText(QCoreApplication.translate("HearingMeasurements", u"500 Hz", None))
        self.Left500HzSpinBox.setSpecialValueText("")
        self.Left1000HzLabel.setText(QCoreApplication.translate("HearingMeasurements", u"1000 Hz", None))
        self.Left1000HzSpinBox.setSpecialValueText("")
        self.Left2000HzLabel.setText(QCoreApplication.translate("HearingMeasurements", u"2000 Hz", None))
        self.Left2000HzSpinBox.setSpecialValueText("")
        self.Left3000HzLabel.setText(QCoreApplication.translate("HearingMeasurements", u"3000 Hz", None))
        self.Left3000HzSpinBox.setSpecialValueText("")
        self.Left4000HzLabel.setText(QCoreApplication.translate("HearingMeasurements", u"4000 Hz", None))
        self.Left4000HzSpinBox.setSpecialValueText("")
        self.Left6000HzLabel.setText(QCoreApplication.translate("HearingMeasurements", u"6000 Hz", None))
        self.Left6000HzSpinBox.setSpecialValueText("")
        self.Left8000HzLabel.setText(QCoreApplication.translate("HearingMeasurements", u"8000 Hz", None))
        self.Left8000HzSpinBox.setSpecialValueText("")
        self.rightGroup.setTitle(QCoreApplication.translate("HearingMeasurements", u"Right Ear", None))
        self.Right500HzLabel.setText(QCoreApplication.translate("HearingMeasurements", u"500 Hz", None))
        self.Right500HzSpinBox.setSpecialValueText("")
        self.Right1000HzLabel.setText(QCoreApplication.translate("HearingMeasurements", u"1000 Hz", None))
        self.Right1000HzSpinBox.setSpecialValueText("")
        self.Right1000HzSpinBox.setSuffix("")
        self.Right1000HzSpinBox.setPrefix("")
        self.Right2000HzLabel.setText(QCoreApplication.translate("HearingMeasurements", u"2000 Hz", None))
        self.Right2000HzSpinBox.setSpecialValueText("")
        self.Right3000HzLabel.setText(QCoreApplication.translate("HearingMeasurements", u"3000 Hz", None))
        self.Right3000HzSpinBox.setSpecialValueText("")
        self.Right4000HzLabel.setText(QCoreApplication.translate("HearingMeasurements", u"4000 Hz", None))
        self.Right4000HzSpinBox.setSpecialValueText("")
        self.Right6000HzLabel.setText(QCoreApplication.translate("HearingMeasurements", u"6000 Hz", None))
        self.Right6000HzSpinBox.setSpecialValueText("")
        self.Right8000HzLabel.setText(QCoreApplication.translate("HearingMeasurements", u"8000 Hz", None))
        self.Right8000HzSpinBox.setSpecialValueText("")
    # retranslateUi

