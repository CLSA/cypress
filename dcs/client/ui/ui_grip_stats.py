# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'grip_stats.ui'
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
    QPushButton, QSizePolicy, QSpacerItem, QVBoxLayout,
    QWidget)

class Ui_GripInfo(object):
    def setupUi(self, GripInfo):
        if not GripInfo.objectName():
            GripInfo.setObjectName(u"GripInfo")
        GripInfo.resize(465, 345)
        self.verticalLayout = QVBoxLayout(GripInfo)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.verticalLayout.setContentsMargins(-1, 0, -1, -1)
        self.controls = QHBoxLayout()
        self.controls.setObjectName(u"controls")
        self.manual_entry_button = QPushButton(GripInfo)
        self.manual_entry_button.setObjectName(u"manual_entry_button")

        self.controls.addWidget(self.manual_entry_button)

        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.controls.addItem(self.horizontalSpacer)

        self.measure_button = QPushButton(GripInfo)
        self.measure_button.setObjectName(u"measure_button")

        self.controls.addWidget(self.measure_button)

        self.submit_button = QPushButton(GripInfo)
        self.submit_button.setObjectName(u"submit_button")

        self.controls.addWidget(self.submit_button)


        self.verticalLayout.addLayout(self.controls)

        self.summary = QGroupBox(GripInfo)
        self.summary.setObjectName(u"summary")
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.summary.sizePolicy().hasHeightForWidth())
        self.summary.setSizePolicy(sizePolicy)
        self.summary.setFlat(False)
        self.verticalLayout_3 = QVBoxLayout(self.summary)
        self.verticalLayout_3.setObjectName(u"verticalLayout_3")
        self.verticalLayout_3.setContentsMargins(-1, 9, -1, -1)
        self.test = QHBoxLayout()
        self.test.setObjectName(u"test")
        self.test_label = QLabel(self.summary)
        self.test_label.setObjectName(u"test_label")

        self.test.addWidget(self.test_label)

        self.test_value = QLabel(self.summary)
        self.test_value.setObjectName(u"test_value")
        self.test_value.setAlignment(Qt.AlignmentFlag.AlignLeading|Qt.AlignmentFlag.AlignLeft|Qt.AlignmentFlag.AlignVCenter)

        self.test.addWidget(self.test_value)


        self.verticalLayout_3.addLayout(self.test)

        self.primary_stat = QHBoxLayout()
        self.primary_stat.setObjectName(u"primary_stat")
        self.primary_stat_label = QLabel(self.summary)
        self.primary_stat_label.setObjectName(u"primary_stat_label")

        self.primary_stat.addWidget(self.primary_stat_label)

        self.primary_stat_value = QLabel(self.summary)
        self.primary_stat_value.setObjectName(u"primary_stat_value")
        self.primary_stat_value.setAlignment(Qt.AlignmentFlag.AlignLeading|Qt.AlignmentFlag.AlignLeft|Qt.AlignmentFlag.AlignVCenter)

        self.primary_stat.addWidget(self.primary_stat_value)


        self.verticalLayout_3.addLayout(self.primary_stat)

        self.sequence = QHBoxLayout()
        self.sequence.setObjectName(u"sequence")
        self.sequence_label = QLabel(self.summary)
        self.sequence_label.setObjectName(u"sequence_label")

        self.sequence.addWidget(self.sequence_label)

        self.sequence_value = QLabel(self.summary)
        self.sequence_value.setObjectName(u"sequence_value")
        self.sequence_value.setAlignment(Qt.AlignmentFlag.AlignLeading|Qt.AlignmentFlag.AlignLeft|Qt.AlignmentFlag.AlignVCenter)

        self.sequence.addWidget(self.sequence_value)


        self.verticalLayout_3.addLayout(self.sequence)

        self.rung = QHBoxLayout()
        self.rung.setObjectName(u"rung")
        self.rung_label = QLabel(self.summary)
        self.rung_label.setObjectName(u"rung_label")

        self.rung.addWidget(self.rung_label)

        self.rung_value = QLabel(self.summary)
        self.rung_value.setObjectName(u"rung_value")
        self.rung_value.setAlignment(Qt.AlignmentFlag.AlignLeading|Qt.AlignmentFlag.AlignLeft|Qt.AlignmentFlag.AlignVCenter)

        self.rung.addWidget(self.rung_value)


        self.verticalLayout_3.addLayout(self.rung)

        self.max_reps = QHBoxLayout()
        self.max_reps.setObjectName(u"max_reps")
        self.max_reps_label = QLabel(self.summary)
        self.max_reps_label.setObjectName(u"max_reps_label")

        self.max_reps.addWidget(self.max_reps_label)

        self.max_reps_value = QLabel(self.summary)
        self.max_reps_value.setObjectName(u"max_reps_value")
        self.max_reps_value.setAlignment(Qt.AlignmentFlag.AlignLeading|Qt.AlignmentFlag.AlignLeft|Qt.AlignmentFlag.AlignVCenter)

        self.max_reps.addWidget(self.max_reps_value)


        self.verticalLayout_3.addLayout(self.max_reps)

        self.rep1 = QHBoxLayout()
        self.rep1.setObjectName(u"rep1")
        self.rep1_label = QLabel(self.summary)
        self.rep1_label.setObjectName(u"rep1_label")

        self.rep1.addWidget(self.rep1_label)

        self.rep1_value = QLabel(self.summary)
        self.rep1_value.setObjectName(u"rep1_value")
        self.rep1_value.setAlignment(Qt.AlignmentFlag.AlignLeading|Qt.AlignmentFlag.AlignLeft|Qt.AlignmentFlag.AlignVCenter)

        self.rep1.addWidget(self.rep1_value)


        self.verticalLayout_3.addLayout(self.rep1)

        self.rep2 = QHBoxLayout()
        self.rep2.setObjectName(u"rep2")
        self.rep2_label = QLabel(self.summary)
        self.rep2_label.setObjectName(u"rep2_label")

        self.rep2.addWidget(self.rep2_label)

        self.rep2_value = QLabel(self.summary)
        self.rep2_value.setObjectName(u"rep2_value")
        self.rep2_value.setAlignment(Qt.AlignmentFlag.AlignLeading|Qt.AlignmentFlag.AlignLeft|Qt.AlignmentFlag.AlignVCenter)

        self.rep2.addWidget(self.rep2_value)


        self.verticalLayout_3.addLayout(self.rep2)

        self.rep3 = QHBoxLayout()
        self.rep3.setObjectName(u"rep3")
        self.rep3_label = QLabel(self.summary)
        self.rep3_label.setObjectName(u"rep3_label")

        self.rep3.addWidget(self.rep3_label)

        self.rep3_value = QLabel(self.summary)
        self.rep3_value.setObjectName(u"rep3_value")
        self.rep3_value.setAlignment(Qt.AlignmentFlag.AlignLeading|Qt.AlignmentFlag.AlignLeft|Qt.AlignmentFlag.AlignVCenter)

        self.rep3.addWidget(self.rep3_value)


        self.verticalLayout_3.addLayout(self.rep3)

        self.avg = QHBoxLayout()
        self.avg.setObjectName(u"avg")
        self.avg_label = QLabel(self.summary)
        self.avg_label.setObjectName(u"avg_label")

        self.avg.addWidget(self.avg_label)

        self.avg_value = QLabel(self.summary)
        self.avg_value.setObjectName(u"avg_value")
        self.avg_value.setAlignment(Qt.AlignmentFlag.AlignLeading|Qt.AlignmentFlag.AlignLeft|Qt.AlignmentFlag.AlignVCenter)

        self.avg.addWidget(self.avg_value)


        self.verticalLayout_3.addLayout(self.avg)

        self.max = QHBoxLayout()
        self.max.setObjectName(u"max")
        self.max_label = QLabel(self.summary)
        self.max_label.setObjectName(u"max_label")

        self.max.addWidget(self.max_label)

        self.max_value = QLabel(self.summary)
        self.max_value.setObjectName(u"max_value")
        self.max_value.setAlignment(Qt.AlignmentFlag.AlignLeading|Qt.AlignmentFlag.AlignLeft|Qt.AlignmentFlag.AlignVCenter)

        self.max.addWidget(self.max_value)


        self.verticalLayout_3.addLayout(self.max)

        self.cv = QHBoxLayout()
        self.cv.setObjectName(u"cv")
        self.cv_label = QLabel(self.summary)
        self.cv_label.setObjectName(u"cv_label")

        self.cv.addWidget(self.cv_label)

        self.cv_value = QLabel(self.summary)
        self.cv_value.setObjectName(u"cv_value")
        self.cv_value.setAlignment(Qt.AlignmentFlag.AlignLeading|Qt.AlignmentFlag.AlignLeft|Qt.AlignmentFlag.AlignVCenter)

        self.cv.addWidget(self.cv_value)


        self.verticalLayout_3.addLayout(self.cv)


        self.verticalLayout.addWidget(self.summary)

        self.verticalSpacer = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.verticalLayout.addItem(self.verticalSpacer)


        self.retranslateUi(GripInfo)

        QMetaObject.connectSlotsByName(GripInfo)
    # setupUi

    def retranslateUi(self, GripInfo):
        GripInfo.setWindowTitle(QCoreApplication.translate("GripInfo", u"Form", None))
        self.manual_entry_button.setText(QCoreApplication.translate("GripInfo", u"Manual Entry", None))
        self.measure_button.setText(QCoreApplication.translate("GripInfo", u"Measure", None))
        self.submit_button.setText(QCoreApplication.translate("GripInfo", u"Submit", None))
        self.summary.setTitle(QCoreApplication.translate("GripInfo", u"Results", None))
        self.test_label.setText(QCoreApplication.translate("GripInfo", u"Test", None))
        self.test_value.setText(QCoreApplication.translate("GripInfo", u"---", None))
        self.primary_stat_label.setText(QCoreApplication.translate("GripInfo", u"Primary Stat", None))
        self.primary_stat_value.setText(QCoreApplication.translate("GripInfo", u"---", None))
        self.sequence_label.setText(QCoreApplication.translate("GripInfo", u"Sequence", None))
        self.sequence_value.setText(QCoreApplication.translate("GripInfo", u"---", None))
        self.rung_label.setText(QCoreApplication.translate("GripInfo", u"Rung", None))
        self.rung_value.setText(QCoreApplication.translate("GripInfo", u"---", None))
        self.max_reps_label.setText(QCoreApplication.translate("GripInfo", u"Max Reps", None))
        self.max_reps_value.setText(QCoreApplication.translate("GripInfo", u"---", None))
        self.rep1_label.setText(QCoreApplication.translate("GripInfo", u"Rep 1", None))
        self.rep1_value.setText(QCoreApplication.translate("GripInfo", u"---", None))
        self.rep2_label.setText(QCoreApplication.translate("GripInfo", u"Rep 2", None))
        self.rep2_value.setText(QCoreApplication.translate("GripInfo", u"---", None))
        self.rep3_label.setText(QCoreApplication.translate("GripInfo", u"Rep 3", None))
        self.rep3_value.setText(QCoreApplication.translate("GripInfo", u"---", None))
        self.avg_label.setText(QCoreApplication.translate("GripInfo", u"Average", None))
        self.avg_value.setText(QCoreApplication.translate("GripInfo", u"---", None))
        self.max_label.setText(QCoreApplication.translate("GripInfo", u"Maximum", None))
        self.max_value.setText(QCoreApplication.translate("GripInfo", u"---", None))
        self.cv_label.setText(QCoreApplication.translate("GripInfo", u"CV", None))
        self.cv_value.setText(QCoreApplication.translate("GripInfo", u"---", None))
    # retranslateUi

