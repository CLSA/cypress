/********************************************************************************
** Form generated from reading UI file 'file_picker_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILE_PICKER_WIDGET_H
#define UI_FILE_PICKER_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FilePickerWidget
{
public:
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QLineEdit *filePath;
    QPushButton *pathButton;

    void setupUi(QWidget *FilePickerWidget)
    {
        if (FilePickerWidget->objectName().isEmpty())
            FilePickerWidget->setObjectName(QString::fromUtf8("FilePickerWidget"));
        FilePickerWidget->resize(439, 85);
        horizontalLayout_2 = new QHBoxLayout(FilePickerWidget);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(FilePickerWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        filePath = new QLineEdit(groupBox);
        filePath->setObjectName(QString::fromUtf8("filePath"));

        horizontalLayout->addWidget(filePath);

        pathButton = new QPushButton(groupBox);
        pathButton->setObjectName(QString::fromUtf8("pathButton"));

        horizontalLayout->addWidget(pathButton);


        horizontalLayout_2->addWidget(groupBox);


        retranslateUi(FilePickerWidget);

        QMetaObject::connectSlotsByName(FilePickerWidget);
    } // setupUi

    void retranslateUi(QWidget *FilePickerWidget)
    {
        FilePickerWidget->setWindowTitle(QCoreApplication::translate("FilePickerWidget", "Form", nullptr));
        groupBox->setTitle(QCoreApplication::translate("FilePickerWidget", "GroupBox", nullptr));
        pathButton->setText(QCoreApplication::translate("FilePickerWidget", "Path", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FilePickerWidget: public Ui_FilePickerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILE_PICKER_WIDGET_H
