# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'audiometer_manual_entry.ui'
##
## Created by: Qt User Interface Compiler version 6.10.2
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (
    QCoreApplication,
    QDate,
    QDateTime,
    QLocale,
    QMetaObject,
    QObject,
    QPoint,
    QRect,
    QSize,
    QTime,
    QUrl,
    Qt,
)
from PySide6.QtGui import (
    QBrush,
    QColor,
    QConicalGradient,
    QCursor,
    QFont,
    QFontDatabase,
    QGradient,
    QIcon,
    QImage,
    QKeySequence,
    QLinearGradient,
    QPainter,
    QPalette,
    QPixmap,
    QRadialGradient,
    QTransform,
)
from PySide6.QtWidgets import (
    QAbstractButton,
    QApplication,
    QDialog,
    QDialogButtonBox,
    QFormLayout,
    QGroupBox,
    QHBoxLayout,
    QLabel,
    QSizePolicy,
    QSpinBox,
    QVBoxLayout,
    QWidget,
)


class Ui_AudiometerManualEntryDialog(object):
    def setupUi(self, AudiometerManualEntryDialog):
        if not AudiometerManualEntryDialog.objectName():
            AudiometerManualEntryDialog.setObjectName("AudiometerManualEntryDialog")
        AudiometerManualEntryDialog.resize(344, 310)
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(
            AudiometerManualEntryDialog.sizePolicy().hasHeightForWidth()
        )
        AudiometerManualEntryDialog.setSizePolicy(sizePolicy)
        AudiometerManualEntryDialog.setModal(True)
        self.verticalLayout = QVBoxLayout(AudiometerManualEntryDialog)
        self.verticalLayout.setObjectName("verticalLayout")
        self.horizontalLayout = QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.leftGroup = QGroupBox(AudiometerManualEntryDialog)
        self.leftGroup.setObjectName("leftGroup")
        self.verticalLayout_2 = QVBoxLayout(self.leftGroup)
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.leftFormLayout = QFormLayout()
        self.leftFormLayout.setObjectName("leftFormLayout")
        self.Left500HzLabel = QLabel(self.leftGroup)
        self.Left500HzLabel.setObjectName("Left500HzLabel")

        self.leftFormLayout.setWidget(
            0, QFormLayout.ItemRole.LabelRole, self.Left500HzLabel
        )

        self.Left500HzSpinBox = QSpinBox(self.leftGroup)
        self.Left500HzSpinBox.setObjectName("Left500HzSpinBox")
        self.Left500HzSpinBox.setSingleStep(1)

        self.leftFormLayout.setWidget(
            0, QFormLayout.ItemRole.FieldRole, self.Left500HzSpinBox
        )

        self.Left1000HzLabel = QLabel(self.leftGroup)
        self.Left1000HzLabel.setObjectName("Left1000HzLabel")

        self.leftFormLayout.setWidget(
            1, QFormLayout.ItemRole.LabelRole, self.Left1000HzLabel
        )

        self.Left1000HzSpinBox = QSpinBox(self.leftGroup)
        self.Left1000HzSpinBox.setObjectName("Left1000HzSpinBox")

        self.leftFormLayout.setWidget(
            1, QFormLayout.ItemRole.FieldRole, self.Left1000HzSpinBox
        )

        self.Left2000HzLabel = QLabel(self.leftGroup)
        self.Left2000HzLabel.setObjectName("Left2000HzLabel")

        self.leftFormLayout.setWidget(
            2, QFormLayout.ItemRole.LabelRole, self.Left2000HzLabel
        )

        self.Left2000HzSpinBox = QSpinBox(self.leftGroup)
        self.Left2000HzSpinBox.setObjectName("Left2000HzSpinBox")

        self.leftFormLayout.setWidget(
            2, QFormLayout.ItemRole.FieldRole, self.Left2000HzSpinBox
        )

        self.Left3000HzLabel = QLabel(self.leftGroup)
        self.Left3000HzLabel.setObjectName("Left3000HzLabel")

        self.leftFormLayout.setWidget(
            3, QFormLayout.ItemRole.LabelRole, self.Left3000HzLabel
        )

        self.Left3000HzSpinBox = QSpinBox(self.leftGroup)
        self.Left3000HzSpinBox.setObjectName("Left3000HzSpinBox")

        self.leftFormLayout.setWidget(
            3, QFormLayout.ItemRole.FieldRole, self.Left3000HzSpinBox
        )

        self.Left4000HzLabel = QLabel(self.leftGroup)
        self.Left4000HzLabel.setObjectName("Left4000HzLabel")

        self.leftFormLayout.setWidget(
            4, QFormLayout.ItemRole.LabelRole, self.Left4000HzLabel
        )

        self.Left4000HzSpinBox = QSpinBox(self.leftGroup)
        self.Left4000HzSpinBox.setObjectName("Left4000HzSpinBox")

        self.leftFormLayout.setWidget(
            4, QFormLayout.ItemRole.FieldRole, self.Left4000HzSpinBox
        )

        self.Left6000HzLabel = QLabel(self.leftGroup)
        self.Left6000HzLabel.setObjectName("Left6000HzLabel")

        self.leftFormLayout.setWidget(
            5, QFormLayout.ItemRole.LabelRole, self.Left6000HzLabel
        )

        self.Left6000HzSpinBox = QSpinBox(self.leftGroup)
        self.Left6000HzSpinBox.setObjectName("Left6000HzSpinBox")

        self.leftFormLayout.setWidget(
            5, QFormLayout.ItemRole.FieldRole, self.Left6000HzSpinBox
        )

        self.Left8000HzLabel = QLabel(self.leftGroup)
        self.Left8000HzLabel.setObjectName("Left8000HzLabel")

        self.leftFormLayout.setWidget(
            6, QFormLayout.ItemRole.LabelRole, self.Left8000HzLabel
        )

        self.Left8000HzSpinBox = QSpinBox(self.leftGroup)
        self.Left8000HzSpinBox.setObjectName("Left8000HzSpinBox")

        self.leftFormLayout.setWidget(
            6, QFormLayout.ItemRole.FieldRole, self.Left8000HzSpinBox
        )

        self.verticalLayout_2.addLayout(self.leftFormLayout)

        self.horizontalLayout.addWidget(self.leftGroup)

        self.rightGroup = QGroupBox(AudiometerManualEntryDialog)
        self.rightGroup.setObjectName("rightGroup")
        self.verticalLayout_3 = QVBoxLayout(self.rightGroup)
        self.verticalLayout_3.setObjectName("verticalLayout_3")
        self.rightFormLayout = QFormLayout()
        self.rightFormLayout.setObjectName("rightFormLayout")
        self.Right500HzLabel = QLabel(self.rightGroup)
        self.Right500HzLabel.setObjectName("Right500HzLabel")

        self.rightFormLayout.setWidget(
            0, QFormLayout.ItemRole.LabelRole, self.Right500HzLabel
        )

        self.Right500HzSpinBox = QSpinBox(self.rightGroup)
        self.Right500HzSpinBox.setObjectName("Right500HzSpinBox")

        self.rightFormLayout.setWidget(
            0, QFormLayout.ItemRole.FieldRole, self.Right500HzSpinBox
        )

        self.Right1000HzLabel = QLabel(self.rightGroup)
        self.Right1000HzLabel.setObjectName("Right1000HzLabel")

        self.rightFormLayout.setWidget(
            1, QFormLayout.ItemRole.LabelRole, self.Right1000HzLabel
        )

        self.Right1000HzSpinBox = QSpinBox(self.rightGroup)
        self.Right1000HzSpinBox.setObjectName("Right1000HzSpinBox")

        self.rightFormLayout.setWidget(
            1, QFormLayout.ItemRole.FieldRole, self.Right1000HzSpinBox
        )

        self.Right2000HzLabel = QLabel(self.rightGroup)
        self.Right2000HzLabel.setObjectName("Right2000HzLabel")

        self.rightFormLayout.setWidget(
            2, QFormLayout.ItemRole.LabelRole, self.Right2000HzLabel
        )

        self.Right2000HzSpinBox = QSpinBox(self.rightGroup)
        self.Right2000HzSpinBox.setObjectName("Right2000HzSpinBox")

        self.rightFormLayout.setWidget(
            2, QFormLayout.ItemRole.FieldRole, self.Right2000HzSpinBox
        )

        self.Right3000HzLabel = QLabel(self.rightGroup)
        self.Right3000HzLabel.setObjectName("Right3000HzLabel")

        self.rightFormLayout.setWidget(
            3, QFormLayout.ItemRole.LabelRole, self.Right3000HzLabel
        )

        self.Right3000HzSpinBox = QSpinBox(self.rightGroup)
        self.Right3000HzSpinBox.setObjectName("Right3000HzSpinBox")

        self.rightFormLayout.setWidget(
            3, QFormLayout.ItemRole.FieldRole, self.Right3000HzSpinBox
        )

        self.Right4000HzLabel = QLabel(self.rightGroup)
        self.Right4000HzLabel.setObjectName("Right4000HzLabel")

        self.rightFormLayout.setWidget(
            4, QFormLayout.ItemRole.LabelRole, self.Right4000HzLabel
        )

        self.Right4000HzSpinBox = QSpinBox(self.rightGroup)
        self.Right4000HzSpinBox.setObjectName("Right4000HzSpinBox")

        self.rightFormLayout.setWidget(
            4, QFormLayout.ItemRole.FieldRole, self.Right4000HzSpinBox
        )

        self.Right6000HzLabel = QLabel(self.rightGroup)
        self.Right6000HzLabel.setObjectName("Right6000HzLabel")

        self.rightFormLayout.setWidget(
            5, QFormLayout.ItemRole.LabelRole, self.Right6000HzLabel
        )

        self.Right6000HzSpinBox = QSpinBox(self.rightGroup)
        self.Right6000HzSpinBox.setObjectName("Right6000HzSpinBox")

        self.rightFormLayout.setWidget(
            5, QFormLayout.ItemRole.FieldRole, self.Right6000HzSpinBox
        )

        self.Right8000HzLabel = QLabel(self.rightGroup)
        self.Right8000HzLabel.setObjectName("Right8000HzLabel")

        self.rightFormLayout.setWidget(
            6, QFormLayout.ItemRole.LabelRole, self.Right8000HzLabel
        )

        self.Right8000HzSpinBox = QSpinBox(self.rightGroup)
        self.Right8000HzSpinBox.setObjectName("Right8000HzSpinBox")

        self.rightFormLayout.setWidget(
            6, QFormLayout.ItemRole.FieldRole, self.Right8000HzSpinBox
        )

        self.verticalLayout_3.addLayout(self.rightFormLayout)

        self.horizontalLayout.addWidget(self.rightGroup)

        self.verticalLayout.addLayout(self.horizontalLayout)

        self.buttonBox = QDialogButtonBox(AudiometerManualEntryDialog)
        self.buttonBox.setObjectName("buttonBox")
        self.buttonBox.setOrientation(Qt.Orientation.Horizontal)
        self.buttonBox.setStandardButtons(
            QDialogButtonBox.StandardButton.Cancel | QDialogButtonBox.StandardButton.Ok
        )

        self.verticalLayout.addWidget(self.buttonBox)

        self.retranslateUi(AudiometerManualEntryDialog)
        self.buttonBox.accepted.connect(AudiometerManualEntryDialog.accept)
        self.buttonBox.rejected.connect(AudiometerManualEntryDialog.reject)

        QMetaObject.connectSlotsByName(AudiometerManualEntryDialog)

    # setupUi

    def retranslateUi(self, AudiometerManualEntryDialog):
        AudiometerManualEntryDialog.setWindowTitle(
            QCoreApplication.translate(
                "AudiometerManualEntryDialog", "Manual Entry", None
            )
        )
        self.leftGroup.setTitle(
            QCoreApplication.translate("AudiometerManualEntryDialog", "Left Ear", None)
        )
        self.Left500HzLabel.setText(
            QCoreApplication.translate("AudiometerManualEntryDialog", "500 Hz", None)
        )
        self.Left500HzSpinBox.setSpecialValueText("")
        self.Left1000HzLabel.setText(
            QCoreApplication.translate("AudiometerManualEntryDialog", "1000 Hz", None)
        )
        self.Left1000HzSpinBox.setSpecialValueText("")
        self.Left2000HzLabel.setText(
            QCoreApplication.translate("AudiometerManualEntryDialog", "2000 Hz", None)
        )
        self.Left2000HzSpinBox.setSpecialValueText("")
        self.Left3000HzLabel.setText(
            QCoreApplication.translate("AudiometerManualEntryDialog", "3000 Hz", None)
        )
        self.Left3000HzSpinBox.setSpecialValueText("")
        self.Left4000HzLabel.setText(
            QCoreApplication.translate("AudiometerManualEntryDialog", "4000 Hz", None)
        )
        self.Left4000HzSpinBox.setSpecialValueText("")
        self.Left6000HzLabel.setText(
            QCoreApplication.translate("AudiometerManualEntryDialog", "6000 Hz", None)
        )
        self.Left6000HzSpinBox.setSpecialValueText("")
        self.Left8000HzLabel.setText(
            QCoreApplication.translate("AudiometerManualEntryDialog", "8000 Hz", None)
        )
        self.Left8000HzSpinBox.setSpecialValueText("")
        self.rightGroup.setTitle(
            QCoreApplication.translate("AudiometerManualEntryDialog", "Right Ear", None)
        )
        self.Right500HzLabel.setText(
            QCoreApplication.translate("AudiometerManualEntryDialog", "500 Hz", None)
        )
        self.Right500HzSpinBox.setSpecialValueText("")
        self.Right1000HzLabel.setText(
            QCoreApplication.translate("AudiometerManualEntryDialog", "1000 Hz", None)
        )
        self.Right1000HzSpinBox.setSpecialValueText("")
        self.Right1000HzSpinBox.setSuffix("")
        self.Right1000HzSpinBox.setPrefix("")
        self.Right2000HzLabel.setText(
            QCoreApplication.translate("AudiometerManualEntryDialog", "2000 Hz", None)
        )
        self.Right2000HzSpinBox.setSpecialValueText("")
        self.Right3000HzLabel.setText(
            QCoreApplication.translate("AudiometerManualEntryDialog", "3000 Hz", None)
        )
        self.Right3000HzSpinBox.setSpecialValueText("")
        self.Right4000HzLabel.setText(
            QCoreApplication.translate("AudiometerManualEntryDialog", "4000 Hz", None)
        )
        self.Right4000HzSpinBox.setSpecialValueText("")
        self.Right6000HzLabel.setText(
            QCoreApplication.translate("AudiometerManualEntryDialog", "6000 Hz", None)
        )
        self.Right6000HzSpinBox.setSpecialValueText("")
        self.Right8000HzLabel.setText(
            QCoreApplication.translate("AudiometerManualEntryDialog", "8000 Hz", None)
        )
        self.Right8000HzSpinBox.setSpecialValueText("")

    # retranslateUi
